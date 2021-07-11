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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_FACTORY_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_FACTORY_H_

#include "thread_pool.h"
#include "log_hub.h"
#include "async_logger.h"

namespace tpn {

namespace log {

/// 异步记录器工厂
class AsynchronousFactory {
 public:
  /// 生产记录器
  ///  @tparam			Appender					追加器类型
  ///  @tparam			AppenderArgs...		追加器所需参数
  ///  @param[in]		logger_name				记录器名称
  ///  @param[in]		args...						追加器构造所需参数
  template <typename Apppender, typename... AppenderArgs>
  static LoggerSptr Create(std::string_view logger_name,
                           AppenderArgs &&...args) {
    auto &mutex = g_log_hub->GetThreadPoolMutex();
    std::lock_guard<std::recursive_mutex> lock(mutex);
    auto tp = g_log_hub->GetThreadPool();
    if (nullptr == tp) {
      tp = std::make_shared<ThreadPool>();
      g_log_hub->SetThreadPool(tp);
    }

    auto appender =
        std::make_shared<Apppender>(std::forward<AppenderArgs>(args)...);
    auto new_logger = std::make_shared<AsyncLogger>(
        logger_name, std::move(appender), std::move(tp));
    g_log_hub->InitializeLogger(new_logger);
    return new_logger;
  }
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_FACTORY_H_
