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

#include <iostream>
#include <fstream>
#include <string>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>

#include "config.h"
#include "fmt_wrap.h"
#include "file_helper.h"
#include "random_hub.h"

#include "addressbook.pb.h"

using namespace std;
using namespace tpn;
using namespace google::protobuf::util;
using namespace rapidjson;

#ifndef _TPN_PROTOBUF_CONFIG_TEST_FILE
#  define _TPN_PROTOBUF_CONFIG_TEST_FILE "config_protobuf_test.json"
#endif

void PromptForAddress(tutorial::Person *person) {
  fmt::print("Enter person ID number: ");
  int id;
  cin >> id;
  person->set_id(id);
  cin.ignore(256, '\n');

  fmt::print("Enter name: ");
  getline(cin, *person->mutable_name());
  // person->set_name("张三");

  fmt::print("Enter email address (blank for none): ");
  string email;
  getline(cin, email);
  if (!email.empty()) {
    person->set_email(email);
  }

  while (true) {
    fmt::print("Enter a phone number (or leave blank to finish): ");
    string number;
    getline(cin, number);
    if (number.empty()) {
      break;
    }

    tutorial::Person::PhoneNumber *phone_number = person->add_phones();
    phone_number->set_number(number);

    fmt::print("Is this a mobile, home, or work phone? ");
    string type;
    getline(cin, type);
    if (type == "mobile") {
      phone_number->set_type(tutorial::Person::MOBILE);
    } else if (type == "home") {
      phone_number->set_type(tutorial::Person::HOME);
    } else if (type == "work") {
      phone_number->set_type(tutorial::Person::WORK);
    } else {
      fmt::print("Unknown phone type.  Using default.");
    }
  }
}

TEST_CASE("addressbook write", "[protobuf]") {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  tutorial::AddressBook address_book;

  {
    fstream input("addressbook", ios::in | ios::binary);
    if (!input) {
      fmt::print("addressbook: File not found.  Creating a new file.\n");
    } else if (!address_book.ParseFromIstream(&input)) {
      fmt::print(stderr, "Failed to parse address book.\n");
      return;
    }
  }

  PromptForAddress(address_book.add_people());

  {
    fstream output("addressbook", ios::out | ios::trunc | ios::binary);
    if (!address_book.SerializeToOstream(&output)) {
      fmt::print(stderr, "Failed to write address book.\n");
      return;
    }
  }
}

void ListPeople(const tutorial::AddressBook &address_book) {
  for (auto &&person : address_book.people()) {
    fmt::print("Person ID: {}\n", person.id());
    fmt::print("\tName: {}\n", person.name());
    if (!person.email().empty()) {
      fmt::print("\tE-mail address: {}\n", person.email());
    }
    for (auto &&phone : person.phones()) {
      switch (phone.type()) {
        case tutorial::Person::MOBILE: {
          fmt::print("\tMobile phone #:");
        } break;
        case tutorial::Person::HOME: {
          fmt::print("\tHome phone #:");
        } break;
        case tutorial::Person::WORK: {
          fmt::print("\tWork phone #:");
        } break;
        default: {
          fmt::print("\tUnkown phone #:");
        } break;
      }
      fmt::print("{}\n", phone.number());
    }
  }
}

void ListPeopleJson(const tutorial::AddressBook &address_book) {
  using namespace google::protobuf::util;

  std::string json_string;

  JsonPrintOptions options;
  options.add_whitespace                = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names    = true;

  MessageToJsonString(address_book, &json_string, options);

  fmt::print("JSON: {}", json_string);
}

TEST_CASE("addressbook read", "[protobuf]") {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  tutorial::AddressBook address_book;

  {
    fstream input("addressbook", ios::in | ios::binary);
    if (!address_book.ParseFromIstream(&input)) {
      fmt::print(stderr, "Failed to parse address book.\n");
      return;
    }
  }

  ListPeople(address_book);

  ListPeopleJson(address_book);
}

#include "complex_type.pb.h"

TEST_CASE("complex", "[protobuf]") {
  using namespace google::protobuf::util;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  test::ComplexObject obj;
  obj.set_id(12345);
  obj.set_name("complex obj");
  obj.add_phones("1122334455");
  obj.add_phones("1122334466");
  obj.set_gender(test::Gender::kGenderMan);
  auto ret1 = obj.add_results();
  ret1->set_url("ret1");
  ret1->set_title("ret1");
  ret1->add_nums(20);
  ret1->add_nums(21);
  auto ret2 = obj.add_results();
  ret2->set_url("ret2");
  ret2->set_title("ret2");
  ret2->add_nums(22);
  ret2->add_nums(23);
  test::MapInfo info1;
  info1.set_id(1);
  info1.set_name("info1");
  obj.mutable_map_info()->insert({info1.id(), std::move(info1)});
  test::MapInfo info2;
  info2.set_id(2);
  info2.set_name("info2");
  obj.mutable_map_info()->insert({info2.id(), std::move(info2)});

  std::string json_string;

  JsonPrintOptions options;
  options.add_whitespace                = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names    = true;

  MessageToJsonString(obj, &json_string, options);

  fmt::print("JSON: {}", json_string);

  auto iter = obj.map_info().find(1);
  if (obj.map_info().end() != iter) {
    fmt::print("map info key:{} name:{}\n", iter->second.id(),
               iter->second.name());
  }

  {
    fstream output("complex_obj", ios::out | ios::trunc | ios::binary);
    if (!obj.SerializeToOstream(&output)) {
      fmt::print(stderr, "Failed to write complex obj.\n");
      return;
    }
  }
}

TEST_CASE("complex read", "[protobuf]") {
  using namespace google::protobuf::util;

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  test::ComplexObject obj;

  {
    fstream input("complex_obj", ios::in | ios::binary);
    if (!obj.ParseFromIstream(&input)) {
      fmt::print(stderr, "Failed to parse complex obj.\n");
      return;
    }
  }

  std::string json_string;

  JsonPrintOptions options;
  options.add_whitespace                = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names    = true;

  MessageToJsonString(obj, &json_string, options);

  fmt::print("JSON: {}", json_string);
}

#include "data.pb.h"

TEST_CASE("data write", "[protobuf]") {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  string config_error;
  if (!g_config->Load(_TPN_PROTOBUF_CONFIG_TEST_FILE, {}, config_error)) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_PROTOBUF_CONFIG_TEST_FILE, config_error);
    return;
  }

  data::DataHubEntryItem item;

  auto item_data1 = item.add_datas();
  item_data1->set_id(1);
  item_data1->set_type(1);
  item_data1->set_sub_type(1);
  item_data1->set_name("道具1");
  item_data1->set_quality(1);

  auto item_data2 = item.add_datas();
  item_data2->set_id(2);
  item_data2->set_type(2);
  item_data2->set_sub_type(1);
  item_data2->set_name("道具2");
  item_data2->set_quality(2);

  data::DataHubEntryLevel level;

  for (int i = 1; i <= 5; ++i) {
    auto level_data = level.add_datas();
    level_data->set_level(i);
    level_data->set_exp(i * 1000);
  }

  data::DataHubEntryShop shop;

  for (int i = 0; i < 3; ++i) {
    auto shop_data = shop.add_datas();
    shop_data->set_id(i + 1);
    shop_data->set_type(RandU32(1, 5));
    shop_data->mutable_item()->set_p1(RandU32(1, 20));
    shop_data->mutable_item()->set_p2(RandU32(100, 1000));
    shop_data->mutable_price()->set_p1(RandU32(9000, 9001));
    shop_data->mutable_price()->set_p2(RandU32(100, 1000));
  }

  data::DataHubEntryPack pack;

  for (int i = 0; i < 2; ++i) {
    auto pack_data = pack.add_datas();
    pack_data->set_id(i + 1);
    for (int i = 0; i < RandU32(2, 4); ++i) {
      auto pool_data = pack_data->add_pool();
      pool_data->set_p1(RandU32(1, 1000));
      pool_data->set_p2(RandU32(100, 1000));
      pool_data->set_p3(RandU32(1, 10000));
    }
  }

  data::DataHubMap data_map;
  (*data_map.mutable_datas())["item"].PackFrom(item);
  (*data_map.mutable_datas())["level"].PackFrom(level);
  (*data_map.mutable_datas())["shop"].PackFrom(shop);
  (*data_map.mutable_datas())["pack"].PackFrom(pack);

  std::string json_string;

  JsonPrintOptions options;
  options.add_whitespace                = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names    = true;

  MessageToJsonString(data_map, &json_string, options);

  FileHelper helper;
  helper.Open("data.json", true);

  FmtMemoryBuf buf;
  buf.append(json_string.data(), json_string.data() + json_string.length());
  helper.Write(buf);
}

#include <filesystem>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

TEST_CASE("data read", "[protobuf]") {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  string config_error;
  if (!g_config->Load(_TPN_PROTOBUF_CONFIG_TEST_FILE, {}, config_error)) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_PROTOBUF_CONFIG_TEST_FILE, config_error);
    return;
  }

  namespace fs   = std::filesystem;
  auto json_file = fs::absolute("data.json");
#ifdef _WIN32
  FILE *fp = fopen(json_file.generic_string().c_str(), "rb");
#else
  FILE *fp = fopen(json_file.generic_string().c_str(), "r");
#endif
  char readBuffer[65536];
  FileReadStream is(fp, readBuffer, sizeof(readBuffer));

  Document d;
  d.ParseStream<kParseCommentsFlag>(is);
  if (d.HasParseError()) {
    fmt::print(stderr, "parse json error, {}", d.GetParseError());
    return;
  }

  StringBuffer buffer;
  Writer<StringBuffer> writer(buffer);
  d.Accept(writer);

  data::DataHubMap data_map;

  JsonStringToMessage(buffer.GetString(), &data_map);

  std::string json_string;

  JsonPrintOptions options;
  options.add_whitespace                = true;
  options.always_print_primitive_fields = true;
  options.preserve_proto_field_names    = true;

  MessageToJsonString(data_map, &json_string, options);

  fmt::print("Json : \n{}\n", json_string);
}
