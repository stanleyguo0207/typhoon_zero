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
#include "message_buffer.h"

#include "net.h"

#include "byte_converter.h"
#include "random_hub.h"
#include "utils.h"

#include "../../../../third_party/protobuf/addressbook.pb.h"

#ifndef _TPN_NET_BASE_SERVER_CONFIG_TEST_FILE
#  define _TPN_NET_BASE_SERVER_CONFIG_TEST_FILE \
    "config_net_base_server_test.json"
#endif

using namespace tpn;
using namespace tpn::net;

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS
TPN_NET_FORWARD_DECL_TCP_SESSION_CLASS

template <typename Derived, typename ArgsType>
class TcpSessionTestBase : public TcpSessionBase<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
  TPN_NET_FRIEND_DECL_TCP_SESSION_CLASS

 public:
  using Super = TcpSessionBase<Derived, ArgsType>;

  using Super::Send;

  explicit TcpSessionTestBase(IoHandle &io_handle,
                              SessionMgr<Derived> &session_mgr,
                              size_t buffer_max, size_t buffer_prepare)
      : Super(io_handle, session_mgr, buffer_max, buffer_prepare) {}

  ~TcpSessionTestBase() = default;

 protected:
  void FireRecv(std::shared_ptr<Derived> &this_ptr, protocol::Header &&header,
                MessageBuffer &&packet) {
    Derived &derive = CRTP_CAST(this);

    LOG_INFO("FireRecv recv data");

    tutorial::AddressBook address_book;
    if (!address_book.ParseFromArray(packet.GetBasePointer(),
                                     packet.GetBufferSize())) {
      LOG_ERROR("TcpClientTest FireRecv error");
      Super::Stop();
      return;
    }

    header.set_size(address_book.ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet2(sizeof(header_size) + header.GetCachedSize() +
                          address_book.GetCachedSize());
    packet2.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet2.GetWritePointer();
    packet2.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet2.GetWritePointer();
    packet2.WriteCompleted(address_book.GetCachedSize());
    address_book.SerializeToArray(ptr, address_book.GetCachedSize());

    derive.Send(std::move(packet2));
  }
};

}  // namespace net

}  // namespace tpn

class TcpSessionTest
    : public TcpSessionTestBase<TcpSessionTest, TemplateArgsTcpSession> {
 public:
  using TcpSessionTestBase<TcpSessionTest,
                           TemplateArgsTcpSession>::TcpSessionTestBase;
};

using TcpServerTest = TcpServerBridge<TcpSessionTest>;

int main(int argc, char *argv[]) {
#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string config_error;
  if (!g_config->Load(_TPN_NET_BASE_SERVER_CONFIG_TEST_FILE,
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

  TcpServerTest server;

  LOG_INFO("Tcp base server start init...");

  server.Start(host, port);

  while (std::getchar() != '\n')
    ;

  server.Stop();

  LOG_INFO("Tcp base server shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
