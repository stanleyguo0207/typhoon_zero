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

class AOINode1 : public tpn::aoi::AOINode {
 public:
  AOINode1() {}
  ~AOINode1() {}

  void SetExtXYZ(float x, float y, float z) {
    ext_x_ = x;
    ext_y_ = y;
    ext_z_ = z;
  }

  float GetExtX() const override { return ext_x_; }
  float GetExtY() const override { return ext_y_; }
  float GetExtZ() const override { return ext_z_; }

 private:
  float ext_x_{0.f};
  float ext_y_{0.f};
  float ext_z_{0.f};
};

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

  AOINode1 *node1 = new AOINode1();
  AOINode1 *node2 = new AOINode1();
  AOINode1 *node3 = new AOINode1();

  node1->SetExtXYZ(10.f, 10.f, 10.f);
  node2->SetExtXYZ(20.f, 20.f, 20.f);
  node3->SetExtXYZ(30.f, 30.f, 30.f);

  aoi_mgr->Insert(node1);
  aoi_mgr->Insert(node2);
  aoi_mgr->Insert(node3);

  std::this_thread::sleep_for(3s);
}
