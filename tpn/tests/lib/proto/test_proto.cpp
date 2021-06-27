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

#include "../../test_include.h"

#include "log.h"
#include "config.h"
#include "test_service.pb.h"

using namespace std;
using namespace tpn;

#ifndef _TPN_LOGGER_CONFIG_TEST_FILE
#  define _TPN_LOGGER_CONFIG_TEST_FILE "config_logger_test.json"
#endif

class CatchProtoTestService2 : public protocol::TestService1 {
 public:
  CatchProtoTestService2() : protocol::TestService1() {}

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request,
                   std::function<void(ByteBuffer)> callback){};

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request){};

  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    uint32_t status){};

  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    const google::protobuf::Message *response){};

  std::string GetCallerInfo() const { return "CatchProtoTestService1 "; };
};

TEST_CASE("test1", "[proto]") {
  string config_error;
  if (!g_config->Load(_TPN_LOGGER_CONFIG_TEST_FILE, {}, config_error)) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_LOGGER_CONFIG_TEST_FILE, config_error);
    return;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  LOG_INFO("Proto Test Start");

  CatchProtoTestService2 service1;

  LOG_INFO("Proto Test End");
}
