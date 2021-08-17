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
#include "analyst.h"
#include "generator_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

JsonGenerator::JsonGenerator() {}

JsonGenerator::~JsonGenerator() {}

bool JsonGenerator::Load(std::string &error) {
  std::string json_file_path = fmt::format(
      "{}/{}.json", g_config->GetStringDefault("xlsx_json_dir", "xlsx2data/json"),
      g_xlsx2data_generator->GetFilePrefix());
  try {
    json_file_.Open(json_file_path, true);

    document_.SetNull();
    document_.SetObject();

    rapidjson::Value key;
    key.SetString(GetMapVarName().data(), GetMapVarName().length(),
                  document_.GetAllocator());

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

bool JsonGenerator::Analyze(xlnt::worksheet &worksheet) {
  LOG_INFO("json generator start analyze worksheet : {}", worksheet.title());

  auto &&ranges = worksheet.rows();
  if (ranges.length() > 2) { // 第三行及以上为数据
    std::string title_raw = GetSheetTitle(worksheet.title());
    TPN_ASSERT(!title_raw.empty(), "sheet title error, title : {}",
               worksheet.title());

    // 获取json的根节点
    rapidjson::Value key;
    key.SetString(GetMapVarName().data(), GetMapVarName().length(),
                  document_.GetAllocator());
    auto &datas = document_[key.Move()];
    TPN_ASSERT(datas.IsObject(), "document_ encode error, key : {}",
               key.GetString());

    rapidjson::Value title_key;
    // json 工作表节点
    std::string title = LowercaseString(title_raw);
    title_key.SetString(title.data(), title.length(), document_.GetAllocator());

    rapidjson::Value title_val(rapidjson::kObjectType);

    std::string type_name = GetJsonTypeName(GetDataHubEntryName(title_raw));

    // any 类型的反射type
    rapidjson::Value type_val;
    type_val.SetString(type_name.data(), type_name.length(),
                       document_.GetAllocator());

    title_val.AddMember("@type", type_val.Move(), document_.GetAllocator());

    // repeated 数据
    rapidjson::Value data_key;
    data_key.SetString(GetArrVarName().data(), GetArrVarName().length(),
                       document_.GetAllocator());

    rapidjson::Value data_val;
    data_val.SetArray();

    // 0 格式 1 注释 >2 数据
    for (size_t idx = 2; idx < ranges.length(); ++idx) {
      rapidjson::Value row_data(rapidjson::kObjectType);
      for (size_t i = 0; i < ranges[idx].length(); ++i) {  // 数据分析
        if (!g_xlsx2data_generator->GetAnalyst().GenerateJsonData(
                document_, row_data, title_raw, i,
                ranges[idx][i].to_string())) {
          LOG_ERROR(
              "json generator json data error, title: {}, idx: {}, index: {}, "
              "data: {}",
              title_raw, idx, i, ranges[idx][i].to_string());
          return false;
        }
      }
      data_val.PushBack(row_data.Move(), document_.GetAllocator());
    }

    title_val.AddMember(data_key.Move(), data_val.Move(),
                        document_.GetAllocator());

    datas.AddMember(title_key.Move(), title_val.Move(),
                    document_.GetAllocator());
  }

  LOG_INFO("json generator finish analyze worksheet : {}", worksheet.title());
  return true;
}

bool JsonGenerator::Generate() {
  LOG_INFO("json generator start generate json file");

  rapidjson::StringBuffer str_buf;
  rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(str_buf);
  writer.SetIndent(' ', 1);
  document_.Accept(writer);

  FmtMemoryBuf buf;
  buf.append(str_buf.GetString(), str_buf.GetString() + str_buf.GetSize());
  json_file_.Write(buf);

  LOG_INFO("json generator finish generate json file");
  return true;
}

}  // namespace xlsx

}  // namespace tpn
