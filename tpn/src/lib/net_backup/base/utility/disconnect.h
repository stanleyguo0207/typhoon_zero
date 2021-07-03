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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_DISCONNECT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_DISCONNECT_H_

#include "net_common.h"
#include "defer_wrap.h"
#include "event_queue.h"

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType>
class Disconnect {
 public:
  using Self = Disconnect<Derived, ArgsType>;

  Disconnect()  = default;
  ~Disconnect() = default;

 protected:
  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> CheckReconnect(
      const std::error_code &ec, std::shared_ptr<DeferWrap> &defer_task) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("CheckReconnect client error {}", ec);

    if (defer_task) {
      return;
    }

    // 提高自动连接的可靠性
    defer_task = std::make_shared<DeferWrap>(
        [&derive, ec, this_ptr = derive.GetSelfSptr()]() mutable {
          auto task = [&derive, ec, this_ptr = std::move(this_ptr)](
                          EventQueueGuard<Derived> &&guard) mutable {
            IgnoreUnused(ec, this_ptr, guard);

            derive.WakeReconnectTimer();
          };

          // 进入事件队列，确保所有事件完成之前 重新连接不会被执行
          derive.EventEnqueue([&derive, t = std::move(task)](
                                  EventQueueGuard<Derived> &&guard) mutable {
            auto task = [guard = std::move(guard), t = std::move(t)]() mutable {
              t(std::move(guard));
            };
            derive.Post(std::move(task));
            return true;
          });
        });
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> DoDisconnect(
      const std::error_code &ec, std::shared_ptr<DeferWrap> defer_task = {}) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("DoDisconnect client error {}", ec);

    NetState expected = NetState::kNetStateStarted;
    if (derive.GetNetState().compare_exchange_strong(
            expected, NetState::kNetStateStopping)) {
      NET_DEBUG(
          "DoDisconnect client stopping state {} CheckReconnect and "
          "PostDisconnect",
          ToNetStateStr(derive.GetNetState()));
      derive.CheckReconnect(ec, defer_task);

      return derive.PostDisconnect(ec, derive.GetSelfSptr(), expected,
                                   std::move(defer_task));
    }

    expected = NetState::kNetStateStarting;
    if (derive.GetNetState().compare_exchange_strong(
            expected, NetState::kNetStateStopping)) {
      NET_DEBUG(
          "DoDisconnect client stopping state {} CheckReconnect and "
          "PostDisconnect",
          ToNetStateStr(derive.GetNetState()));
      derive.CheckReconnect(ec, defer_task);

      return derive.PostDisconnect(ec, derive.GetSelfSptr(), expected,
                                   std::move(defer_task));
    }
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<IsSession, void> DoDisconnect(
      const std::error_code &ec, std::shared_ptr<DeferWrap> defer_task = {}) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("DoDisconnect session error {} key {}", ec, derive.GetHashKey());

    NetState expected = NetState::kNetStateStarted;
    if (derive.GetNetState().compare_exchange_strong(
            expected, NetState::kNetStateStopping)) {
      NET_DEBUG(
          "DoDisconnect session stopping state {} CheckReconnect and "
          "PostDisconnect key {}",
          ToNetStateStr(derive.GetNetState()), derive.GetHashKey());
      return derive.PostDisconnect(ec, derive.GetSelfSptr(), expected,
                                   std::move(defer_task));
    }
    expected = NetState::kNetStateStarting;
    if (derive.GetNetState().compare_exchange_strong(
            expected, NetState::kNetStateStopping)) {
      NET_DEBUG(
          "DoDisconnect session stopping state {} CheckReconnect and "
          "PostDisconnect key {}",
          ToNetStateStr(derive.GetNetState()), derive.GetHashKey());
      return derive.PostDisconnect(ec, derive.GetSelfSptr(), expected,
                                   std::move(defer_task));
    }
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> PostDisconnect(
      const std::error_code &ec, std::shared_ptr<Derived> this_ptr,
      NetState old_state, std::shared_ptr<DeferWrap> defer_task = {}) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("DoDisconnect client error {} old state {}", ec,
              ToNetStateStr(old_state));

    auto task = [&derive, ec, this_ptr = std::move(this_ptr), old_state,
                 defer_task = std::move(defer_task)](
                    EventQueueGuard<Derived> &&guard) mutable {
      SetLastError(ec);

      NetState expected = NetState::kNetStateStopping;
      if (derive.GetNetState().compare_exchange_strong(
              expected, NetState::kNetStateStopped)) {
        NET_DEBUG("DoDisconnect client task old state {} state {}",
                  ToNetStateStr(old_state),
                  ToNetStateStr(derive.GetNetState()));
        if (old_state == NetState::kNetStateStarted) {
          // 如果已经启动 通知断开连接处理
          // derive.FireDisconnect(this_ptr, ec);
        }

        // 处理断开连接
        derive.HandleDisconnect(ec, std::move(this_ptr));
      } else {
        TPN_ASSERT(false, "DoDisconnect client NetState error");
      }

      //! 这里不能调用 derive.DoStop()，这将导致服务器关闭时自动重启连接无效，
      //! 因此我们使用 defer_task来确定是否应该调用 derive.DoStop()
    };

    NET_DEBUG("PostDisconnect client event enqueue");

    // 进入下面的链执行后，后续所有的事件将会被取消
    derive.EventEnqueue([&derive, t = std::move(task)](
                            EventQueueGuard<Derived> &&guard) mutable {
      auto task = [guard = std::move(guard), t = std::move(t)]() mutable {
        t(std::move(guard));
      };

      NET_DEBUG("PostDisconnect client event post task");
      derive.Post(std::move(task));
      return true;
    });
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<IsSession, void> PostDisconnect(
      const std::error_code &ec, std::shared_ptr<Derived> this_ptr,
      NetState old_state, std::shared_ptr<DeferWrap> defer_task = {}) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("DoDisconnect session {} error {} old state {}",
              derive.GetHashKey(), ec, ToNetStateStr(old_state));

    // 通过异步事件来关闭套接字
    // 因为不允许在多线程中操作同一个套接字，如果在一个线程中关闭套接字，而在
    // 另一个线程中正在调用套接字的async_...函数，它将崩溃。因此，我们必须要
    // 注意操作套接字。当需要关闭套接字的时候，使用子链发布事件，确保套接字在
    // 关闭操作是同一个线程中。

    // 1. 首先确保所有的send和recv事件没有被再次执行
    auto task = [&derive, ec, this_ptr = std::move(this_ptr), old_state,
                 defer_task = std::move(defer_task)](
                    EventQueueGuard<Derived> &&guard) mutable {
      // 运行到这里，所有等待中的发送事件都将被取消

      NET_DEBUG("DoDisconnect session {} task old state {} state {}",
                derive.GetHashKey(), ToNetStateStr(old_state),
                ToNetStateStr(derive.GetNetState()));

      // 2. 其次确保当前的会话从会话管理器中移除
      derive.session_mgr_.Erase(
          this_ptr, [&derive, ec, this_ptr, guard = std::move(guard), old_state,
                     defer_task = std::move(defer_task)](bool) mutable {
            NET_DEBUG("DoDisconnect Erase session {} old state {} state {}",
                      derive.GetHashKey(), ToNetStateStr(old_state),
                      ToNetStateStr(derive.GetNetState()));

            SetLastError(ec);

            NetState expected = NetState::kNetStateStopping;

            if (derive.GetNetState().compare_exchange_strong(
                    expected, NetState::kNetStateStopped)) {
              if (NetState::kNetStateStarted == old_state) {
                // 如果已经启动 通知关闭事件
                // derive.FireDisconnect(
                //     const_cast<std::shared_ptr<Derived> &>(this_ptr));
              } else {
                TPN_ASSERT(false, "DoDisconnect session NetState error");
              }

              // 3. 我们可以停止会话并且关闭套接字
              derive.Post([&derive, ec, self_ptr = std::move(this_ptr),
                           guard      = std::move(guard),
                           defer_task = std::move(defer_task)]() mutable {
                NET_DEBUG("DoDisconnect session HandleDisconnect key {}",
                          derive.GetHashKey());
                // 调用CRTP多态Stop
                derive.HandleDisconnect(ec, std::move(self_ptr));
              });
            }
          });
    };

    NET_DEBUG("PostDisconnect session event enqueue");

    derive.EventEnqueue([&derive, t = std::move(task)](
                            EventQueueGuard<Derived> &&guard) mutable {
      auto task = [guard = std::move(guard), t = std::move(t)]() mutable {
        t(std::move(guard));
      };

      NET_DEBUG("PostDisconnect session event post task");

      // 我们必须使用asio::post来执行任务，否则：当服务器接受器与此会话线程相同时，
      // 当服务器停止时，将调用session_mgr_.ApplyAll -> session_sptr->Stop()
      // -> GetDerivedObj().EventEnqueue -> session_mgr_.Erase => 这将导致死锁
      derive.Post(std::move(task));
      return true;
    });
  }

  TPN_INLINE void HandleDisconnect(const std::error_code &ec,
                                   std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("HandleDisconnect error {}", ec);
    IgnoreUnused(ec, this_ptr);
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_DISCONNECT_H_
