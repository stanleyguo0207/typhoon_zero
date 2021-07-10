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

#ifndef TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVER_H_
#define TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVER_H_

#include "net.h"

#include "chat_session.hpp"
#include "chat_room.h"
#include "chat_service.h"
#include "chat_service_dispather.h"

using namespace tpn;

namespace test {

template <typename SessionType>
class TcpChatServerBridge
    : public net::TcpServerBase<TcpChatServerBridge<SessionType>, SessionType> {
 public:
  using Super =
      net::TcpServerBase<TcpChatServerBridge<SessionType>, SessionType>;
  using Self = TcpChatServerBridge;

  using Super::TcpServerBase;

  TcpChatServerBridge() : Super(), room_() {
    chat_dispather->AddService<TcpChatService>();
  }

  ~TcpChatServerBridge() { this->Stop(); }

  template <typename... Args>
  TPN_INLINE std::shared_ptr<SessionType> MakeSession(Args &&... args) {
    NET_DEBUG("TcpServerBase MakeSession state {}",
              ToNetStateStr(this->state_));
    return Super::MakeSession(std::forward<Args>(args)..., room_);
  }

  ChatRoom room_;  ///< 房间
};

using TcpChatServer = TcpChatServerBridge<TcpChatSession>;

}  // namespace test

#endif  // TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVER_H_
