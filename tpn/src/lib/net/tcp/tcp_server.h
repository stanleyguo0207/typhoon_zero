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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_

#include "net_common.h"
#include "server.h"
#include "tcp_session.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS

template <typename Derived, typename SessionType>
class TcpServerBase : public ServerBase<Derived, SessionType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
 public:
  explicit TcpServerBase() {}
};

/// tcp服务器桥梁
template <typename SessionType>
class TcpServerBridge
    : public TcpServerBase<TcpServerBridge<SessionType>, SessionType> {
 public:
  using TcpServerBase<TcpServerBridge<SessionType>, SessionType>::TcpServerBase;
};

/// tcp服务器
using TcpServer = TcpServerBridge<TcpSession>;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
