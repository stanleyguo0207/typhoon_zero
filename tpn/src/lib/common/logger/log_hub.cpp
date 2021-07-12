//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "log_hub.h"

#include <initializer_list>

#include "utils.h"
#include "debug_hub.h"
#include "config.h"
#include "logger.h"
#include "async_logger.h"
#include "exception_hub.h"
#include "appender_console.h"
#include "appender_daily_file.h"
#include "platform.h"
#include "thread_pool.h"
#include "periodic_worker.h"

namespace tpn {

namespace log {

void LogHub::Init() {
  automatic_registration_ =
      g_config->GetBoolDefault("log_automatic_registration", true);
  global_log_level_ =
      ToLogLevelEnum(g_config->GetStringDefault("log_global_level", "DEBUG"));
  global_flush_level_ = ToLogLevelEnum(
      g_config->GetStringDefault("log_global_flush_level", "DEBUG"));
  pattern_time_type_ =
      "utc" == g_config->GetStringDefault("log_pattern_type_type", "local")
          ? PatternTimeType::kPatternTimeTypeUtc
          : PatternTimeType::kPatternTimeTypeLocal;

  // 解析levels
  auto levels_str = g_config->GetStringDefault("log_logger_levels", "");
  if (!levels_str.empty()) {
    auto tk1 = Tokenizer(levels_str, ';');
    for (auto &&level_pair_str : tk1) {
      auto tk2 = Tokenizer(level_pair_str, '-');
      TPN_ASSERT(2 == tk2.size(), "log_logger_levels pair pattern wrong. {}",
                 tk2);
      levels_[tk2[0]] = ToLogLevelEnum(tk2[1]);
    }
  }

  uint32_t thread_pool_size =
      g_config->GetU32Default("log_thread_pool_size", 1);
  thread_pool_ = std::make_shared<ThreadPool>(thread_pool_size);

  auto console_logger = std::make_shared<AppenderConsoleStdout>();
  auto daily_logger   = std::make_shared<AppenderDailyFile>(
      g_config->GetStringDefault("log_daily_file_base_path",
                                 "log/daily/daily.log"),
      g_config->GetI32Default("log_daily_file_rotation_hour", 0),
      g_config->GetI32Default("log_daily_file_rotation_minute", 0),
      g_config->GetBoolDefault("log_daily_file_truncate", false),
      static_cast<uint16_t>(g_config->GetI32Default("log_daily_file_max", 7)));
  auto default_logger = std::make_shared<AsyncLogger>(
      g_config->GetStringDefault("log_default_logger_name", "default"),
      AppenderInitList({console_logger, daily_logger}), thread_pool_);
  InitializeLogger(default_logger);
  SetDefaultLogger(std::move(default_logger));

  // 解析日志格式类型
  format_type_    = EnumToUnderlyType(FormatType::kFormatTypeDefault);
  auto format_str = g_config->GetStringDefault("log_format_types", "");
  if (!format_str.empty()) {
    auto tk1 = Tokenizer(format_str, '&');
    for (auto &&format_type_str : tk1) {
      format_type_ |= ToInteger<uint32_t>(format_type_str);
    }
  }

  uint32_t flush_interval = g_config->GetU32Default("log_flush_interval", 1000);
  FlushEvery(MilliSeconds(flush_interval));
}

void LogHub::RegisterLogger(LoggerSptr new_logger) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  DoRegisterLogger(std::move(new_logger));
}

void LogHub::InitializeLogger(LoggerSptr new_logger) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  if (global_err_handler_) {
    new_logger->SetErrHandler(global_err_handler_);
  }

  auto iter = levels_.find(
      std::string{new_logger->GetName().data(), new_logger->GetName().size()});
  auto new_level = iter != levels_.end() ? iter->second : global_log_level_;
  new_logger->SetLogLevel(new_level);
  new_logger->SetFlushLevel(global_flush_level_);

  if (automatic_registration_) {
    DoRegisterLogger(std::move(new_logger));
  }
}

LoggerSptr LogHub::GetLoggerByName(std::string_view logger_name) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  auto iter = loggers_.find(logger_name);
  return iter != loggers_.end() ? iter->second : nullptr;
}

Logger *LogHub::GetDefaultLoggerRaw() { return default_logger_.get(); }

LoggerSptr LogHub::GetDefaultLogger() {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  return default_logger_;
}

void LogHub::SetDefaultLogger(LoggerSptr new_default_logger) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  if (nullptr != default_logger_) {
    loggers_.erase(default_logger_->GetName());
  }
  if (nullptr != new_default_logger) {
    loggers_[new_default_logger->GetName()] = new_default_logger;
  }
  default_logger_ = std::move(new_default_logger);
}

void LogHub::SetGlobalLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  global_log_level_ = level;
  for (auto &&[_, logger] : loggers_) {
    logger->SetLogLevel(global_log_level_);
  }
}

void LogHub::SetGlobalFlushLevel(LogLevel level) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  global_flush_level_ = level;
  for (auto &&[_, logger] : loggers_) {
    logger->SetFlushLevel(global_flush_level_);
  }
}

void LogHub::FlushAll() {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);

  for (auto &&[_, logger] : loggers_) {
    logger->Flush();
  }
}

// template <typename Rep, typename Period>
// void LogHub::FlushEvery(std::chrono::duration<Rep, Period> interval) {
//   std::lock_guard<std::mutex> lock(flush_mutex_);
//   auto task         = [this] { this->FlushAll(); };
//   periodic_flusher_ = std::make_unique<PeriodicWorker>(task, interval);
// }

void LogHub::SetErrHandler(ErrHandler err_handler) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  global_err_handler_ = std::move(err_handler);
  for (auto &&[_, logger] : loggers_) {
    logger->SetErrHandler(global_err_handler_);
  }
}

void LogHub::ApplyAll(const std::function<void(const LoggerSptr)> &func) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  for (auto &&[_, logger] : loggers_) {
    func(logger);
  }
}

void LogHub::Drop(std::string_view logger_name) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  loggers_.erase(logger_name);
  if (default_logger_ && default_logger_->GetName() == logger_name) {
    default_logger_.reset();
  }
}

void LogHub::DropAll() {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  default_logger_.reset();
  loggers_.clear();
}

void LogHub::Shutdown() {
  FlushAll();
  {
    std::lock_guard<std::mutex> lock(flush_mutex_);
    periodic_flusher_.reset();
  }
  DropAll();
  {
    std::lock_guard<std::recursive_mutex> lock(thread_pool_mutex_);
    thread_pool_.reset();
  }
}

void LogHub::SetAutomaticRegistration(bool automatic_registration) {
  std::lock_guard<std::mutex> lock(logger_map_mutex_);
  automatic_registration_ = automatic_registration;
}

PatternTimeType LogHub::GetPatternTimeType() const {
  return pattern_time_type_;
}

std::tm LogHub::GetTime(LogClock::time_point tp) const {
  if (PatternTimeType::kPatternTimeTypeLocal == pattern_time_type_) {
    return Localtime(LogClock::to_time_t(tp));
  } else {
    return GmTime(LogClock::to_time_t(tp));
  }
}

uint32_t LogHub::GetFormatType() const { return format_type_; }

std::recursive_mutex &LogHub::GetThreadPoolMutex() {
  return thread_pool_mutex_;
}

void LogHub::SetThreadPool(ThreadPoolSptr thread_pool) {
  std::lock_guard<std::recursive_mutex> lock(thread_pool_mutex_);
  thread_pool_ = std::move(thread_pool);
}

ThreadPoolSptr LogHub::GetThreadPool() {
  std::lock_guard<std::recursive_mutex> lock(thread_pool_mutex_);
  return thread_pool_;
}

void LogHub::ThrowIfExists(std::string_view logger_name) {
  if (loggers_.count(logger_name)) {
    TPN_THROW(LogException(fmt::format(
        "LogHub::ThrowIfExists logger_name {} already exists", logger_name)));
  }
}

void LogHub::DoRegisterLogger(LoggerSptr new_logger) {
  ThrowIfExists(new_logger->GetName());
  loggers_[new_logger->GetName()] = std::move(new_logger);
}

TPN_SINGLETON_IMPL(LogHub)

}  // namespace log

}  // namespace tpn
