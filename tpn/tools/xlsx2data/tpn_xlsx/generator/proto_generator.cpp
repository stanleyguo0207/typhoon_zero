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
#include "config.h"
#include "debug_hub.h"
#include "fmt_wrap.h"
#include "utils.h"
#include "helper.h"
#include "generator_hub.h"

namespace tpn {

namespace xlsx {

ProtoGenerator::ProtoGenerator() {}

ProtoGenerator::~ProtoGenerator() {}

bool ProtoGenerator::Load(std::string &error) {
  std::string proto_file_path = fmt::format(
      "{}/{}.proto", g_config->GetStringDefault("xlsx_proto_dir", "proto"),
      g_xlsx2data_generator->GetFilePrefix());
  try {
    proto_file_.Open(proto_file_path, true);
  } catch (FileException &ex) {
    error = std::string{ex.what()};
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()};
    return false;
  }

  printer_.Reset();

  // license
  auto license = GetLicense();
  printer_.Print(license);

  // proto3 head
  auto proto3_head = GetProto3Head();
  printer_.Print(proto3_head);

  // map
  auto data_hub_map = GetProto3DataHubMap();
  printer_.Print(data_hub_map);

  proto_file_.Write(printer_.GetBuf());

  return true;
}

bool ProtoGenerator::Analyze(xlnt::worksheet &worksheet) {
  LOG_INFO("proto generator start analyze worksheet : {}", worksheet.title());

  auto &&ranges = worksheet.rows();
  if (ranges.length() > 0) {
    printer_.Reset();  // 重置一下缓冲流

    std::string title_raw = GetSheetTitle(worksheet.title());
    TPN_ASSERT(!title_raw.empty(), "sheet title error, title : {}",
               worksheet.title());

    std::string title = CapitalizeFirstLetter(title_raw);

    printer_.Println(fmt::format("message {} {{", GetProto3MessageName(title)));
    printer_.Indent();

    printer_.Println(fmt::format("message {} {{", title));
    printer_.Indent();

    // 字段解析
    auto &&row = ranges[0];
    for (size_t i = 0; i < row.length(); ++i) {
      if (!g_xlsx2data_generator->GetAnalyst().GenerateProtoData(
              printer_, title_raw, i)) {
        LOG_ERROR(
            "proto generator proto data error, title: {}, index: {}, data: {}",
            title_raw, i, row[i].to_string());
        return false;
      }
    }

    printer_.Outdent();
    printer_.Println("}");
    printer_.Println(fmt::format("repeated {} datas = 1;", title));

    printer_.Outdent();
    printer_.Println("}");
    printer_.Println("");

    proto_file_.Write(printer_.GetBuf());
  }

  LOG_INFO("proto generator finish analyze worksheet : {}", worksheet.title());
  return true;
}

}  // namespace xlsx

}  // namespace tpn
