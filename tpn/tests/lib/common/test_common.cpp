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

#include <ctime>

using namespace std;

// debug_hub
#include "common.h"
#include "debug_hub.h"

TEST_CASE("debug_hub", "[common]") {
  TPN_ASSERT(3 < 5, "assert 3 < 5");
  TPN_ASSERT(3 > 5, "assert 3 > 5 maybe error : time{}", time(nullptr));
}

// configmgr
#include "config.h"

TEST_CASE("config", "[common]") {
#ifndef _TPN_COMMON_CONFIG_TEST_FILE
#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
#endif

  std::string error;
  g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {}, error);
  if (!error.empty()) {
    cout << error.c_str() << endl;
    return;
  }

  auto host = g_config->GetStringDefault("host", "127.0.0.1");
  cout << "host: " << host.c_str() << endl;

  auto num_1 = g_config->GetI32Default("num_1", -1);
  cout << "num_1: " << num_1 << endl;

  auto num_2 = g_config->GetI64Default("num_2", 1);
  cout << "num_2: " << num_2 << endl;

  auto num_3 = g_config->GetU32Default("num_3", 2);
  cout << "num_3: " << num_3 << endl;

  auto num_4 = g_config->GetU64Default("num_4", 3);
  cout << "num_4: " << num_4 << endl;

  auto num_5 = g_config->GetU64Default("num_5", 4);
  cout << "num_5: " << num_5 << endl;

  auto float_1 = g_config->GetFloatDefault("float_1", 1.0);
  cout << "float_1: " << float_1 << endl;

  auto double_1 = g_config->GetDoubleDefault("double_1", -1.0);
  cout << "double_1: " << double_1 << endl;
}
