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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_RECV_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_RECV_H_

#include "byte_converter.h"
#include "message_buffer.h"
#include "net_common.h"
#include "io_pool.h"
#include "condition_wrap.h"
#include "buffer_wrap.h"

namespace tpn {

namespace net {

/// tcp接收消息
/// 只支持固定格式的拆包方式不接收其他形式的包
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType>
class TcpRecv {
 public:
  /// 构造函数
  TcpRecv() : match_condition_() {}
  ~TcpRecv() = default;

 protected:
  /// 获取tcp拆包组件
  TyphoonKcpMatchCondition &GetMatchCondition() {
    return this->match_condition_;
  }

  /// tcp提交接收数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  void TcpPostRecv(std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    if (!derive.IsStarted()) {
      NET_WARN("TcpRecv TcpPostRecv state {}",
               ToNetStateStr(derive.GetNetState()));
      return;
    }

    try {
      asio::async_read_until(
          derive.GetStream(), derive.GetBuffer().GetBase(),
          derive.GetMatchCondition(),
          asio::bind_executor(
              derive.GetIoHandle().GetStrand(),
              MakeAllocator(
                  derive.GetReadAllocator(),
                  [&derive, self_ptr = std::move(this_ptr)](
                      const std::error_code &ec, size_t bytes_recvd) mutable {
                    NET_DEBUG(
                        "TcpRecv TcpPostRecv state {} read bytes {} error {} ",
                        ToNetStateStr(derive.GetNetState()), bytes_recvd, ec);
                    derive.HandleRecv(ec, bytes_recvd, std::move(self_ptr));
                  })));
    } catch (std::system_error &e) {
      NET_WARN("TcpRecv TcpPostRecv state {} error: {}",
               ToNetStateStr(derive.GetNetState()), e.code());
      SetLastError(e);
      derive.DoDisconnect(e.code());
    }
  }

  /// tcp处理接收到的数据
  ///  @param[in]   ec          错误码
  ///  @param[in]   bytes_recvd 收到的数据长度，因为在match的时候做了处理，所以需要注意
  ///                           bytes_recvd == 0 错误需要断开
  ///                           bytes_recvd  > 0 收到的整个包数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  void TcpHandleRecv(const std::error_code &ec, size_t bytes_recvd,
                     std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("TcpRecv TcpHandleRecv state {} read bytes {} error {}",
              ToNetStateStr(derive.GetNetState()), bytes_recvd, ec);

    SetLastError(ec);

    if (0 == bytes_recvd) {  // tcp拆包错误
      TPN_ERROR("TcpRecv TcpHandleRecv bytes_recvd error");
      derive.Stop();
      return;
    }

    if (!ec) {
      // 更新收到包的时间
      derive.UpdateAliveTime();

      const uint8_t *buffer =
          static_cast<const uint8_t *>(derive.GetBuffer().data().data());

      uint16_t header_length = *(reinterpret_cast<const uint16_t *>(buffer));
      tpn::EndianRefMakeLittle(header_length);
      if ((0 == header_length) || (header_length + kHeaderBytes >= bytes_recvd))
          [[unlikely]] {
        TPN_ERROR("TcpRecv TcpHandleRecv header_length {} error",
                  header_length);
        derive.Stop();
        return;
      }

      protocol::Header header;
      if (!header.ParseFromArray(buffer + kHeaderBytes, header_length))
          [[unlikely]] {
        TPN_ERROR("TcpRecv TcpHandleRecv header parse header_length {} error",
                  header_length);
        derive.Stop();
        return;
      }

      if ((0 == header.size()) ||
          (header.size() + header_length + kHeaderBytes > bytes_recvd))
          [[unlikely]] {
        TPN_ERROR(
            "TcpRecv TcpHandleRecv header_length {} packet_length {} error",
            header_length, header.size());
        derive.Stop();
        return;
      }

      MessageBuffer packet(header.size());  // 消息包
      packet.Write(buffer + kHeaderBytes + header_length, header.size());

      // 通知会话拆包后的数据
      derive.FireRecv(this_ptr, std::move(header), std::move(packet));

      // asio缓冲区中将数据移除
      derive.GetBuffer().consume(bytes_recvd);

      // 监听新的接收
      derive.PostRecv(std::move(this_ptr));
    } else {
      // 如果发生错误，则不会启动新的异步操作。
      // 这意味着对连接对象的所有 shared_ptr 引用都将消失
      // 并且该对象将在此处理程序返回后自动销毁。
      // 连接类的析构函数关闭套接字。
      derive.DoDisconnect(ec);
    }
  }

 protected:
  TcpMatchCondition match_condition_;  ///< tcp拆包组件
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_RECV_H_
