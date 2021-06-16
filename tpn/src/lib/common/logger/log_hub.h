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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "log_common.h"

namespace tpn {

namespace log {

/// 日志中枢
class TPN_COMMON_API LogHub {
 public:
  void Init();

  void RegisterLogger(LoggerSptr new_logger);

  void InitializeLogger(LoggerSptr new_logger);

  LoggerSptr GetLoggerByName(std::string_view logger_name);

  LoggerSptr GetDefaultLogger();

  void SetDefaultLogger(LoggerSptr new_default_logger);

  void SetGlobalLevel(LogLevel level);

  void SetGlobalFlushLevel(LogLevel level);

  void FlushAll();

  void SetErrHandler(ErrHandler err_handler);

  void ApplyAll(const std::function<void(const LoggerSptr)> &func);

  void Drop(std::string_view logger_name);

  void DropAll();

  void Shutdown();

  void SetAutomaticRegistration(bool automatic_registration);

 private:
  std::mutex logger_map_mutex_;                          ///< 日志记录器锁
  std::mutex flush_mutex_;                               ///< 刷新锁
  std::unordered_map<std::string, LoggerSptr> loggers_;  ///< 注册的日志记录器
  std::unordered_map<std::string, LogLevel> levels_;  ///< 注册的日志级别
  LogLevel global_log_level_{LogLevel::kLogLevelDebug};  ///< 全局志记级别
  LogLevel global_flush_level_{LogLevel::kLogLevelDebug};  ///< 全局刷新级别
  ErrHandler global_err_handler_{nullptr};  ///< 全局错误处理
  LoggerSptr default_logger_{nullptr};      ///< 默认记录器
  bool automatic_registration_{true};       ///< 是否默认注册
  PatternTimeType pattern_time_type_{
      PatternTimeType::kPatternTimeTypeLocal};  ///< 模式时间

  TPN_SINGLETON_DECL(LogHub)
};

}  // namespace log

}  // namespace tpn

/// global log hub instance
#define g_log_hub tpn::log::LogHub::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_
