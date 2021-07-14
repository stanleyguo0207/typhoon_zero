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

#include "chat_service.h"
#include "chat_session.hpp"
#include "chat_room.h"

namespace test {

TcpChatService::TcpChatService(std::shared_ptr<TcpChatSession> session_sptr)
    : TChatService(session_sptr) {}

protocol::ErrorCode TcpChatService::HandleUpdateInfo(
    const ::tpn::protocol::TUpdateInfoRequest *request) {
  LOG_INFO("client set name {}", request->name());
  this->session_sptr_->SetName(request->name());
  this->session_sptr_->GetRoom().Join(this->session_sptr_);
  return kErrorCodeOk;
}

protocol::ErrorCode TcpChatService::HandleChat(
    const ::tpn::protocol::TChatRequest *request) {
  //this->session_sptr_->Chat(request->message());
  this->session_sptr_->GetRoom().Deliver(request->message());
  return kErrorCodeOk;
}

}  // namespace test
