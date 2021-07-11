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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPMC_QUEUE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPMC_QUEUE_H_

#include <mutex>
#include <queue>
#include <atomic>
#include <type_traits>
#include <condition_variable>

namespace tpn {

/// 多生产者多消费者队列
///  @tparam  T     元素类型
template <typename T>
class MPMCQueue {
 public:
  /// 构造函数
  MPMCQueue() : shutdown_(false) {}

  /// 获取生产者队列长度
  ///  @return 生产者队列长度
  size_t GetSize() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.size();
  }

  /// 队列是否为空
  ///  @return 为空返回true
  bool IsEmpty() {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.empty();
  }

  /// 入队
  ///  @param[in]   value     元素值
  void Enqueue(const T &value) {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    queue_.push(std::move(value));
    condition_.notify_one();
  }

  /// 出队
  ///  @param[in]   value     队头元素
  ///  @return 如果队空或队列销毁中返回失败
  bool Dequeue(T &value) {
    std::lock_guard<std::mutex> lock(queue_mutex_);

    if (queue_.empty() || shutdown_) {
      return false;
    }

    value = queue_.front();
    queue_.pop();

    return true;
  }

  /// 等待出队
  /// 如果队空并且没有销毁 等待元素入队
  ///  @param[in]   value     队头元素
  void DequeueWait(T &value) {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    while (queue_.empty() && !shutdown_) {
      condition_.wait(lock);
    }

    if (queue_.empty() || shutdown_) {
      return;
    }

    value = queue_.front();
    queue_.pop();
  }

  /// 清空所有队列元素，并且设置队列为销毁状态
  void Cancel() {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    while (!queue_.empty()) {
      T &value = queue_.front();

      // 元素销毁
      DeleteQueuedObject(value);

      queue_.pop();
    }
    shutdown_ = true;
    condition_.notify_all();
  }

 private:
  template <typename E = T>
  std::enable_if_t<std::is_pointer_v<E>> DeleteQueuedObject(E &obj) {
    delete obj;
  }

  template <typename E = T>
  std::enable_if_t<!std::is_pointer_v<E>> DeleteQueuedObject(const E &) {}

 private:
  std::mutex queue_mutex_;             ///<  队列互斥锁
  std::queue<T> queue_;                ///<  队列
  std::condition_variable condition_;  ///<  条件
  std::atomic<bool> shutdown_;         ///<  结束标志
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPMC_QUEUE_H_
