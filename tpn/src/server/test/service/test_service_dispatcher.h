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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE_DISPATCHER_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE_DISPATCHER_H_

#include "byte_buffer.h"
#include "log.h"
#include "common.h"
#include "service_dispatcher.h"
#include "test_service123.h"

namespace tpn {

class TestSession;

using namespace net;

class TestServerServiceDispatcher : public ServiceDispatcher<TestSession> {
 public:
  void Init();

  TPN_SINGLETON_DECL(TestServerServiceDispatcher)
};

}  // namespace tpn

#define g_test_svr_service_dispatcher \
  tpn::TestServerServiceDispatcher::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_TEST_SERVICE_TEST_SERVICE_DISPATCHER_H_
