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

#include "net_common.h"
#include "io_pool.h"
#include "debug_hub.h"

namespace tpn {

namespace net {

TEMPLATE_DECL_2 class EventQueue;

template <typename Derived, typename ArgsType = void>
class EventQueueGuard {
  TEMPLATE_DECL_2 friend class EventQueue;

 public:
  TPN_INLINE EventQueueGuard(const EventQueueGuard &other)
      : derive(other.derive),
        derive_sptr_(std::move(other.derive_sptr_)),
        valid_(other.valid_) {
    const_cast<EventQueueGuard &>(other).valid_ = !other.valid_;
  }

  TPN_INLINE EventQueueGuard(EventQueueGuard &&other)
      : derive(other.derive),
        derive_sptr_(std::move(other.derive_sptr_)),
        valid_(other.valid_) {
    other.valid_ = !other.valid_;
  }

  void operator=(const EventQueueGuard &other) = delete;
  void operator=(EventQueueGuard &&other) = delete;

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

template <typename Derived, typename ArgsT = void>
class EventQueue {
 public:
  EventQueue()  = default;
  ~EventQueue() = default;

  template <typename Callback>
  TPN_INLINE Derived &EventEnqueue(Callback &&callback) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("EventQueue enqueue {}", typeid(callback).name());

    if (derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      bool empty = this->events_.empty();
      this->events_.emplace(std::forward<Callback>(callback));
      if (empty) {
        (this->events_.front())(EventQueueGuard<Derived>{derive});
      }
      return derive;
    }

    derive.Post([this, &derive, self = derive.GetSelfSptr(),
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
  template <typename = void>
  inline Derived &NextEvent() {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("EventQueue next event");

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

    derive.Post([this, &derive, self = derive.GetSelfSptr()]() mutable {
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
