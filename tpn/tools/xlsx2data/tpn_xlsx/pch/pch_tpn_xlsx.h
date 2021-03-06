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

#ifndef TPN_TOOLS_XLSX2DATA_TPN_XLSX_PCH_PCH_TPN_XLSX_H_
#define TPN_TOOLS_XLSX2DATA_TPN_XLSX_PCH_PCH_TPN_XLSX_H_

#include <limits>
#include <vector>
#include <fstream>
#include <string>
#include <string_view>
#include <filesystem>
#include <unordered_map>

#include <xlnt/xlnt.hpp>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>

#include <rapidjson/writer.h>
#include <rapidjson/document.h>
#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/filereadstream.h>

#include "log.h"
#include "utils.h"
#include "config.h"
#include "banner.h"
#include "debug_hub.h"
#include "fmt_wrap.h"

#endif // TPN_TOOLS_XLSX2DATA_TPN_XLSX_PCH_PCH_TPN_XLSX_H_
