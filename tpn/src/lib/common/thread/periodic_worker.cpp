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

#include "periodic_worker.h"

namespace tpn {

// template <typename Rep, typename Period>
// PeriodicWorker::PeriodicWorker(const task_type &task,
//                                std::chrono::duration<Rep, Period> interval) {
//   active_ = (interval > MilliSeconds::zero());
//   if (!active_) {
//     return;
//   }
//
//   worker_ = std::thread([this, task, interval] {
//     for (;;) {
//       std::unique_lock<std::mutex> lock(this->mutex_);
//       if (this->cv_.wait_for(lock, interval,
//                              [this] { return !this->active_; })) {
//         return;
//       }
//       task();
//     }
//   });
// }

/// 析构函数
PeriodicWorker::~PeriodicWorker() {
  if (worker_.joinable()) {
    {
      std::unique_lock<std::mutex> lock(this->mutex_);
      active_ = false;
    }
    cv_.notify_one();
    worker_.join();
  }
}

}  // namespace tpn
