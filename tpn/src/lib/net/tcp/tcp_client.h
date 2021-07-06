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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_

#include "net_common.h"
#include "client.h"

namespace tpn {

namespace net {

struct TemplateArgsTcpClient {
  static constexpr bool is_session = false;
  static constexpr bool is_client  = true;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

template <typename Derived, typename ArgsType>
class TcpClientBase : public ClientBase<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_CLIENT_CLASS
 public:
  explicit TcpClientBase() {}
};

class TcpClient : public TcpClientBase<TcpClient, TemplateArgsTcpClient> {
 public:
  using TcpClientBase<TcpClient, TemplateArgsTcpClient>::TcpClientBase;
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_
