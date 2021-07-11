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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPSC_QUEUE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPSC_QUEUE_H_

#include <atomic>
#include <utility>

#include "define.h"

namespace tpn {

/// 多生产者单消费者无锁队列
/// C++ implementation of Dmitry Vyukov's lock free MPSC queue
/// http://www.1024cores.net/home/lock-free-algorithms/queues/non-intrusive-mpsc-node-based-queue
/// 关于内存操作顺序见 https://zh.cppreference.com/w/cpp/atomic/memory_order
template <typename T>
class MPSCQueue {
 public:
  /// 构造函数
  MPSCQueue()
      : head_(new Node()), tail_(head_.load(std::memory_order_relaxed)) {
    Node *front = head_.load(std::memory_order_relaxed);
    front->next.store(nullptr, std::memory_order_relaxed);
  }

  /// 析构函数
  ~MPSCQueue() {
    T *tmp_value;
    while (this->Dequeue(tmp_value)) {
      ;
    }
    Node *front = head_.load(std::memory_order_relaxed);
    delete front;
  }

  /// 入队
  ///  @param[in]   value     元素指针
  void Enqueue(T *value) {
    Node *node      = new Node(value);
    Node *prev_head = head_.exchage(node, std::memory_order_relaxed);
    prev_head->next.store(node, std::memory_order_relaxed);
  }

  /// 出队
  ///  @param[in]   value     元素指针
  ///  @return 队空返回失败
  bool Dequeue(T *&value) {
    Node *tail = tail_.load(std::memory_order_relaxed);
    Node *next = tail->next.load(std::memory_order_acquire);
    if (!next) {
      return false;
    }
    value = next->data;
    tail_.store(next, std::memory_order_release);
    delete tail;
    return true;
  }

 private:
  /// 内部节点结构
  struct Node {
    Node() = default;
    explicit Node(T *data_in) : data(data_in) {
      next.store(nullptr, std::memory_order_relaxed);
    }

    T *data;                   ///<  节点数据
    std::atomic<Node *> next;  ///<  节点下一跳指针
  };

  std::atomic<Node *> head_;  ///<  链表头
  std::atomic<Node *> tail_;  ///<  链表尾

  TPN_NO_COPYABLE(MPSCQueue)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_ALGORITHM_MPSC_QUEUE_H_
