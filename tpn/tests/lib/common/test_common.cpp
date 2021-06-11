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

#include "fmt_wrap.h"

using namespace std;
using namespace tpn;

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

// random_hub
#include "random_hub.h"

TEST_CASE("random base", "[common]") {
  fmt::print("Rand32          {}\n", Rand32());
  fmt::print("RandI32         {}\n", RandI32());
  fmt::print("RandU32         {}\n", RandU32());
  fmt::print("RandFloat       {}\n", RandFloat());
  fmt::print("RandDouble      {}\n", RandDouble());
  fmt::print("RandMS          {}\n", RandMS());
  fmt::print("RandTime        {}\n",
             static_cast<uint32_t>(
                 RandTime(MilliSeconds(300), MilliSeconds(500)).count()));
  fmt::print("RandNorm        {}\n", RandNorm());
  fmt::print("RandChance      {}\n", RandChance());
  fmt::print("RollRhanceFloat {}\n", RollChanceFloat(80.5));
  fmt::print("RollChanceI32   {}\n", RollChanceI32(30));
}

TEST_CASE("random weight", "[common]") {
  const int kThousand10  = 10000;
  const int kThousand100 = 100000;

  std::vector<double> weights{1000.0, 1500.0, 2000.0, 1500.0, 4000.0, 3000.0};
  std::unordered_map<uint32_t, uint32_t> result_map;

  for (int i = 0; i < kThousand10; ++i) {
    auto n = RandU32Weighted(weights);
    ++result_map[n];
  }

  for (auto &&[n, count] : result_map) {
    fmt::print("{}:    {}\t{}\n", n, count,
               std::string(static_cast<int>(count * 100.0 / kThousand10), '*')
                   .c_str());
  }
}

TEST_CASE("random weight func", "[common]") {
  struct Item {
    Item() = default;
    Item(int id_in, double weight_in) : id(id_in), weight(weight_in) {}

    int id{0};
    double weight{0.0f};
  };

  std::vector<Item> bag;

  for (int i = 0; i < 10; ++i) {
    bag.emplace_back(i + 1, RandDouble());
  }

  for (auto &&item : bag) {
    fmt::print("bag item id:{} weight:{}\n", item.id, item.weight);
  }

  fmt::print("\n");

  for (int i = 0; i < 5; ++i) {
    // auto rand_iter =
    //     RandContainerWeighted(bag, [](const auto &_la) { return _la.weight; });
    // if (rand_iter != bag.end()) {
    //   fmt::print("rand item id:{} weight:{}\n", rand_iter->id,
    //              rand_iter->weight);
    // }
  }
}
