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

#include "net.h"

#include "rpc_type.pb.h"
#include "test_service.pb.h"

#include "byte_converter.h"
#include "random_hub.h"
#include "utils.h"

#ifndef _TPN_NET_SERVICE_CLIENT_CONFIG_TEST_FILE
#  define _TPN_NET_SERVICE_CLIENT_CONFIG_TEST_FILE \
    "config_net_service_client_test.json"
#endif

using namespace tpn;
using namespace tpn::net;

/// tcp客户端
class TcpClientTest
    : public TcpClientBase<TcpClientTest, TemplateArgsTcpClient> {
 public:
  using TcpClientBase<TcpClientTest, TemplateArgsTcpClient>::TcpClientBase;

  using Super = TcpClientBase<TcpClientTest, TemplateArgsTcpClient>;

  using Super::Send;

  void FireRecv(std::shared_ptr<TcpClientTest> &this_ptr,
                protocol::Header &&header, MessageBuffer &&packet) {
    LOG_INFO("TcpClientTest recv data");

    if (0 == packet.GetActiveSize()) {
      return;
    }

    protocol::TChatNtf ntf;
    if (!ntf.ParseFromArray(packet.GetReadPointer(),
                                packet.GetActiveSize())) {
      LOG_ERROR("TcpClientTest FireRecv error");
      Stop();
      return;
    }

    fmt::print("recv msg: {}\n", ntf.message_list());
  }

  void FireConnect(std::shared_ptr<TcpClientTest> &this_ptr,
                   std::error_code ec) {
    LOG_INFO("TcpClientTest connect server");

    protocol::TUpdateInfoRequest request;
    request.set_name("机器人_{}"_format(RandU32()));

    protocol::Header header;
    header.set_service_id(0);
    header.set_service_hash(protocol::TChatService::ServiceHash::value);
    header.set_method_id(0x40000001);
    header.set_size(request.ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                         request.GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet.GetWritePointer();
    packet.WriteCompleted(request.GetCachedSize());
    request.SerializeToArray(ptr, request.GetCachedSize());

    Send(std::move(packet));
  }
};

int main(int argc, char *argv[]) {
#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string config_error;
  if (!g_config->Load(_TPN_NET_SERVICE_CLIENT_CONFIG_TEST_FILE,
                      std::vector<std::string>(argv, argv + argc),
                      config_error)) {
    printf("Error in config file: %s\n", config_error.c_str());
    return 1;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  std::string_view host = "127.0.0.1";
  std::string_view port = "9990";

  TcpClientTest client;

  LOG_INFO("Tcp base client start init...");

  client.AutoReconnect(true, MilliSeconds(1000));

  client.Start(host, port);

  char line[512];

  while (std::cin.getline(line, 512)) {
    protocol::TChatRequest request;

    size_t len = strlen(line);
    request.set_message(line, len);

    protocol::Header header;
    header.set_service_id(0);
    header.set_service_hash(protocol::TChatService::ServiceHash::value);
    header.set_method_id(0x40000002);
    header.set_size(request.ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                         request.GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet.GetWritePointer();
    packet.WriteCompleted(request.GetCachedSize());
    request.SerializeToArray(ptr, request.GetCachedSize());

    client.Send(std::move(packet));
  }

  client.Stop();

  LOG_INFO("Tcp base client shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
