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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_SYNC_FACTORY_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_SYNC_FACTORY_H_

#include "log_hub.h"
#include "logger.h"

namespace tpn {

namespace log {

/// 同步记录器工厂
class SynchronousFactory {
 public:
  /// 生产记录器
  ///  @tparam			Appender					追加器类型
  ///  @tparam			AppenderArgs...		追加器所需参数
  ///  @param[in]		logger_name				记录器名称
  ///  @param[in]		args...						追加器构造所需参数
  template <typename Apppender, typename... AppenderArgs>
  static LoggerSptr Create(std::string_view logger_name,
                           AppenderArgs &&...args) {
    auto appender =
        std::make_shared<Apppender>(std::forward<AppenderArgs>(args)...);
    auto new_logger =
        std::make_shared<Logger>(logger_name, std::move(appender));
    g_log_hub->InitializeLogger(new_logger);
    return new_logger;
  }
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_SYNC_FACTORY_H_