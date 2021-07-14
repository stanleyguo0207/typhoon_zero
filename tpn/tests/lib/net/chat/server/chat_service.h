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

#ifndef TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVICE_H_
#define TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVICE_H_

#include "net.h"
#include "service.h"
#include "test_service.pb.h"

using namespace tpn;

namespace test {

class TcpChatSession;

class TcpChatService
    : public net::Service<TcpChatSession, protocol::TChatService> {
 public:
  using TChatService = net::Service<TcpChatSession, protocol::TChatService>;

  TcpChatService(std::shared_ptr<TcpChatSession> session_sptr);

  protocol::ErrorCode HandleUpdateInfo(
      const ::tpn::protocol::TUpdateInfoRequest *request) override;

  protocol::ErrorCode HandleChat(
      const ::tpn::protocol::TChatRequest *request) override;
};

}  // namespace test

#endif  // TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SERVICE_H_
