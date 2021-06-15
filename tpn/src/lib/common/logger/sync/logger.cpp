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

#include "logger.h"

#include "platform.h"
#include "appender.h"

namespace tpn {
namespace log {

Logger::Logger(std::string_view name)
    : name_{name.data(), name.size()}, appenders_{} {}

Logger::Logger(std::string_view name, AppenderInitList appenders)
    : Logger(name, appenders.begin(), appenders.end()) {}

Logger::Logger(std::string_view name, AppenderSptr single_appender)
    : Logger(name, {std::move(single_appender)}) {}

Logger::Logger(const Logger &other)
    : name_(other.name_),
      level_(other.level_.load(std::memory_order_relaxed)),
      flush_level_(other.flush_level_.load(std::memory_order_relaxed)),
      appenders_(other.appenders_),
      err_handler_(other.err_handler_) {}

Logger::Logger(Logger &&other) noexcept
    : name_{std::move(other.name_)},
      level_(other.level_.load(std::memory_order_relaxed)),
      flush_level_(other.flush_level_.load(std::memory_order_relaxed)),
      appenders_{std::move(other.appenders_)},
      err_handler_{std::move(other.err_handler_)} {}

Logger &Logger::operator=(Logger other) noexcept {
  this->Swap(other);
  return *this;
}

void Logger::Swap(Logger &other) noexcept {
  name_.swap(other.name_);

  auto other_level = other.level_.load();
  auto my_level    = level_.exchange(other_level);
  other.level_.store(my_level);

  other_level = other.flush_level_.load();
  my_level    = flush_level_.exchange(other_level);
  other.flush_level_.store(my_level);

  appenders_.swap(other.appenders_);
  err_handler_.swap(other.err_handler_);
}

void Logger::Log(LogClock::time_point log_time, SourceLocation src_loc,
                 LogLevel level, std::string_view msg) {
  bool log_enabled = ShouldLog(level);
  if (!log_enabled) {
    return;
  }
  LogMsg log_msg(name_, level, log_time, src_loc, msg);
  AppenderDoLog(log_msg);
}

void Logger::Log(SourceLocation src_loc, LogLevel level, std::string_view msg) {
  bool log_enabled = ShouldLog(level);
  if (!log_enabled) {
    return;
  }
  LogMsg log_msg(name_, level, src_loc, msg);
  AppenderDoLog(log_msg);
}

void Logger::Log(LogLevel level, std::string_view msg) {
  Log(SourceLocation{}, level, msg);
}

void Logger::Flush() { AppenderDoFlush(); }

std::string_view Logger::GetName() const { return name_; }

void Logger::SetLogLevel(LogLevel level) { level_.store(level); }

LogLevel Logger::GetLogLevel() const {
  return level_.load(std::memory_order_relaxed);
}

void Logger::SetFlushLevel(LogLevel level) { flush_level_.store(level); }

LogLevel Logger::GetFlushLevel() const {
  return flush_level_.load(std::memory_order_relaxed);
}

const AppenderSptrVec &Logger::GetAppenders() const { return appenders_; }

AppenderSptrVec &Logger::GetAppenders() { return appenders_; }

void Logger::SetErrHandler(ErrHandler handler) {
  err_handler_ = std::move(handler);
}

bool Logger::ShouldLog(LogLevel level) const {
  auto level_need = level_.load(std::memory_order_relaxed);
  return (LogLevel::kLogLevelOff != level_need) && (level >= level_need);
}

bool Logger::ShouldFlush(LogLevel level) const {
  auto level_need = flush_level_.load(std::memory_order_relaxed);
  return (LogLevel::kLogLevelOff != level_need) && (level >= level_need);
}

void Logger::DoErrhandler(std::string_view msg) {
  if (err_handler_) {
    err_handler_(msg);
  } else {
    static std::mutex mutex;
    static LogClock::time_point last_report_time;
    static size_t err_counter = 0;
    std::lock_guard<std::mutex> lock{mutex};
    auto now = LogClock::now();
    if (now - last_report_time < Seconds(1)) {
      return;
    }
    last_report_time = now;
    auto tm_time     = Localtime(LogClock::to_time_t(now));
    fmt::print(stderr, "[ErrHandler] [{:%Y-%m-%d %H:%M:%S}] [{}] [{}] {} ",
               tm_time, name_, err_counter, msg);
  }
}

void Logger::AppenderDoLog(const LogMsg &msg) {
  for (auto &&appender : appenders_) {
    if (appender->ShouldLog(msg.level)) {
      try {
        appender->Log(msg);
      } catch (const std::exception &ex) {
        DoErrhandler(ex.what());
      } catch (...) {
        DoErrhandler("Unkown exception in logger.");
      }
    }
  }
}

void Logger::AppenderDoFlush() {
  for (auto &&appender : appenders_) {
    try {
      appender->Flush();
    } catch (const std::exception &ex) {
      DoErrhandler(ex.what());
    } catch (...) {
      DoErrhandler("Unkown exception in logger.");
    }
  }
}

void Swap(Logger &a, Logger &b) { a.Swap(b); }

}  // namespace log
}  // namespace tpn
