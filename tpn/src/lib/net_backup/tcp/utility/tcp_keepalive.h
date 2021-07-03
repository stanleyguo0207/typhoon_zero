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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_KEEPALIVE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_KEEPALIVE_H_

#include "net_common.h"

#if (TPN_PLATFORM_WIN == TPN_PLATFORM)
#  include <mstcpip.h>
#endif

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType>
class TcpKeepAlive {
 public:
  TcpKeepAlive(typename ArgsType::socket_type &socket) : socket_ref_(socket) {}
  ~TcpKeepAlive() = default;

  bool SetKeepAliveOptions(bool enable = true, unsigned int idle = 60,
                           unsigned int interval = 3, unsigned int count = 3) {
    NET_DEBUG(
        "TcpKeepAlive SetKeepAliveOptions enable:{} idle:{} interval:{} "
        "count:{}",
        enable, idle, interval, count);
    try {
      std::ignore = count;

      auto &socket = this->socket_ref_.lowest_layer();
      if (!socket.is_open()) {
        NET_ERROR("TcpKeepAlive SetKeepAliveOptions socket is not open");
        SetLastError(asio::error::not_connected);
        return false;
      }

      asio::socket_base::keep_alive option(enable);
      socket.set_option(option);

      auto native_fd = socket.native_handle();

#if (TPN_PLATFORM_WIN == TPN_PLATFORM)
      tcp_keepalive keepalive_options;
      keepalive_options.onoff             = enable;
      keepalive_options.keepalivetime     = idle * 1000;
      keepalive_options.keepaliveinterval = interval * 1000;

      DWORD bytes_returned = 0;
      if (SOCKET_ERROR ==
          ::WSAIoctl(native_fd, SIO_KEEPALIVE_VALS, (LPVOID)&keepalive_options,
                     (DWORD)sizeof(keepalive_options), nullptr, 0,
                     (LPDWORD)&bytes_returned, nullptr, nullptr)) {
        if (WSAEWOULDBLOCK != ::WSAGetLastError()) {
          NET_ERROR("TcpKeepAlive SetKeepAliveOptions WSAIoctl error {}",
                    ::WSAGetLastError());
          SetLastError(::WSAGetLastError());
          return false;
        }
      }

#elif (TPN_PLATFORM_UNIX == TPN_PLATFORM)
      int ret_keepidle  = setsockopt(native_fd, SOL_TCP, TCP_KEEPIDLE,
                                    (void *)&idle, sizeof(unsigned int));
      int ret_keepintvl = setsockopt(native_fd, SOL_TCP, TCP_KEEPINTVL,
                                     (void *)&interval, sizeof(unsigned int));
      int ret_keepinit  = setsockopt(native_fd, SOL_TCP, TCP_KEEPCNT,
                                    (void *)&count, sizeof(unsigned int));

      if (ret_keepidle || ret_keepintvl || ret_keepinit) {
        SetLastError(errno);
        return false;
      }
#endif
      return true;

    } catch (std::system_error &e) {
      NET_ERROR("TcpKeepAlive SetKeepAliveOptions error {}", e.code());
      SetLastError(e);
    }
    return false;
  }

 protected:
  typename ArgsType::socket_type &socket_ref_;  ///<  套接字引用
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_KEEPALIVE_H_
