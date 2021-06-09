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

#include "../../../test_include.h"

#include <asio.hpp>

using asio::ip::tcp;

const int kMaxLength = 1024;

void Session(tcp::socket sock) {
  try {
    for (;;) {
      char data[kMaxLength];

      asio::error_code error;
      size_t length = sock.read_some(asio::buffer(data), error);
      if (error == asio::error::eof) {
        break;
      } else if (error) {
        throw asio::system_error(error);
      }
      asio::write(sock, asio::buffer(data, length));
    }
  } catch (std::exception &ex) {
    std::cerr << "Exception in thread: " << ex.what() << "\n";
  }
}

void Server(asio::io_context &io_context, uint16_t port) {
  tcp::acceptor a(io_context, tcp::endpoint(tcp::v4(), port));
  for (;;) {
    std::thread(Session, a.accept()).detach();
  }
}

TEST_CASE("Test asio tcp echo", "[asio]") {
  try {
    asio::io_context io_context;

    Server(io_context, 9001);
  } catch (std::exception &ex) {
    std::cerr << "Exception: " << ex.what() << "\n";
  }
}
