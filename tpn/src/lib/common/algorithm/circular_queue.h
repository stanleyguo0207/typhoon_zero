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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_CIRCULAR_QUEUE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_CIRCULAR_QUEUE_H_

#include <vector>

#include "define.h"
#include "debug_hub.h"

namespace tpn {

/// 循环队列
///  @tparam	T			元素类型
template <typename T>
class CircularQueue {
  using value_type   = T;
  using storage_type = std::vector<T>;
  using size_type    = typename storage_type::size_type;

 public:
  /// 默认构造函数
  /// 虽然提供了默认构造函数，但是默认构造函数是不合法的
  CircularQueue() = default;

  /// 构造函数
  ///  @param[in]		capacity		最大容量
  explicit CircularQueue(size_type capacity)
      : capacity_(capacity + 1), storage_(capacity_) {}

  /// 移动构造函数
  ///  @param[out]	other			其他循环队列
  CircularQueue(CircularQueue &&other) noexcept {
    CopyMoveable(std::move(other));
  }

  /// 移动拷贝函数
  ///  @param[out]	other			其他循环队列
  ///  @return 移动完的循环队列
  CircularQueue &operator=(CircularQueue &&other) noexcept {
    if (this != &other) {
      CopyMoveable(std::move(other));
    }
    return *this;
  }

  /// 入队
  ///  @param[in]		item		元素
  void EnQueue(T &&item) {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    storage_[tail_] = std::move(item);
    tail_           = (tail_ + 1) % capacity_;

    if (head_ == tail_) {
      head_ = (head_ + 1) % capacity_;
      ++overrun_counter_;
    }
  }

  /// 出队
  void DeQueue() {
    TPN_ASSERT(!IsEmpty(), "CircularQueue empty");
    head_ = (head_ + 1) % capacity_;
  }

  /// 获取循环队列长度
  ///  @return 循环队列当前长度
  const size_type GetSize() const {
    if (tail_ > head_) {
      return tail_ - head_;

    } else {
      return capacity_ - (head_ - tail_);
    }
  }

  /// 获取指定位置元素 只读
  ///  @param[in]		index			指定位置
  ///  @return 指定位置的元素
  const T &At(size_type index) const {
    TPN_ASSERT(index < GetSize(), "Index out of range");
    return storage_[(head_ + index) % capacity_];
  }

  /// 获取指定位置元素 只读
  ///  @param[in]		index			指定位置
  ///  @return 指定位置的元素
  const T &operator[](size_type index) const {
    TPN_ASSERT(index < GetSize(), "Index out of range");
    return storage_[(head_ + index) % capacity_];
  }

  /// 队列是否为空
  ///  @return 为空返回true
  bool IsEmpty() const { return tail_ == head_; }

  /// 队列是否为满
  ///  @return 为满返回true
  bool IsFull() const {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    return ((tail_ + 1) % capacity_) == head_;
  }

  /// 获取队头元素
  ///  @return 队头元素
  T &GetFront() {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    return storage_[head_];
  }

  /// 获取队头元素
  ///  @return 队头元素
  const T &GetFront() const {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    return storage_[head_];
  }

  /// 获取队尾元素
  ///  @return 队尾元素
  T &GetRear() {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    return storage_[(tail_ + (capacity_ - 1)) % capacity_];
  }

  /// 获取队尾元素
  ///  @return 队尾元素
  const T &GetRead() const {
    TPN_ASSERT(capacity_ > 0, "You should use capacity > 0");
    return storage_[(tail_ + (capacity_ - 1)) % capacity_];
  }

  /// 获取超限数量
  ///  @return 超限数量
  size_type GetOverrunCount() const { return overrun_counter_; }

 private:
  /// 移动拷贝
  ///  @param[out]	other			要移动的队列
  void CopyMoveable(CircularQueue &&other) noexcept {
    capacity_        = other.capacity_;
    head_            = other.head_;
    tail_            = other.tail_;
    overrun_counter_ = other.overrun_counter_;
    storage_         = std::move(other.storage_);

    other.capacity_        = 0;
    other.head_            = 0;
    other.tail_            = 0;
    other.overrun_counter_ = 0;
  }

 private:
  size_type capacity_{0};         ///< 容量
  size_type head_{0};             ///< 队头
  size_type tail_{0};             ///< 队尾
  size_type overrun_counter_{0};  ///< 超限计数
  storage_type storage_;          ///< 容器

  TPN_DEFAULT_COPY(CircularQueue)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_CIRCULAR_QUEUE_H_
