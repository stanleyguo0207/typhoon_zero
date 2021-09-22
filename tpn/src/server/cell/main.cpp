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
#include <memory>
#include <thread>

#include "fmt_wrap.h"
#include "config.h"
#include "log.h"

#include "entity.h"
#include "entity_coordinate_node.h"
#include "coordinate_system.h"

#ifndef _TPN_SVR_CELL_CONFIG
#  define _TPN_SVR_CELL_CONFIG "cell_config.json"
#endif

int main(int argc, char *argv[]) {
  if (auto error_opt = g_config->Load(
          _TPN_SVR_CELL_CONFIG, std::vector<std::string>(argv, argv + argc))) {
    fmt::print("Error in config file : {}\n", error_opt.value());
    return 1;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  LOG_INFO("Cell server start...");

  auto coordinate_sys_sptr = std::make_shared<tpn::CoordinateSystem>();

  auto entity1_sptr = std::make_shared<tpn::Entity>();
  auto entity2_sptr = std::make_shared<tpn::Entity>();
  auto entity3_sptr = std::make_shared<tpn::Entity>();

  entity1_sptr->SetPosition({10.f, 10.f, 10.f});
  entity2_sptr->SetPosition({20.f, 10.f, 20.f});
  entity3_sptr->SetPosition({20.f, 30.f, 10.f});

  auto entity1_coordinate_sptr =
      new tpn::EntityCoordinateNode(entity1_sptr.get());
  auto entity2_coordinate_sptr =
      new tpn::EntityCoordinateNode(entity2_sptr.get());
  auto entity3_coordinate_sptr =
      new tpn::EntityCoordinateNode(entity3_sptr.get());

  entity1_coordinate_sptr->SetDescStr("node1");
  entity2_coordinate_sptr->SetDescStr("node2");
  entity3_coordinate_sptr->SetDescStr("node3");

  coordinate_sys_sptr->Insert(entity1_coordinate_sptr);
  coordinate_sys_sptr->Insert(entity2_coordinate_sptr);
  coordinate_sys_sptr->Insert(entity3_coordinate_sptr);

  LOG_INFO("Cell server shutdown in 3s...");

  std::this_thread::sleep_for(3s);

  return 0;
}
