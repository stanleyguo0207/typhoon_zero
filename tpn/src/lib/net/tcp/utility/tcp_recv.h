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

#include <string_view>

#include "byte_buffer.h"
#include "byte_converter.h"
#include "rpc_type.pb.h"
#include "net_common.h"
#include "io_pool.h"

#include "test_service.pb.h"

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType>
class TcpRecv {
 public:
  TcpRecv() : header_length_buffer_(2, ByteBuffer::ResizeFlag()) {}

  ~TcpRecv() = default;

 protected:
  void TcpPostRecv(std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("TcpRecv TcpPostRecv");

    if (!derive.IsStarted()) {
      NET_WARN("TcpRecv TcpPostRecv state {}",
               ToNetStateStr(derive.GetNetState()));
      return;
    }

    try {
      derive.GetStream().async_read_some(
          derive.GetBuffer().prepare(derive.GetBuffer().GetPrepareSize()),
          MakeAllocator(
              derive.GetReadAllocator(),
              [&derive, self_ptr = std::move(this_ptr)](
                  const std::error_code &ec, size_t bytes_recvd) mutable {
                NET_DEBUG(
                    "TcpRecv TcpPostRecv state {} read bytes {} error {} ",
                    ToNetStateStr(derive.GetNetState()), bytes_recvd, ec);
                derive.HandleRecv(ec, bytes_recvd, std::move(self_ptr));
              }));
    } catch (std::system_error &e) {
      NET_WARN("TcpRecv TcpPostRecv state {} error: {}",
               ToNetStateStr(derive.GetNetState()), e.code());
      SetLastError(e);
      derive.DoDisconnect(e.code());
    }
  }

  // template <typename Func>
  // bool PartialProcessPacket(std::shared_ptr<Derived> this_ptr,
  //                           ByteBuffer &out_buffer, Func &&func) {
  //   Derived &derive = CRTP_CAST(this);
  //
  //   auto &input_buffer = derive.GetBuffer().GetBase();
  //   if (out_buffer.GetRemainingSpace() > 0) {
  //     size_t read_data_size =
  //         (std::min)(input_buffer.size(), out_buffer.GetRemainingSpace());
  //     out_buffer.Append(
  //         static_cast<const uint8_t *>(input_buffer.data().data()),
  //         read_data_size);
  //     input_buffer.consume(read_data_size);
  //   }
  //
  //   if (out_buffer.GetRemainingSpace() > 0) {
  //     NET_DEBUG(
  //         "TcpRecv TcpHandleRecv could't receive the whole data this time "
  //         "remain size {}",
  //         out_buffer.GetRemainingSpace());
  //     return false;
  //   }
  //
  //   if (!func(this_ptr)) {
  //     NET_DEBUG("TcpRecv TcpHandleRecv func process error DoDisconnect");
  //     derive.DoDisconnect(asio::error::connection_aborted);
  //     return false;
  //   }
  //
  //   return true;
  // }

  bool HandleReadHeaderLength() {
    Derived &derive = CRTP_CAST(this);
    uint16_t len    = 0;
    derive.header_length_buffer_ >> len;
    EndianRefMakeLittle(len);
    derive.header_buffer_.Resize(len);

    NET_DEBUG("TcpRecv HandleReadHeaderLength length : {}", len);
    return true;
  }

  bool HandleReadHeader() {
    Derived &derive = CRTP_CAST(this);
    protocol::Header header;
    if (!header.ParseFromArray(derive.header_buffer_.GetContents(),
                               derive.header_buffer_.GetSize())) {
      NET_WARN("TcpRecv HandleReadHeader error size : ",
               derive.header_buffer_.GetSize());
      return false;
    }
    derive.packet_buffer_.Resize(header.size());
    return true;
  }

  bool HandleReadPacket() {
    Derived &derive = CRTP_CAST(this);
    protocol::Header header;
    TPN_ASSERT(header.ParseFromArray(derive.header_buffer_.GetContents(),
                                     derive.header_buffer_.GetSize()),
               "TcpRecv HandleReadPacket header error");

    protocol::SearchRequest request;
    NET_DEBUG("TcpRecv HandleReadPacket query {}", request.query());
    return true;
  }

  void TcpHandleRecv(const std::error_code &ec, size_t bytes_recvd,
                     std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("TcpRecv TcpHandleRecv state {} read bytes {} error {}",
              ToNetStateStr(derive.GetNetState()), bytes_recvd, ec);

    SetLastError(ec);

    if (!ec) {
      derive.UpdateAliveTime();

      auto &input_buffer = derive.GetBuffer();
      while (input_buffer.size() > 0) {
        // if (!derive.PartialProcessPacket(
        //         this_ptr, derive.header_length_buffer_,
        //         [&derive](std::shared_ptr<Derived> self_ptr) mutable -> bool {
        //           return derive.HandleReadHeaderLength(self_ptr);
        //         })) {
        //   break;
        // }
        // if (!derive.PartialProcessPacket(
        //         this_ptr, derive.header_buffer_,
        //         [&derive](std::shared_ptr<Derived> self_ptr) mutable -> bool {
        //           return derive.HandleReadHeader(self_ptr);
        //         })) {
        //   break;
        // }
        // if (!derive.PartialProcessPacket(
        //         this_ptr, derive.packet_buffer_,
        //         [&derive](std::shared_ptr<Derived> self_ptr) mutable -> bool {
        //           return derive.HandleReadPacket(self_ptr);
        //         })) {
        //   break;
        // }
        if (header_length_buffer_.GetRemainingSpace() > 0) {
          size_t read_data_size = (std::min)(
              input_buffer.size(), header_length_buffer_.GetRemainingSpace());
          header_length_buffer_.Append(
              static_cast<const uint8_t *>(input_buffer.data().data()),
              read_data_size);
          input_buffer.consume(read_data_size);

          if (header_length_buffer_.GetRemainingSpace() > 0) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv header_length_buffer could't receive "
                "the whole data this time remain size {}",
                header_length_buffer_.GetRemainingSpace());
            break;
          }

          if (!HandleReadHeaderLength()) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv header_length_buffer func process error "
                "DoDisconnect");
            SetLastError(asio::error::operation_aborted);
            derive.DoDisconnect(asio::error::operation_aborted);
            return;
          }
        }

        if (header_buffer_.GetRemainingSpace() > 0) {
          size_t read_data_size = (std::min)(
              input_buffer.size(), header_buffer_.GetRemainingSpace());
          header_buffer_.Append(
              static_cast<const uint8_t *>(input_buffer.data().data()),
              read_data_size);
          input_buffer.consume(read_data_size);

          if (header_buffer_.GetRemainingSpace() > 0) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv header_buffer could't receive "
                "the whole data this time remain size {}",
                header_buffer_.GetRemainingSpace());
            break;
          }

          if (!HandleReadHeader()) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv header_buffer func process error "
                "DoDisconnect");
            SetLastError(asio::error::operation_aborted);
            derive.DoDisconnect(asio::error::operation_aborted);
            return;
          }
        }

        if (packet_buffer_.GetRemainingSpace() > 0) {
          size_t read_data_size = (std::min)(
              input_buffer.size(), header_buffer_.GetRemainingSpace());
          packet_buffer_.Append(
              static_cast<const uint8_t *>(input_buffer.data().data()),
              read_data_size);
          input_buffer.consume(read_data_size);

          if (packet_buffer_.GetRemainingSpace() > 0) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv packet_buffer could't receive "
                "the whole data this time remain size {}",
                packet_buffer_.GetRemainingSpace());
            break;
          }

          if (!HandleReadPacket()) {
            NET_DEBUG(
                "TcpRecv TcpHandleRecv packet_buffer func process error "
                "DoDisconnect");
            SetLastError(asio::error::operation_aborted);
            derive.DoDisconnect(asio::error::operation_aborted);
            return;
          }
        }

        header_length_buffer_.Clear();
        header_buffer_.Clear();
      }

      derive.PostRecv(std::move(this_ptr));
    } else {
      NET_DEBUG("TcpRecv TcpHandleRecv error {} and DoDisconnect", ec);
      derive.DoDisconnect(ec);
    }
  }

 protected:
  ByteBuffer header_length_buffer_;  ///< 数据包头长度缓冲区 固定2个字节
  ByteBuffer header_buffer_;         ///< 数据包头缓冲区
  ByteBuffer packet_buffer_;         ///< 数据包体缓冲区
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_UTILITY_TCP_RECV_H_
