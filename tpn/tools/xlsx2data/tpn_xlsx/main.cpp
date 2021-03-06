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
#include <string>

#include "config.h"
#include "banner.h"
#include "log.h"

#include "generator_hub.h"

#ifndef _TPN_XLSX2DATA_CONFIG
#  define _TPN_XLSX2DATA_CONFIG "xlsx2data_config.json"
#endif

int main(int argc, char *argv[]) {
  do {
    if (auto error =
            g_config->Load(_TPN_XLSX2DATA_CONFIG,
                           std::vector<std::string>(argv, argv + argc))) {
      printf("Error in config file:%s, error:%s\n ", _TPN_XLSX2DATA_CONFIG,
             (*error).c_str());
      break;
    }

    tpn::BannerShow("typhoon xlsx to data tools", [&]() {});

    tpn::log::Init();
    std::shared_ptr<void> log_handle(nullptr,
                                     [](void *) { tpn::log::Shutdown(); });

    std::string data_dir =
        g_config->GetStringDefault("xlsx_data_dir", "xlsx2data/data");

    std::string load_error;
    if (!g_xlsx2data_generator->Load(data_dir, load_error)) {
      printf("Error in data dir:%s, error:%s\n ", data_dir.c_str(),
             load_error.c_str());
      break;
    }

    g_xlsx2data_generator->Generate();
  } while (0);

  std::this_thread::sleep_for(4s);

  return 0;
}
