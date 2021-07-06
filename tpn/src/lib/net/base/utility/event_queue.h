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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_EVENT_QUEUE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_EVENT_QUEUE_H_

#include <memory>
#include <functional>
#include <queue>

#include "debug_hub.h"
#include "net_common.h"
#include "io_pool.h"

namespace tpn {

namespace net {

TEMPLATE_DECL_2 class EventQueue;

/// 事件队列守护
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class EventQueueGuard {
  TEMPLATE_DECL_2 friend class EventQueue;

 public:
  TPN_INLINE EventQueueGuard(const EventQueueGuard &other)
      : derive(other.derive),
        derive_sptr_(std::move(other.derive_sptr_)),
        valid_(other.valid_) {
    // 移动后的事件无效
    const_cast<EventQueueGuard &>(other).valid_ = !other.valid_;
  }

  TPN_INLINE EventQueueGuard(EventQueueGuard &&other)
      : derive(other.derive),
        derive_sptr_(std::move(other.derive_sptr_)),
        valid_(other.valid_) {
    // 移动后的事件无效
    other.valid_ = !other.valid_;
  }

  TPN_INLINE void operator=(const EventQueueGuard &other) = delete;
  TPN_INLINE void operator=(EventQueueGuard &&other) = delete;

  /// 处理下一个事件
  ~EventQueueGuard() {
    if (this->valid_) {
      derive.NextEvent();
    }
  }

 protected:
  /// 构造函数
  ///  @param[in]   derive    CRTP 对象
  ///  @param[in]   valid     守护是否有效
  EventQueueGuard(Derived &derive, bool valid = true)
      : derive(derive), derive_sptr_(derive.GetSelfSptr()), valid_(valid) {}

 protected:
  Derived &derive;                                 ///<  CRTP对象
  std::shared_ptr<Derived> derive_sptr_{nullptr};  ///<  防止失效的智能指针
  bool valid_{false};                              ///<  是否有效
};

/// 事件队列
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class EventQueue {
 public:
  EventQueue()  = default;
  ~EventQueue() = default;

  /// 事件入队
  ///  @tparam      Callback    事件函数类型
  ///  @param[in]   callback    事件回调
  template <typename Callback>
  TPN_INLINE Derived &EventEnqueue(Callback &&callback) {
    Derived &derive = CRTP_CAST(this);

    // 必须确保在strand上运行，否则不能保证串行
    if (derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      bool empty = this->events_.empty();
      this->events_.emplace(std::forward<Callback>(callback));
      if (empty) {
        (this->events_.front())(EventQueueGuard<Derived>{derive});
      }
      return derive;
    }

    // 非strand 提交到对应的strand上运行
    derive.Post([this, &derive, self_ptr = derive.GetSelfSptr(),
                 callback = std::forward<Callback>(callback)]() mutable {
      bool empty = this->events_.empty();
      this->events_.emplace(std::move(callback));
      if (empty) {
        (this->events_.front())(EventQueueGuard<Derived>{derive});
      }
    });

    return (derive);
  }

 protected:
  /// 事件链上处理下一个事件
  template <typename = void>
  TPN_INLINE Derived &NextEvent() {
    Derived &derive = CRTP_CAST(this);

    // 必须确保在strand上运行，否则不能保证串行
    if (derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      TPN_ASSERT(!this->events_.empty(), "EventQueue events empty");
      if (!this->events_.empty()) {
        this->events_.pop();
        if (!this->events_.empty()) {
          (this->events_.front())(EventQueueGuard<Derived>{derive});
        }
      }
      return derive;
    }

    // 非strand 提交到对应的strand上运行
    derive.Post([this, &derive, self_ptr = derive.GetSelfSptr()]() mutable {
      TPN_ASSERT(!this->events_.empty(), "EventQueue events empty");
      if (!this->events_.empty()) {
        this->events_.pop();
        if (!this->events_.empty()) {
          (this->events_.front())(EventQueueGuard<Derived>{derive});
        }
      }
    });

    return (derive);
  }

 protected:
  std::queue<std::function<bool(EventQueueGuard<Derived> &&)>>
      events_;  ///<  事件
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_EVENT_QUEUE_H_
