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

#include <memory>
#include <functional>
#include <string>
#include <string_view>

#include "net_common.h"
#include "io_pool.h"
#include "event_queue.h"
#include "byte_buffer.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

template <typename Derived, typename ArgsType>
class SendWrap {
  TPN_NET_FRIEND_DECL_BASE_CLASS

 public:
  SendWrap()  = default;
  ~SendWrap() = default;

 protected:
  TPN_INLINE bool Send(ByteBuffer &&packet) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("SendWrap Send");

    try {
      if (!derive.IsStarted()) {
        NET_ERROR("SendWrap is not started");
        asio::detail::throw_error(asio::error::not_connected);
      }

      derive.EventQueue([&derive, packet = std::move(packet))](
                            EventQueueGuard<Derived> &&guard) mutable {
        NET_DEBUG("SendWrap Send DoSend");
        return derive.DoSend(std::move(packet));
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
