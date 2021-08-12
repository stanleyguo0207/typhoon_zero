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

#include "json_generator.h"

#include <filesystem>

#include <rapidjson/stringbuffer.h>
#include <rapidjson/prettywriter.h>

#include "log.h"
#include "config.h"
#include "debug_hub.h"
#include "fmt_wrap.h"
#include "utils.h"
#include "helper.h"
#include "generator_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

JsonGenerator::JsonGenerator() {}

JsonGenerator::~JsonGenerator() {}

bool JsonGenerator::Load(std::string &error) {
  std::string json_file_path = fmt::format(
      "{}/{}.json", g_config->GetStringDefault("xlsx_json_dir", "json"),
      g_xlsx2data_generator->GetFilePrefix());
  try {
    json_file_.Open(json_file_path, true);

    document_.SetNull();
    document_.SetObject();

    rapidjson::Value key;
    key.SetString(GetMapVarName().data(), GetMapVarName().length());

    rapidjson::Value val(rapidjson::kObjectType);

    document_.AddMember(key.Move(), val.Move(), document_.GetAllocator());
  } catch (FileException &ex) {
    error = std::string{ex.what()};
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()};
    return false;
  }

  return true;
}

bool JsonGenerator::Analyze(std::string_view workbook_path) {
  LOG_INFO("json generator start analyze workbook : {}", workbook_path);

  xlnt::workbook wb;

  try {
    wb.load(workbook_path.data());
  } catch (xlnt::exception &e) {
    LOG_ERROR("json generator load workbook exception error : {}", e.what());
    return false;
  } catch (...) {
    LOG_ERROR("json generator load workbook exception ...");
    return false;
  }

  for (auto &&sheet : wb) {
    if (!SheetTitleIsOutput(sheet.title())) {
      continue;
    }

    if (!Analyze(sheet)) {
      return false;
    }
  }

  LOG_INFO("json generator finish analyze workbook : {}", workbook_path);
  return true;
}

bool JsonGenerator::Analyze(xlnt::worksheet &worksheet) {
  LOG_INFO("json generator start analyze worksheet : {}", worksheet.title());

  auto &&ranges = worksheet.rows();
  if (ranges.length() > 1) {  // 第一行为此列字段的标签定义
    std::string title_raw = GetSheetTitle(worksheet.title());
    TPN_ASSERT(!title_raw.empty(), "sheet title error, title : {}",
               worksheet.title());
    std::string title = LowercaseString(title_raw);

    rapidjson::Value key;
    key.SetString(GetMapVarName().data(), GetMapVarName().length());
    auto &datas = document_[key.Move()];
    TPN_ASSERT(datas.IsObject(), "document_ encode error, key : {}",
               key.GetString());

    rapidjson::Value title_key;
    title_key.SetString(title.data(), title.length(), document_.GetAllocator());

    rapidjson::Value val(rapidjson::kObjectType);
    datas.AddMember(title_key.Move(), val.Move(), document_.GetAllocator());
  }

  LOG_INFO("json generator finish analyze worksheet : {}", worksheet.title());
  return true;
}

bool JsonGenerator::Generate() {
  LOG_INFO("json generator start generate json file");

  rapidjson::StringBuffer str_buf;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(str_buf);
  document_.Accept(writer);

  FmtMemoryBuf buf;
  buf.append(str_buf.GetString(), str_buf.GetString() + str_buf.GetSize());
  json_file_.Write(buf);

  LOG_INFO("json generator finish generate json file");
  return true;
}

}  // namespace xlsx

}  // namespace tpn
