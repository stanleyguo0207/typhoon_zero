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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_THREAD_POOL_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_THREAD_POOL_H_

#include <vector>
#include <memory>
#include <thread>
#include <future>
#include <stdexcept>
#include <functional>

#include "define.h"
#include "utils.h"
#include "mpmc_queue.h"

namespace tpn {

/// 多消费者线程池
class TPN_COMMON_API ThreadPool {
  using task_type = std::function<void()>;

 public:
  /// 构造函数
  ///  @param[in]   concurrency_hint    线程池大小
  explicit ThreadPool(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2);
  ~ThreadPool();

  /// 提交任务
  template <typename Func, typename... Args>
  decltype(auto) Submit(Func &&func, Args &&...args) {
    using return_type = std::invoke_result_t<Func, Args...>;

    auto task = std::make_shared<std::packaged_task<return_type()>>(
        ForwardAsLambda(std::forward<decltype(func)>(func),
                        std::forward<decltype(args)>(args)...));

    std::future<return_type> ret = task->get_future();
    queue_.Enqueue([task]() { (*task)(); });
    return ret;
  }

 private:
  std::vector<std::thread> workers_;  ///<  工作线程
  MPMCQueue<task_type> queue_;        ///<  任务队列

  TPN_NO_COPYABLE(ThreadPool)
  TPN_NO_MOVEABLE(ThreadPool)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_THREAD_POOL_H_
