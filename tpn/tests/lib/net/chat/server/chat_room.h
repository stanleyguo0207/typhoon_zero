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

#ifndef TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_ROOM_H_
#define TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_ROOM_H_

#include <set>
#include <deque>
#include <string>

#include "log.h"
#include "test_service.pb.h"

using namespace tpn;

namespace test {

class TcpChatSession;

static constexpr uint32_t kMaxRecentMsgs = 100;

class ChatRoom {
  using ChatMessageQueue = std::deque<std::string>;

 public:
  ChatRoom();
  ~ChatRoom();

  void Join(std::shared_ptr<TcpChatSession> user);

  void Leave(std::shared_ptr<TcpChatSession> user);

  void Deliver(const std::string &msg);

 private:
  std::set<std::shared_ptr<TcpChatSession>> users_;  ///<  聊天室内会话
  ChatMessageQueue recent_msg_;                      ///< 消息
};

}  // namespace test

#endif  // TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_ROOM_H_
