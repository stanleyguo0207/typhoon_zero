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

#include "data_entry.h"

#include <cstdio>

#include <string>

namespace tpn {

namespace data {

#ifndef _TPN_DATA_FILE_PATH
#  define _TPN_DATA_FILE_PATH "data/data_hub.bin"
#endif

bool Init() {
  std::string data_error;
  if (!g_data_hub->Load(_TPN_DATA_FILE_PATH, data_error)) {
    printf("Error in data file: %s\n", data_error.c_str());
    return false;
  }

  return true;
}

}  // namespace data

}  // namespace tpn
