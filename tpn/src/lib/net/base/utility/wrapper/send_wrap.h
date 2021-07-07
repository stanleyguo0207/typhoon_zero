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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SEND_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SEND_WRAP_H_

#include "traits_hub.h"
#include "message_buffer.h"
#include "net_common.h"
#include "io_pool.h"
#include "event_queue.h"
#include "callback_helper.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

/// 数据发送包裹
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class SendWrap {
  TPN_NET_FRIEND_DECL_BASE_CLASS
 public:
  SendWrap()  = default;
  ~SendWrap() = default;

  /// 发送数据
  ///  @param[in]   buffer    调用需要保证buffer满足底层拆包逻辑
  ///  @return 发送成功返回true
  TPN_INLINE bool Send(MessageBuffer &&buffer) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("SendWrap Send");

    try {
      if (!derive.IsStarted()) {
        NET_WARN("SendWrap derive is not started");
        asio::detail::throw_error(asio::error::not_connected);
      }

      derive.EventEnqueue([&derive, buffer = std::move(buffer)](
                              EventQueueGuard<Derived> &&guard) mutable {
        NET_DEBUG("SendWrap Send DoSend");
        return derive.DoSend(std::move(buffer),
                             [guard = std::move(guard)](const std::error_code &,
                                                        size_t) mutable {});
      });
      return true;
    } catch (std::system_error &e) {
      NET_ERROR("SendWrap Send error {}", e.code());
      SetLastError(e);
    } catch (std::exception &ex) {
      NET_ERROR("SendWrap Send exception {}", ex.what());
      SetLastError(asio::error::eof);
    }
    return false;
  }

  /// 发送数据
  ///  @tparam      Callback  发送数据完回调类型
  ///  @param[in]   buffer    调用需要保证buffer满足底层拆包逻辑
  ///  @param[in]   callback  发送数据回调
  ///  @return 发送成功返回true
  template <typename Callback>
  TPN_INLINE typename std::enable_if_t<is_callable_v<Callback>, bool> Send(
      MessageBuffer &&buffer, Callback &&callback) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("SendWrap Send");

    try {
      if (!derive.IsStarted()) {
        NET_WARN("SendWrap derive is not started");
        asio::detail::throw_error(asio::error::not_connected);
      }

      derive.EventEnqueue([&derive, buffer = std::move(buffer),
                           callback = std::forward<Callback>(callback)](
                              EventQueueGuard<Derived> &&guard) mutable {
        NET_DEBUG("SendWrap Send DoSend");
        return derive.DoSend(
            std::move(buffer),
            [&callback, guard = std::move(guard)](const std::error_code &,
                                                  size_t bytes_sent) mutable {
              CallbackHelper::Call(callback, bytes_sent);
            });
      });
      return true;
    } catch (std::system_error &e) {
      NET_ERROR("SendWrap Send error {}", e.code());
      SetLastError(e);
    } catch (std::exception &ex) {
      NET_ERROR("SendWrap Send exception {}", ex.what());
      SetLastError(asio::error::eof);
    }
    return false;
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SEND_WRAP_H_
