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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE123_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE123_H_

#include "common.h"
#include "service.h"
#include "test_service.pb.h"

namespace tpn {

namespace protocol {}

using namespace tpn::protocol;

using namespace net;

class TestSession;

class Test2 : public Service<tpn::protocol::TestService2, TestSession> {
  using TestService2 = Service<tpn::protocol::TestService2, TestSession>;

 public:
  Test2(std::shared_ptr<TestSession> session_sptr);

  uint32_t HandleProcessClientRequest21(
      const ::tpn::protocol::SearchRequest *request) override;

  uint32_t HandleProcessClientRequest22(
      const ::tpn::protocol::SearchRequest *request,
      ::tpn::protocol::SearchResponse *response,
      std::function<void(ServiceBase *, uint32_t,
                         const google::protobuf::Message *)> &continuation)
      override;
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE123_H_
