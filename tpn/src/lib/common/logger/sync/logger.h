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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_

#include <vector>
#include <atomic>

#include "log_common.h"

namespace tpn {

namespace log {

/// 同步日志记录器
class TPN_COMMON_API Logger {
 public:
 protected:
  std::string name_{""};                                 ///< 记录器名称
  std::atomic<LogLevel> level_{LogLevel::kLogLevelOff};  ///< 志记级别
  std::atomic<LogLevel> flush_level_{LogLevel::kLogLevelOff};  ///< 刷新级别
  AppenderSptrVec appenders_;                                  ///< 附加器
  ErrHandler err_handler_{nullptr};                            ///< 错误处理
};

/// 交换两个记录器
void Swap(Logger &a, Logger &b);

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_
