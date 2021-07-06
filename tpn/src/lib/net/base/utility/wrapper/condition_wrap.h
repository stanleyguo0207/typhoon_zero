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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_CONDITION_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_CONDITION_WRAP_H_

#include "rpc_type.pb.h"
#include "byte_converter.h"
#include "net_common.h"

namespace tpn {

namespace net {

/// tcp拆包条件
class TcpMatchCondition {
 public:
  TcpMatchCondition() = default;

  /// asio底层拆包
  template <typename Iterator>
  std::pair<Iterator, bool> operator()(Iterator begin, Iterator end) const {
    if (end - begin < kHeaderBytes) {  // 包头长度最大允许2个字节
      return std::pair(begin, false);
    }

    Iterator iter = begin;
    while (iter != end) {
      if (0 == header_len_) {
        header_len_ = *(reinterpret_cast<const uint16_t *>(iter.operator->()));
        tpn::EndianConvert(header_len_);
        if (0 == header_len_) {
          return std::pair(begin, true);
        }
      }

      iter += kHeaderBytes;
      if (end - iter < header_len_) {  // 包头需要数据长度不满足
        break;
      }

      if (0 == packet_len_) {
        tpn::protocol::Header header;
        if (!header.ParseFromArray(
                reinterpret_cast<const uint8_t *>(iter.operator->()),
                header_len_)) {
          return std::pair(begin, true);
        }

        packet_len_ = header.size();
        if (0 == packet_len_) {
          return std::pair(begin, true);
        }
      }

      iter += header_len_;
      if (end - iter < packet_len_) {  // 包体需要数据长度不满足
        break;
      }

      auto packet_pair = std::pair(iter + packet_len_, true);
      this->Reset();
      return std::move(packet_pair);
    }

    return std::pair(begin, false);
  }

 private:
  // 重置
  void Reset() const {
    header_len_ = 0;
    packet_len_ = 0;
  }

 private:
  mutable uint16_t header_len_{0};  ///< 协议包头长度
  mutable uint32_t packet_len_{0};  ///< 协议包体长度
};

/// udp拆包条件
class KcpMatchCondition {
 public:
  KcpMatchCondition();

  TPN_INLINE decltype(auto) operator()() { return cond_; }

 private:
  asio::detail::transfer_at_least_t cond_{1};  ///< 条件
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_CONDITION_WRAP_H_
