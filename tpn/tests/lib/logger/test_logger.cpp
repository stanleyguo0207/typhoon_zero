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

using namespace std;

// formatter
#include "formatter.h"

using namespace tpn;
using namespace tpn::log;

TEST_CASE("formatter", "[logger]") {
  LogMsg msg("test", LogLevel::kLogLevelInfo,
             SourceLocation(__FILE__, __FUNCTION__, __LINE__),
             "This is test formatter.\n");
  FmtMemoryBuf buf;
  try {
    fmt::format_to(buf, "msg: {}", msg);
  } catch (const std::exception &ex) {
    fmt::print("error {}\n", ex.what());
  }
  std::string_view strv(buf.data(), buf.size());
  fmt::print("{}", strv);

  AsyncLogMsg async_msg(msg);
  buf.clear();
  fmt::format_to(buf, "async msg: {}", msg);
  strv = {buf.data(), buf.size()};
  fmt::print("{}", strv);
}
