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
#include <filesystem>

#include <xlnt/xlnt.hpp>

#include "config.h"
#include "banner.h"
#include "log.h"

namespace fs = std::filesystem;

#ifndef _TPN_XLSX2DATA_CONFIG
#  define _TPN_XLSX2DATA_CONFIG "xlsx2data_config.json"
#endif

int main(int argc, char *argv[]) {
  std::string config_error;
  if (!g_config->Load(_TPN_XLSX2DATA_CONFIG,
                      std::vector<std::string>(argv, argv + argc),
                      config_error)) {
    printf("Error in config file:%s, error:%s\n ", _TPN_XLSX2DATA_CONFIG,
           config_error.c_str());
    return 1;
  }

  tpn::BannerShow("typhoon xlsx to data tools", [&]() {});

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  std::string data_dir = g_config->GetStringDefault("xlsx_data_dir", "data");
  for (auto &iter : fs::directory_iterator(data_dir)) {
    LOG_DEBUG("path: {}", iter.path());
    xlnt::workbook wb;  
    wb.load(iter.path());
    for (auto &&sheet : wb) {
      LOG_INFO("sheet: {}", sheet.title());
      for (auto &&row : sheet.rows()) {
        for (auto &&cell : row) {
          printf("%s\n", cell.to_string().c_str());
        }
      }
    }
  }

  std::this_thread::sleep_for(1s);

  return 0;
}
