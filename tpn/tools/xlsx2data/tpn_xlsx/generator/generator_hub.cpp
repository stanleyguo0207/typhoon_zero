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
#include "config.h"
#include "debug_hub.h"
#include "proto_generator.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

bool GeneratorHub::Load(std::string_view path, std::string &error,
                        bool reload /* = false */) {
  if (!reload) {
    path_ = {path.data(), path.size()};
  }

  xlsx_file_paths_.clear();

  auto xlsx_path = fs::path(path);
  xlsx_path.make_preferred();

  try {
    for (auto &&iter : fs::directory_iterator(xlsx_path)) {
      xlsx_file_paths_.emplace_back(iter.path().generic_string());
    }

    if (xlsx_file_paths_.empty()) {
      error = std::string("coudn't find xlsx files in") + " (" + path_ + ") ";
      return false;
    }

    // proto生成路径
    std::string proto_file_path =
        g_config->GetStringDefault("xlsx_proto_dir", "proto") +
        "/data_hub.proto";
    proto_file_.Open(proto_file_path, true);

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

  GenerateProtoFileHeader();

  for (auto &&path : xlsx_file_paths_) {
    if (!GenerateProtoFile(path)) {
      LOG_ERROR("xlsx generator generate proto file fail, workbook path : {} ",
                path);
      return false;
    }
  }

  LOG_INFO("xlsx generator finish generate");

  return true;
}

bool GeneratorHub::GenerateProtoFile(std::string_view workbook_path) {
  LOG_INFO("xlsx generator start generate proto file, workbook path : {}",
           workbook_path);

  xlnt::workbook wb;

  try {
    wb.load(workbook_path.data());
  } catch (xlnt::exception &e) {
    LOG_ERROR("xlsx generator load workbook exception error : {}", e.what());
    return false;
  } catch (...) {
    LOG_ERROR("xlsx generator load workbook exception ...");
    return false;
  }

  ProtoGenerator proto_gr(wb, proto_file_);

  // proto文件生成
  if (!proto_gr.Generate()) {
    LOG_INFO("xlsx generator generate proto file fail, workbook path : {}",
             workbook_path);
    return false;
  }

  LOG_INFO("xlsx generator finish generate proto file, workbook path : {}",
           workbook_path);
  return true;
}

static constexpr std::string_view s_proto3_header = R"Header(//
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

syntax = "proto3";

package tpn.data;

option optimize_for = SPEED;
option cc_generic_services = false;

)Header";

void GeneratorHub::GenerateProtoFileHeader() {
  FmtMemoryBuf buf;
  buf.append(s_proto3_header.data(),
             s_proto3_header.data() + s_proto3_header.size());
  //fmt::format_to(FmtBufferAppender(buf), s_proto3_header);
  proto_file_.Write(buf);
}

TPN_SINGLETON_IMPL(GeneratorHub)

}  // namespace xlsx

}  // namespace tpn
