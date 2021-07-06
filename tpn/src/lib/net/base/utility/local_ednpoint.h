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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_LOCAL_EDNPOINT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_LOCAL_EDNPOINT_H_

#include "net_common.h"

namespace tpn {

namespace net {

/// 本地端点
///  @tparam  Derived     CRTP对象
///  @tparam  ArgsType    子类模板参数 需要提供 socket_type::lowest_layer_type::endpoint_type
template <typename Derived, typename ArgsType>
class LocalEndpoint {
 public:
  using endpoint_type = typename ArgsType::socket_type::lowest_layer_type::
      endpoint_type;  ///< 端点类型

  LocalEndpoint() : local_endpoint_(){};
  ~LocalEndpoint() = default;

  /// 设置本地端点
  /// 使用主机任意地址初始化 INADDR_ANY or in6addr_any
  ///  @tparam      InternetProtocol    网络协议类型
  ///  @param[in]   internet_protocol   网络协议
  ///  @param[in]   port                本地端口
  ///  @return CRTP对象引用 支持链式调用
  /// @example
  ///    要初始化端口为8888一个IPv4 TCP的端点
  /// @code
  /// asio::ip::tcp::endpoint ep(asio::io::tcp::v4(), 8888);
  /// @endcode
  /// @example
  ///    要初始化端口为8888一个TPv6 UDP的端点
  /// @code
  /// asio::ip::udp::endpoint ep(asio::io::udp::v6(), 8888);
  /// @endcode
  template <typename InternetProtocol>
  TPN_INLINE Derived &SetLocalEndpoint(
      const InternetProtocol &internet_protocol, unsigned short port) {
    NET_DEBUG("LocalEndpoint SetLocalEndpoint protocol {} port {}",
              internet_protocol.protocol(), port);
    this->local_endpoint_ = endpoint_type(internet_protocol, port);
    return (CRTP_CAST(this));
  }

  /// 设置本地端点 用于与特定端口远程接口
  /// 使用传入的地址初始化地址
  ///  @param[in]   addr    地址
  ///  @param[in]   port    端口
  ///  @return CRTP对象引用 支持链式调用
  /// @example
  ///    要初始化端口为xxx:8888一个IPv4 UDP的端点
  /// @code
  /// asio::ip::udp::endpoint ep(asio::ip::address_v4::from_string("..."), 8888);
  /// @endcode
  /// @example
  ///    要初始化端口为xxx:8888一个IPv6 TCP的端点
  /// @code
  /// asio::ip::tcp::endpoint ep(asio::ip::address_v6::from_string("..."), 8888);
  /// @endcode
  ///
  TPN_INLINE Derived &SetLocalEndpoint(const asio::ip::address &addr,
                                       unsigned short port) {
    NET_DEBUG("LocalEndpoint SetLocalEndpoint address {} port {}",
              addr.to_string(), port);
    this->local_endpoint_ = endpoint_type(addr, port);
    return (CRTP_CAST(this));
  }

  /// 获取本地端点
  ///  @return 本地端点
  TPN_INLINE endpoint_type &GetLocalEndpoint() { return this->local_endpoint_; }

 protected:
  endpoint_type local_endpoint_;  ///< 本地端点
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_LOCAL_EDNPOINT_H_
