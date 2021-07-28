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

#include "generator_hub.h"

#include <filesystem>

#include "log.h"
#include "debug_hub.h"
#include "helper.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

bool GeneratorHub::Load(std::string_view path, std::string &error,
                        bool reload /* = false */) {
  if (!reload) {
    path_ = {path.data(), path.size()};
  }

  xlsx_file_paths_.clear();
  index_ = 0;

  auto xlsx_path = fs::path(path);
  xlsx_path.make_preferred();

  try {
    for (auto &&iter : fs::directory_iterator(xlsx_path)) {
      xlsx_file_paths_.emplace_back(iter.path());
    }

    if (xlsx_file_paths_.empty()) {
      error = std::string("coun't find xlsx files in") + " (" + path_ + ") ";
      return false;
    }
  } catch (fs::filesystem_error &e) {
    error = std::string{e.what()} + " (" + path_ + ") ";
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()} + " (" + path_ + ") ";
    return false;
  }

  return true;
}

bool GeneratorHub::Reload(std::string &error) { return Load({}, error, true); }

bool GeneratorHub::Generate() {
  LOG_INFO("xlsx generator start generate");

  xlnt::workbook wb;

  for (auto &&path : xlsx_file_paths_) {
    LOG_INFO("xlsx generator load path: {}", path);

    wb.load(path);

    for (auto &&sheet : wb) {
      LOG_INFO("xlsx generator load sheet: {}", sheet.title());
      if (!tpn::xlsx::SheetTitleIsOutput(sheet.title())) {
        continue;
      }
      for (auto &&row : sheet.rows()) {
        for (auto &&cell : row) {
          printf("%s\n", cell.to_string().c_str());
        }
      }
    }

    ++index_;
  }

  LOG_INFO("xlsx generator finish generate");

  return true;
}

TPN_SINGLETON_IMPL(GeneratorHub)

}  // namespace xlsx

}  // namespace tpn
