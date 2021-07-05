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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

/// 网络层服务器基类
///  @tparam  Derived     服务器子类
///  @tparam  SessionType 网络层会话类型
template <typename Derived, typename SessionType>
class ServerBase : public CRTPObject<Derived, false> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
 public:
 protected:
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_
