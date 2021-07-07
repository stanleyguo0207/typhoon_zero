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

#include "message_buffer.h"
#include "net_common.h"

namespace tpn {

namespace net {

/// tcp接收消息
/// 只支持固定格式的拆包方式不接收其他形式的包
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class TcpSendWrap {
 public:
  TcpSendWrap()  = default;
  ~TcpSendWrap() = default;

 protected:
  /// 发送数据
  ///  @tparam      Callback  发送数据完回调类型
  ///  @param[in]   buffer    调用需要保证buffer满足底层拆包逻辑
  ///  @param[in]   callback  发送数据回调
  ///  @return 发送成功返回true
  template <typename Callback>
  TPN_INLINE bool TcpSend(MessageBuffer &&buffer, Callback &&callback) {
    Derived &derive = CRTP_CAST(this);

    asio::async_write(
        derive.GetStream(),
        asio::buffer(buffer.GetBasePointer(), buffer.GetBufferSize()),
        asio::bind_executor(
            derive.GetIoHandle().GetStrand(),
            MakeAllocator(
                derive.GetWriteAllocator(),
                [&derive, self_ptr = derive.GetSelfSptr(),
                 callback = std::forward<Callback>(callback)](
                    const std::error_code &ec, size_t bytes_sent) mutable {
                  NET_DEBUG(
                      "TcpSendWrap TcpSend async_write error {} bytes "
                      "{}",
                      ec, bytes_sent);
                  SetLastError(ec);

                  callback(ec, bytes_sent);

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
