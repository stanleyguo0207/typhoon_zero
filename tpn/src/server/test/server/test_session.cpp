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

#include "test_session.h"

namespace tpn {

TestSession::TestSession() {}

TestSession::~TestSession() {}

void TestSession::SendRequest(uint32_t service_hash, uint32_t method_id,
                              const google::protobuf::Message *request,
                              std::function<void(ByteBuffer)> callback) {}

void TestSession::SendRequest(uint32_t service_hash, uint32_t method_id,
                              const google::protobuf::Message *request) {}

void TestSession::SendResponse(uint32_t service_hash, uint32_t method_id,
                               uint32_t token, uint32_t status) {}

void TestSession::SendResponse(uint32_t service_hash, uint32_t method_id,
                               uint32_t token,
                               const google::protobuf::Message *response) {}

std::string TestSession::GetCallerInfo() const { return "TestSession"; }

}  // namespace tpn
