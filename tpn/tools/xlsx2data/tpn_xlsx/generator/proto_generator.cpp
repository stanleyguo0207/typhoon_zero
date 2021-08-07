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

#include "proto_generator.h"

#include "log.h"
#include "helper.h"
#include "utils.h"

namespace tpn {

namespace xlsx {

ProtoGenerator::ProtoGenerator(xlnt::workbook &workbook)
    : workbook_(workbook) {}

ProtoGenerator::~ProtoGenerator() {}

bool ProtoGenerator::Generate() {
  LOG_INFO("proto generator start generate");

  for (auto &&sheet : workbook_) {
    LOG_INFO("proto generator load sheet: {}", sheet.title());
    if (!tpn::xlsx::SheetTitleIsOutput(sheet.title())) {
      continue;
    }

    FmtMemoryBuf buf;

    for (auto &&row : sheet.rows()) {
      for (auto &&cell : row) {
        fmt::format_to(FmtBufferAppender(buf), "{}\t", cell.to_string());
      }
      fmt::format_to(FmtBufferAppender(buf), "\n");
    }

    LOG_DEBUG("\n{}", std::string_view(buf.data(), buf.size()));
  }

  LOG_INFO("proto generator finish generate");

  return true;
}

}  // namespace xlsx

}  // namespace tpn
