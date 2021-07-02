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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_WRAPPER_TCP_SEND_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_WRAPPER_TCP_SEND_WRAP_H_

#include "net_common.h"
#include "byte_converter.h"
#include "byte_buffer.h"

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType>
class TcpSendWrap {
 public:
  TcpSendWrap()  = default;
  ~TcpSendWrap() = default;

 protected:
  TPN_INLINE bool TcpSend(ByteBuffer &&packet) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("TcpSendWrap TcpSend");

    asio::async_write(
        derive.GetStream(),
        asio::buffer(packet.GetContents(), packet.GetSize()),
        std::bind_executor(
            derive.GetIoHandle().GetStrand(),
            MakeAllocator(
                derive.GetWriteAllocator(),
                [&derive, self_ptr = derive.GetSelfSptr()](
                    const std::error_code &ec, size_t bytes_sent) mutable {
                  NET_DEBUG(
                      "TcpSendWrap TcpSend async_write error {} bytes "
                      "{}",
                      ec, bytes_sent);
                  SetLastError(ec);

                  if (ec) {
                    NET_DEBUG("TcpSendWrap TcpSend error {} and DoDisconnect",
                              ec);
                    derive.DoDisconnect(ec);
                  }
                })));
    return true;
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_WRAPPER_TCP_SEND_WRAP_H_
