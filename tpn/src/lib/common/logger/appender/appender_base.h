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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_BASE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_BASE_H_

#include <mutex>

#include "appender.h"

namespace tpn {

namespace log {

/// 附加器基类
class TPN_COMMON_API AppenderBase : public Appender {
 public:
  AppenderBase() = default;

  /// 记录日志接口
  ///  @param[in]		msg			日志信息
  virtual void Log(const LogMsg &msg) final;

  /// 日志刷新接口
  virtual void Flush() final;

 protected:
  /// 记录日志实现接口
  ///  @param[in]		msg			日志信息
  virtual void DoLog(const LogMsg &msg) = 0;

  /// 日志刷新实现接口
  virtual void DoFlush() = 0;

 private:
  std::mutex mutex_;  ///< 操作锁

  TPN_NO_COPYABLE(AppenderBase)
  TPN_NO_MOVEABLE(AppenderBase)
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_APPENDER_BASE_H_
