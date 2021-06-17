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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_H_

#include <atomic>

#include "formatter.h"
#include "log_common.h"

namespace tpn {

namespace log {

/// 追加器抽象类
class TPN_COMMON_API Appender {
 public:
  virtual ~Appender() = default;

  /// 记录日志接口
  ///  @param[in]		msg			日志信息
  virtual void Log(const LogMsg &msg) = 0;

  /// 日志刷新接口
  virtual void Flush() = 0;

  /// 设置追加器志记级别
  ///  @param[in]		level		日志级别
  void SetLevel(LogLevel level);

  /// 获取志记级别
  ///  @return 志记级别
  LogLevel GetLevel() const;

  /// 检查传入级别是否满足志记要求
  ///  @param[in]		level		日志级别
  ///  @return true满足志记要求 false不满足
  bool ShouldLog(LogLevel level) const;

 protected:
  std::atomic<LogLevel> level_{LogLevel::kLogLevelTrace};  ///< 追加器志记级别
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_H_
