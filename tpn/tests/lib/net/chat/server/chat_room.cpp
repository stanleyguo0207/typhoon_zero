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

#include "chat_room.h"
#include "chat_session.hpp"

namespace test {

ChatRoom::ChatRoom() {}
ChatRoom::~ChatRoom() {}

void ChatRoom::Join(std::shared_ptr<TcpChatSession> user) {
  NET_INFO("Join a new user： {}", user->GetCallerInfo());

  users_.emplace(user);
  for (auto &&msg : recent_msg_) {
    user->Deliver(msg);
  }
}

void ChatRoom::Leave(std::shared_ptr<TcpChatSession> user) {
  NET_INFO("Leave a user: {}", user->GetCallerInfo());
  users_.erase(user);
}

void ChatRoom::Deliver(const std::string &msg) {
  recent_msg_.emplace_back(msg);
  while (recent_msg_.size() > kMaxRecentMsgs) {
    recent_msg_.pop_front();
  }

  for (auto &&user : users_) {
    user->Deliver(msg);
  }
}

}  // namespace test
