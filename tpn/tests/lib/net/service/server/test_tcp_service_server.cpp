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

#include "rpc_type.pb.h"
#include "test_service.pb.h"
#include "message_buffer.h"

#include "net.h"

#include "byte_converter.h"
#include "random_hub.h"
#include "utils.h"
#include "service.h"
#include "service_mgr.h"
#include "error_code.pb.h"

#ifndef _TPN_NET_SERVICE_SERVER_CONFIG_TEST_FILE
#  define _TPN_NET_SERVICE_SERVER_CONFIG_TEST_FILE \
    "config_net_service_server_test.json"
#endif

using namespace tpn;

class TcpServiceSession;

class TcpServiceDispatcher : public net::ServiceMgr<TcpServiceSession> {
 public:
  TPN_SINGLETON_DECL(TcpServiceDispatcher)
};

TPN_SINGLETON_IMPL(TcpServiceDispatcher)

#define test_dispather TcpServiceDispatcher::Instance()

class TcpServiceSession
    : public net::TcpSessionBase<TcpServiceSession,
                                 net::TemplateArgsTcpSession> {
 public:
  using Super =
      net::TcpSessionBase<TcpServiceSession, net::TemplateArgsTcpSession>;
  using Self = TcpServiceSession;

  using Super::TcpSessionBase;

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request,
                   std::function<void(MessageBuffer)> callback) {
    NET_DEBUG("TcpServiceSessionBase SendRequest callback");
  }

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request) {
    NET_DEBUG("TcpServiceSessionBase SendRequest");

    protocol::Header header;
    header.set_service_id(0);
    header.set_service_hash(0x512656A2u);
    header.set_method_id(0x80000001);
    header.set_size(request->ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                         request->GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet.GetWritePointer();
    packet.WriteCompleted(request->GetCachedSize());
    request->SerializeToArray(ptr, request->GetCachedSize());

    Send(std::move(packet));
  }

  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    uint32_t status) {
    NET_DEBUG("TcpServiceSessionBase SendResponse status");
  }

  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    const google::protobuf::Message *response) {
    NET_DEBUG("TcpServiceSessionBase SendResponse response");
  }

  std::string GetCallerInfo() const { return "TcpServiceSessionBase"; }

  void FireRecv(std::shared_ptr<Self> &this_ptr, protocol::Header &&header,
                MessageBuffer &&packet) {
    LOG_INFO("FireRecv recv data");

    test_dispather->Dispatch(this->GetSelfSptr(), header.service_hash(),
                             header.token(), header.method_id(),
                             std::move(packet));
  }
};

using TcpServiceServer = net::TcpServerBridge<TcpServiceSession>;

class TcpTestService3
    : public net::Service<TcpServiceSession, protocol::TestService3> {
  using TestService3 = net::Service<TcpServiceSession, protocol::TestService3>;

 public:
  TcpTestService3(std::shared_ptr<TcpServiceSession> session_sptr)
      : TestService3(session_sptr) {}

  protocol::ErrorCode HandleProcessClientRequest31(
      const ::tpn::protocol::SearchRequest *request) override {
    LOG_INFO("query: {} page_number: {} per_page: {}", request->query(),
             request->page_number(), request->result_per_page());

    protocol::SearchRequest req;
    req.set_query("{}_{}"_format(request->query(), RandU32()));

    ProcessClientRequest31(&req);

    return kErrorCodeOk;
  }
};

int main(int argc, char *argv[]) {
#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string config_error;
  if (!g_config->Load(_TPN_NET_SERVICE_SERVER_CONFIG_TEST_FILE,
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

  TcpServiceServer server;

  test_dispather
      ->AddService<net::Service<TcpServiceSession, protocol::TestService1>>();
  test_dispather
      ->AddService<net::Service<TcpServiceSession, protocol::TestService2>>();
  // test_dispather
  //     ->AddService<net::Service<TcpServiceSession, protocol::TestService3>>();
  test_dispather->AddService<TcpTestService3>();

  LOG_INFO("Tcp base server start init...");

  server.Start(host, port);

  while (std::getchar() != '\n')
    ;

  server.Stop();

  LOG_INFO("Tcp base server shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
