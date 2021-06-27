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

#include "test_service123.h"

#include "test_session.h"
#include "error_code.pb.h"

namespace tpn {

Test2::Test2(std::shared_ptr<TestSession> session_sptr)
    : TestService2(session_sptr) {}

uint32_t Test2::HandleProcessClientRequest21(
    const ::tpn::protocol::SearchRequest *request) {
  return kErrorCodeOk;
}

uint32_t Test2::HandleProcessClientRequest22(
    const ::tpn::protocol::SearchRequest *request,
    ::tpn::protocol::SearchResponse *response,
    std::function<void(ServiceBase *, uint32_t,
                       const google::protobuf::Message *)> &continuation) {
  return kErrorCodeOk;
}

}  // namespace tpn
