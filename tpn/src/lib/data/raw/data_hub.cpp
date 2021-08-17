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

#include "data_hub.h"

#include <fstream>
#include <filesystem>

#include "log.h"
#include "utils.h"
#include "config.h"
#include "debug_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace data {

bool DataHubMgr::Load(std::string_view path, std::string &error,
                      bool reload /*  = false */) {
  if (!reload) {
    path_ = {path.data(), path.size()};
  }
  return true;
}

bool DataHubMgr::Reload(std::string &error) { return Load({}, error, true); }

std::string_view DataHubMgr::GetPath() { return path_; }

}  // namespace data

}  // namespace tpn
