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

#include "io_pool.h"

#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <type_traits>

#include "log.h"
#include "chrono_wrap.h"

namespace tpn {

namespace net {

/// io_context 对象池
class IoPool::IoPoolImpl {
 public:
  IoPoolImpl(size_t concurrency_hint) : ios_(concurrency_hint) {
    NET_DEBUG("io pool size {}.", ios_.size());
    this->threads_.reserve(ios_.size());
    this->workers_.reserve(ios_.size());
  }

  ~IoPoolImpl() { this->Stop(); }

  TPN_INLINE bool Start() {
    std::lock_guard<std::mutex> lock(this->mutex_);

    NET_DEBUG("io pool start.");
    if (!this->stopped_) {
      NET_ERROR("io pool stopped");
      return false;
    }

    if (!this->workers_.empty() || !this->threads_.empty()) {
      NET_ERROR("io pool has {} workers and {} threads", this->workers_.size(),
                this->threads_.size());
      return false;
    }

    // 创建对象池 并且 启动所有的io_context对象
    for (auto &io : this->ios_) {
      // 重新启动io_context以准备后续的run()调用
      /// Restart the io_context in preparation for a subsequent run() invocation.
      /**
       * This function must be called prior to any second or later set of
       * invocations of the run(), run_one(), poll() or poll_one() functions when a
       * previous invocation of these functions returned due to the io_context
       * being stopped or running out of work. After a call to restart(), the
       * io_context object's stopped() function will return @c false.
       *
       * This function must not be called while there are any unfinished calls to
       * the run(), run_one(), poll() or poll_one() functions.
       */
      io.GetIoContext().restart();
      this->workers_.emplace_back(io.GetIoContext().get_executor());

      this->threads_.emplace_back([&io]() {
        NET_DEBUG("io context run");
        io.GetIoContext().run();
      });
    }

    this->stopped_ = false;
    return true;
  }

  TPN_INLINE void Stop() {
    {
      std::lock_guard<std::mutex> lock(this->mutex_);
      NET_DEBUG("io pool stop.");

      if (this->stopped_) {
        NET_WARN("io pool stopped");
        return;
      }

      if (this->workers_.empty() && this->threads_.empty()) {
        NET_WARN("io pool has {} workers and {} threads", this->workers_.size(),
                 this->threads_.size());
        return;
      }

      if (IsRunningInIoPoolThreads()) {
        NET_WARN("this thread is running in io pool");
        return;
      }

      this->stopped_ = true;
    }

    // 等待所有嵌套事件完成 要把互斥锁放开 否则造成死锁
    WaitIoThreads();

    {
      std::lock_guard<std::mutex> lock(this->mutex_);

      for (auto &worker : this->workers_) {
        worker.reset();
      }

      for (auto &thread : this->threads_) {
        thread.join();
      }

      this->workers_.clear();
      this->threads_.clear();
    }
  }

  TPN_INLINE bool IsStarted() const { return !this->stopped_; }

  TPN_INLINE bool IsStopped() const { return this->stopped_; }

  TPN_INLINE IoHandle &GetIoHandleByIndex(size_t index) {
    return this->ios_[index < this->ios_.size()
                          ? index
                          : ((++(this->next_)) % this->ios_.size())];
  }

 private:
  /// 检查当前线程是否在对象池中
  ///  @return 线程是否在对象池中
  TPN_INLINE bool IsRunningInIoPoolThreads() {
    auto curr_tid = std::this_thread::get_id();
    for (auto &thread : this->threads_) {
      if (curr_tid == thread.get_id()) {
        return true;
      }
    }

    return false;
  }

  /// 确保asio::post(...)事件被调用
  TPN_INLINE void WaitIoThreads() {
    {
      NET_DEBUG("io pool wait io threads");
      std::lock_guard<std::mutex> lock(this->mutex_);

      if (IsRunningInIoPoolThreads()) {
        NET_WARN("this thread is running in io pool");
        return;
      }

      if (!this->workers_.empty()) {
        this->workers_.front().reset();
      }
    }

    constexpr auto max = MilliSeconds(10);
    constexpr auto min = MilliSeconds(1);

    if (!this->ios_.empty()) {
      auto t1          = SteadyClock::now();
      auto &io_context = this->ios_.front().GetIoContext();
      while (!io_context.stopped()) {
        NET_DEBUG("wait io context to stop");
        auto t2 = SteadyClock::now();
        std::this_thread::sleep_for((
            std::max<SteadyClock::duration>)((std::min<
                                                 SteadyClock::duration>)(t2 -
                                                                             t1,
                                                                         max),
                                             min));
      }
      NET_DEBUG("wait io context to stoped");
    }

    {
      std::lock_guard<std::mutex> lock(this->mutex_);

      for (size_t i = 1; i < this->workers_.size(); ++i) {
        this->workers_[i].reset();
      }
    }

    for (size_t i = 1; i < this->ios_.size(); ++i) {
      auto t1          = SteadyClock::now();
      auto &io_context = this->ios_[i].GetIoContext();
      while (!io_context.stopped()) {
        NET_DEBUG("wait io context to stop ios_ index {}", i);
        auto t2 = SteadyClock::now();
        std::this_thread::sleep_for((
            std::max<SteadyClock::duration>)((std::min<
                                                 SteadyClock::duration>)(t2 -
                                                                             t1,
                                                                         max),
                                             min));
      }
      NET_DEBUG("wait io context to stoped ios_ index {}", i);
    }
  }

 private:
  std::vector<std::thread> threads_;  ///< 运行io_context的线程池
  std::vector<IoHandle> ios_;         ///< io_context对象池
  std::mutex mutex_;
  bool stopped_{true};  ///< io_context对象池是否停止标志
  size_t next_{0};  ///< 下一个可用的io_context对象池ios_中的下标
  std::vector<asio::executor_work_guard<asio::io_context::executor_type>>
      workers_;  ///< 执行io_context的工作线程 他们的run函数不会停止
};

IoPool::IoPool(
    size_t concurrency_hint /* = std::thread::hardware_concurrency() * 2 */) {
  this->io_pool_uptr_ = std::make_unique<IoPoolImpl>(
      0 == concurrency_hint ? std::thread::hardware_concurrency() * 2
                            : concurrency_hint);
}

IoPool::~IoPool() {}

bool IoPool::IoPoolStart() { this->io_pool_uptr_->Start(); }

void IoPool::IoPoolStop() { this->io_pool_uptr_->Stop(); }

bool IoPool::IsIoPoolStarted() const {
  return this->io_pool_uptr_->IsStarted();
}

bool IoPool::IsIoPoolStopped() const {
  return this->io_pool_uptr_->IsStopped();
}

IoHandle &IoPool::GetIoHandleByIndex(
    size_t index /* = static_cast<size>(-1) */) {
  return this->io_pool_uptr_->GetIoHandleByIndex(index);
}

}  // namespace net

}  // namespace tpn
