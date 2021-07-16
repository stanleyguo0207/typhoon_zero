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

#include <vector>
#include <string>
#include <string_view>

#include "log.h"
#include "config.h"
#include "chrono_wrap.h"
#include "message_buffer.h"
#include "byte_converter.h"
#include "random_hub.h"
#include "utils.h"

#include "error_code.pb.h"
#include "rpc_type.pb.h"
#include "test_service.pb.h"

#include "net.h"

#include "chat_server.hpp"

#ifndef _TPN_NET_CHAT_SERVER_CONFIG_TEST_FILE
#  define _TPN_NET_CHAT_SERVER_CONFIG_TEST_FILE \
    "config_net_chat_server_test.json"
#endif

using namespace tpn;
using namespace test;

int main(int argc, char *argv[]) {
#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string config_error;
  if (!g_config->Load(_TPN_NET_CHAT_SERVER_CONFIG_TEST_FILE,
                      std::vector<std::string>(argv, argv + argc),
                      config_error)) {
    printf("Error in config file: %s\n", config_error.c_str());
    return 1;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  std::string_view host = "0.0.0.0";
  std::string_view port = "9990";

  TcpChatServer server;

  LOG_INFO("Tcp chat server start init...");

  // server.StartTimer(kTimerIdTest, 300ms, [&]() { LOG_DEBUG("Timer call"); });
  // server.StartDelayTimer(kTimerIdTest, 2s, 1s,
  //                        [&]() { LOG_DEBUG("Timer call"); });
  server.StartCycleTimer(kTimerIdTest, 1s, 3,
                         [&]() { LOG_DEBUG("Timer call"); });

  server.Start(host, port);

  while (std::getchar() != '\n')
    ;

  LOG_INFO("Tcp chat server shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
