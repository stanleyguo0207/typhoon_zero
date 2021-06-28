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

#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <filesystem>

#include "config.h"
#include "log.h"
#include "chrono_wrap.h"
// #include "test_service_dispatcher.h"

#ifndef _TPN_SVR_TEST_CONFIG
#  define _TPN_SVR_TEST_CONFIG "test_config.json"
#endif

int main(int argc, char *argv[]) {
  std::string config_error;
  if (!g_config->Load(_TPN_SVR_TEST_CONFIG,
                      std::vector<std::string>(argv, argv + argc),
                      config_error)) {
    printf("Error in config file: %s\n", config_error.c_str());
    return 1;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  LOG_INFO("Test server start init...");

  // g_test_svr_service_dispatcher->Init();

  LOG_INFO("Test server shutdown in 5s...");
  std::this_thread::sleep_for(5s);

  return 0;
}
