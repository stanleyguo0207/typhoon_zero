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
#include "chat_room.hpp"
#include "chat_service.hpp"
#include "chat_service_dispather.hpp"

namespace tpn {

using namespace test;

namespace net {

class TcpChatServer : public TcpServerBridge<TcpChatSession> {
 public:
  using Super = TcpServerBridge<TcpChatSession>;
  using Self  = TcpChatServer;

  TcpChatServer() : Super(), room_() {
    chat_dispather->AddService<TcpChatService>();
  }
  ~TcpChatServer() { Super::Stop(); }

 protected:
  /// 创造一个tcp会话
  ///  @tapram      Args        会话子类所需额外参数预留类型
  ///  @param[in]   args...     会话子类所需额外参数预留
  template <typename... Args>
  std::shared_ptr<TcpChatSession> MakeSession(Args &&...args) {
    return std::make_shared<TcpChatSession>(
        std::forward<Args>(args)..., room_, this->GetIoHandleByIndex(),
        this->session_mgr_, this->buffer_max_, this->buffer_prepare_);
  }

 protected:
  test::ChatRoom room_;  ///< 房间
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVER_H_
