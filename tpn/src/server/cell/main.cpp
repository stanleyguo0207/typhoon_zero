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
  LOG_INFO("Cell server shutdown in 3s...");

  std::this_thread::sleep_for(3s);

  return 0;
}
