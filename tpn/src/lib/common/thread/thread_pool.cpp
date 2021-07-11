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

#include "thread_pool.h"

namespace tpn {

ThreadPool::ThreadPool(
    size_t concurrency_hint /* = std::thread::hardware_concurrency() * 2 */) {
  for (size_t i = 0; i < concurrency_hint; ++i) {
    workers_.emplace_back([this] {
      for (;;) {
        task_type task;
        queue_.DequeueWait(task);
        if (nullptr == task) {
          return;
        }
        task();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  queue_.Cancel();

  for (auto &worker : workers_) {
    if (worker.joinable()) {
      worker.join();
    }
  }
}

}  // namespace tpn
