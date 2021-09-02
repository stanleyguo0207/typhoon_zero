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
#include "aoi.h"
#include "aoi_node.h"
#include "config.h"

#ifndef _TPN_AOI_CONFIG_TEST_FILE
#  define _TPN_AOI_CONFIG_TEST_FILE "config_aoi_test.json"
#endif

TEST_CASE("data1", "data") {
  std::string config_error;
  if (!g_config->Load(_TPN_AOI_CONFIG_TEST_FILE, {}, config_error)) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_AOI_CONFIG_TEST_FILE, config_error);
    return;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  std::unique_ptr<tpn::aoi::AOIMgr> aoi_mgr =
      std::make_unique<tpn::aoi::AOIMgr>();

  tpn::aoi::AOINode *node1 = new tpn::aoi::AOINode(aoi_mgr.get());
  tpn::aoi::AOINode *node2 = new tpn::aoi::AOINode(aoi_mgr.get());
  tpn::aoi::AOINode *node3 = new tpn::aoi::AOINode(aoi_mgr.get());

  node1->SetX(10.0f);
  node1->SetY(10.0f);
  node1->SetZ(10.0f);

  node2->SetX(20.0f);
  node2->SetY(20.0f);
  node2->SetZ(20.0f);

  node3->SetX(30.0f);
  node3->SetY(30.0f);
  node3->SetZ(30.0f);

  aoi_mgr->Insert(node1);
  aoi_mgr->Insert(node2);
  aoi_mgr->Insert(node3);

  std::this_thread::sleep_for(3s);
}
