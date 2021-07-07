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

#include "../../../../third_party/protobuf/addressbook.pb.h"

#ifndef _TPN_NET_BASE_CLIENT_CONFIG_TEST_FILE
#  define _TPN_NET_BASE_CLIENT_CONFIG_TEST_FILE \
    "config_net_base_client_test.json"
#endif

using namespace tpn;
using namespace tpn::net;

void ListPeople(const tutorial::AddressBook &address_book) {
  for (auto &&person : address_book.people()) {
    LOG_INFO("Person ID: {}\n", person.id());
    LOG_INFO("\tName: {}\n", person.name());
    if (!person.email().empty()) {
      LOG_INFO("\tE-mail address: {}\n", person.email());
    }
    for (auto &&phone : person.phones()) {
      switch (phone.type()) {
        case tutorial::Person::MOBILE: {
          LOG_INFO("\tMobile phone #:");
        } break;
        case tutorial::Person::HOME: {
          LOG_INFO("\tHome phone #:");
        } break;
        case tutorial::Person::WORK: {
          LOG_INFO("\tWork phone #:");
        } break;
        default: {
          LOG_INFO("\tUnkown phone #:");
        } break;
      }
      LOG_INFO("{}\n", phone.number());
    }
  }
}

/// tcp客户端
class TcpClientTest
    : public TcpClientBase<TcpClientTest, TemplateArgsTcpClient> {
 public:
  using TcpClientBase<TcpClientTest, TemplateArgsTcpClient>::TcpClientBase;

  using Super = TcpClientBase<TcpClientTest, TemplateArgsTcpClient>;

  using Super::Send;

  void FireRecv(std::shared_ptr<TcpClientTest> &this_ptr,
                protocol::Header &&header, MessageBuffer &&packet) {
    ++count_;
    if (count_ > 10) {
      Stop();
      return;
    }
    std::this_thread::sleep_for(1s);

    tutorial::AddressBook address_book;
    if (!address_book.ParseFromArray(packet.GetBasePointer(),
                                     packet.GetBufferSize())) {
      LOG_ERROR("TcpClientTest FireRecv error");
      Stop();
      return;
    }

    ListPeople(address_book);

    auto people = address_book.add_people();
    people->set_id(RandI32());
    people->set_email("123@123.com");
    people->set_name("张三" + ToString(RandU32()));

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

    // Send(std::move(packet2));
  }

  void FireConnect(std::shared_ptr<TcpClientTest> &this_ptr,
                   std::error_code ec) {
    LOG_INFO("TcpClientTest connect server");
  }

 private:
  int count_{0};
};

int main(int argc, char *argv[]) {
#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
  _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

  std::string config_error;
  if (!g_config->Load(_TPN_NET_BASE_CLIENT_CONFIG_TEST_FILE,
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

  std::this_thread::sleep_for(2s);

  tutorial::AddressBook address_book;
  auto people = address_book.add_people();
  people->set_id(RandI32());
  people->set_email("123@123.com");
  people->set_name("张三" + ToString(RandU32()));

  protocol::Header header;
  header.set_size(address_book.ByteSizeLong());

  uint16_t header_size = (uint16_t)header.ByteSizeLong();
  EndianRefMakeLittle(header_size);

  MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                       address_book.GetCachedSize());
  packet.Write(&header_size, sizeof(header_size));
  uint8_t *ptr = packet.GetWritePointer();
  packet.WriteCompleted(header.GetCachedSize());
  header.SerializePartialToArray(ptr, header.GetCachedSize());
  ptr = packet.GetWritePointer();
  packet.WriteCompleted(address_book.GetCachedSize());
  address_book.SerializeToArray(ptr, address_book.GetCachedSize());

  client.Send(std::move(packet));

  while (std::getchar() != '\n')
    ;

  client.Stop();

  LOG_INFO("Tcp base client shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
