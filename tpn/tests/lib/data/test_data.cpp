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

#include "log.h"
#include "utils.h"
#include "config.h"
#include "fmt_wrap.h"
#include "data_entry.h"

#ifndef _TPN_DATA_CONFIG_TEST_FILE
#  define _TPN_DATA_CONFIG_TEST_FILE "config_data_test.json"
#endif

TEST_CASE("data1", "data") {
  if (auto error = g_config->Load(_TPN_DATA_CONFIG_TEST_FILE, {})) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_DATA_CONFIG_TEST_FILE, *error);
    return;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  GOOGLE_PROTOBUF_VERIFY_VERSION;

  std::shared_ptr<void> protobuf_handle(
      nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  LOG_INFO("data data1 start");

  if (!tpn::data::Init()) {
    return;
  }

  LOG_DEBUG("data path: {}", g_data_hub->GetPath());

  for (uint32_t i = 1; i < 4; ++i) {
    auto skill_data = g_data_hub->GetDataHubEntrySkill(1, i);
    if (skill_data) {
      LOG_DEBUG("skill data, id:{}, level:{}, name:{}, type:{}",
                skill_data->id(), skill_data->level(),
                tpn::MaybeUtf8ToStr(skill_data->name()),
                skill_data->type());
    } else {
      LOG_WARN("skill data, id:{}, level:{} not exist", 1, i);
    }
  }

  LOG_INFO("data data1 end");

  std::this_thread::sleep_for(3s);
}
