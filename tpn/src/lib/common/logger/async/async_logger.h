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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_LOGGER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_LOGGER_H_

#include <string_view>

#include "crtp_object.h"
#include "log_msg.h"
#include "logger.h"
#include "periodic_worker.h"

namespace tpn {

namespace log {

/// 异步日志记录器
class TPN_COMMON_API AsyncLogger : public CRTPObject<AsyncLogger>,
                                   public Logger {
 public:
  /// 构造函数 迭代器
  ///  @tparam			Iter		迭代器类型
  ///  @param[in]		name		记录器名称
  ///  @param[in]		begin		追加器容器开始迭代器
  ///  @param[in]		end			追加器容器结束迭代器
  template <typename Iter>
  AsyncLogger(std::string_view name, Iter begin, Iter end,
              ThreadPoolWptr thread_pool)
      : CRTPObject<AsyncLogger>(),
        Logger(name, begin, end),
        thread_pool_(thread_pool) {}

  /// 构造函数 容器
  ///  @param[in]		name				记录器名称
  ///  @param[in]		appenders		追加器初始化列表
  AsyncLogger(std::string_view name, AppenderInitList appenders,
              ThreadPoolWptr thread_pool);

  /// 构造函数 迭代器
  ///  @param[in]		name							记录器名称
  ///  @param[in]		single_appender		单个追加器
  AsyncLogger(std::string_view name, AppenderSptr single_appender,
              ThreadPoolWptr thread_pool);

 protected:
  /// 追加器记录日志
  ///  @param[in]		msg			志记信息
  void AppenderDoLog(const LogMsg &msg) final;

  /// 追加器刷新
  void AppenderDoFlush() final;

  /// 异步记录日志
  void BlackendDoLog(const AsyncLogMsg &async_msg);

  /// 异步刷新
  void BlackendDoFlush();

 private:
  ThreadPoolWptr thread_pool_;  ///< 线程池
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_ASYNC_ASYNC_LOGGER_H_
