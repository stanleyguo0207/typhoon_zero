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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_

#include "net_common.h"
#include "session.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS
TPN_NET_FORWARD_DECL_TCP_SESSION_CLASS

struct TemplateArgsTcpSession {
  static constexpr bool is_session = true;
  static constexpr bool is_client  = false;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

template <typename Derived, typename ArgsType>
class TcpSessionBase : public SessionBase<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
  TPN_NET_FRIEND_DECL_TCP_SESSION_CLASS

 public:
  explicit TcpSessionBase() {}
};

class TcpSession : public TcpSessionBase<TcpSession, TemplateArgsTcpSession> {
 public:
  using TcpSessionBase<TcpSession, TemplateArgsTcpSession>::TcpSessionBase;
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_
