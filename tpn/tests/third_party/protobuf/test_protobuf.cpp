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

#include "fmt_wrap.h"
#include "addressbook.pb.h"

using namespace std;
using namespace tpn;

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
}
