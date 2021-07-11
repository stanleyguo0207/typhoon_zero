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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_PERIODIC_WORKER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_PERIODIC_WORKER_H_

#include <mutex>
#include <thread>
#include <functional>
#include <condition_variable>

#include "define.h"
#include "chrono_wrap.h"

namespace tpn {

/// 周期工作
class TPN_COMMON_API PeriodicWorker {
 public:
  using task_type = std::function<void()>;

 public:
  /// 构造函数
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   task      工作任务
  ///  @param[in]   interval  工作间隔
  template <typename Rep, typename Period>
  PeriodicWorker(const task_type &task,
                 std::chrono::duration<Rep, Period> interval) {
    active_ = (interval > MilliSeconds::zero());
    if (!active_) {
      return;
    }

    worker_ = std::thread([this, task, interval] {
      for (;;) {
        std::unique_lock<std::mutex> lock(this->mutex_);
        if (this->cv_.wait_for(lock, interval,
                               [this] { return !this->active_; })) {
          return;
        }
        task();
      }
    });
  }

  /// 析构函数
  ~PeriodicWorker();

 private:
  bool active_{false};          ///<  可用标志
  std::thread worker_;          ///<  工作线程
  std::mutex mutex_;            ///< 任务锁
  std::condition_variable cv_;  ///< 生产者条件
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_THREAD_PERIODIC_WORKER_H_
