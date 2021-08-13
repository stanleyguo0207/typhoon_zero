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

#include "analyst.h"

#include "utils.h"
#include "log.h"
#include "debug_hub.h"
#include "helper.h"

namespace tpn {

namespace xlsx {

AnalystField::AnalystField() {}

AnalystField::~AnalystField() {}

bool AnalystField::Analyze(std::string_view field) {
  auto ret_vec = Tokenize(field, GetFieldDelimiters());
  if (ret_vec.empty()) {
    LOG_ERROR("field analyze error, {}", field);
    return false;
  }

  for (size_t i = 0; i < ret_vec.size(); ++i) {
    if (0 == i) {  // 字段名
      if (!AnalyzeName(ret_vec[i])) {
        LOG_ERROR("field analyze name error, {}", field);
        return false;
      }
    } else if (1 == i) {  // 字段类型
      if (!AnalyzeType(ret_vec[i])) {
        LOG_ERROR("field analyze type error, {}", field);
        return false;
      }
    } else if (2 == i) {  // 字段约束
      if (!AnalyzeConstraintType(ret_vec[i])) {
        LOG_ERROR("field analyze constraint type error, {}", field);
        return false;
      }
    } else if (3 == i) {  // 导出类型
      if (!AnalyzeExportType(ret_vec[i])) {
        LOG_ERROR("field analyze export type error, {}", field);
        return false;
      }
    } else if (4 == i) {  // 字段检查
      if (!AnalyzeCheck(ret_vec[i])) {
        LOG_ERROR("field analyze check error, {}", field);
        return false;
      }
    }
  }

  return true;
}

bool AnalystField::GenerateJsonDataByType(XlsxDataType type,
                                          rapidjson::Document &document,
                                          rapidjson::Value &row_data,
                                          std::string_view field_name,
                                          std::string_view data) {
  std::string data_str(data.data(), data.length());
  rapidjson::Value key;
  key.SetString(field_name.data(), field_name.length(),
                document.GetAllocator());

  rapidjson::Value val;
  switch (type) {
    case XlsxDataType::kXlsxDataTypeDouble: {
      val.SetDouble(std::stod(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeFloat: {
      val.SetFloat(std::stof(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeI32: {
      val.SetInt(std::stol(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeI64: {
      val.SetInt64(std::stoll(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeU32: {
      val.SetUint(std::stoul(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeU64: {
      val.SetUint64(std::stoull(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeBool: {
      val.SetBool(StringToBool(data_str));
    } break;
    case XlsxDataType::kXlsxDataTypeStr: {
      val.SetString(data_str.data(), data_str.length(),
                    document.GetAllocator());
    } break;
    case XlsxDataType::kXlsxDataTypeDesc: {  // 注释跳过
      return true;
    } break;
    case XlsxDataType::kXlsxDataTypeComplexObj: {
      val.SetObject();
      TPN_ASSERT(1 == complex_field_.first.length(),
                 "{} obj delimiter only be one, {} is error", name_,
                 complex_field_.first);
      auto data_vec = Tokenize(data_str, complex_field_.first);
      TPN_ASSERT(data_vec.size() == complex_field_.second.size(),
                 "{} data length error, {}", name_, data_vec);
      for (size_t i = 0; i < complex_field_.second.size(); ++i) {
        GenerateJsonDataByType(complex_field_.second[i], document, val,
                               "p{}"_format(i + 1), data_vec[i]);
      }
    } break;
    case XlsxDataType::kXlsxDataTypeComplexArr: {
      val.SetArray();
      TPN_ASSERT(complex_field_.first.length() > 1,
                 "obj delimiter only more than one, {} is error",
                 complex_field_.first);
    } break;
    default:
      return false;
  }

  row_data.AddMember(key.Move(), val.Move(), document.GetAllocator());

  return true;
}

bool AnalystField::GenerateJsonData(rapidjson::Document &document,
                                    rapidjson::Value &row_data,
                                    std::string_view data) {
  switch (constraint_type_) {
    case XlsxDataConstraintType::kXlsxDataConstraintTypePrimaryKey:
    case XlsxDataConstraintType::kXlsxDataConstraintTypeNotEmpty: {
      TPN_ASSERT(!data.empty(), "{} constraint could'nt be empty", name_);
    } break;
    default:
      break;
  }

  return GenerateJsonDataByType(type_, document, row_data, name_, data);
}

void AnalystField::PrintStorage() const {
  fmt::print("name: {}, type: {}, constraint: {}, export: {}\n", name_, type_,
             constraint_type_, export_type_);
  if (!complex_field_.first.empty()) {
    fmt::print("delimiters: {},", complex_field_.first);
    fmt::print("[");
    for (auto &&type : complex_field_.second) {
      fmt::print("{} ", type);
    }
    fmt::print("]\n");
  }
}

bool AnalystField::AnalyzeName(std::string_view strv) {
  if (strv.empty()) {
    return false;
  }
  name_.assign(strv.data(), strv.length());
  return true;
}

bool AnalystField::AnalyzeComplexType(std::string_view strv) {
  std::string delimiters = "";
  int end                = strv.length() - 1;
  while (end > 0 && IsTypeNameComplexDelimiter(strv[end])) {
    delimiters += strv[end];
    --end;
  }
  if (end <= 0) {  // 错误解析
    LOG_ERROR("complex type error, end idx: {}, {}", end, strv);
    return false;
  }
  std::string_view type_str = strv.substr(0, end + 1);
  std::string delimiter     = "";  // 非法
  for (auto &&ch : type_str) {
    if (IsTypeNameComplexDelimiter(ch)) {
      delimiter = std::string(1, ch);
      break;
    }
  }
  if (delimiter.empty()) {
    LOG_ERROR("complex type error, delimiter empty type str: {}, {}", type_str,
              strv);
    return false;
  }
  delimiters += delimiter;
  auto field_vec = Tokenize(type_str, delimiter);
  if (field_vec.empty()) {
    LOG_ERROR("complex type error, field_vec empty {}", strv);
    return false;
  }
  std::vector<XlsxDataType> type_vec;
  for (auto &&type_name : field_vec) {
    auto tmp_type = GetTypeByTypeName(type_name);
    if (XlsxDataType::kXlsxDataTypeNone == tmp_type) {
      LOG_ERROR("complex type error, field_vec tmp type error, {}", strv);
      return false;
    }
    type_vec.emplace_back(std::move(tmp_type));
  }
  complex_field_ = make_pair(std::move(delimiters), std::move(type_vec));

  return true;
}

bool AnalystField::AnalyzeType(std::string_view strv) {
  type_ = GetTypeByTypeName(strv);
  if (XlsxDataType::kXlsxDataTypeNone == type_) {
    LOG_ERROR("type error, {}", strv);
    return false;
  }

  if (XlsxDataType::kXlsxDataTypeComplexObj == type_ ||
      XlsxDataType::kXlsxDataTypeComplexArr == type_) {
    return AnalyzeComplexType(strv);
  }
  return true;
}

bool AnalystField::AnalyzeConstraintType(std::string_view strv) {
  constraint_type_ = GetConstraintTypeByTypeName(strv);
  return true;
}

bool AnalystField::AnalyzeExportType(std::string_view strv) {
  export_type_ = GetExportTypeByTypeName(strv);
  return true;
}

bool AnalystField::AnalyzeCheck(std::string_view strv) {
  // TODO 检查内容处理
  return true;
}

AnalystSheet::AnalystSheet() {}

AnalystSheet::AnalystSheet(std::string_view sheet_title)
    : sheet_title_(sheet_title.data(), sheet_title.length()) {}

AnalystSheet::~AnalystSheet() {}

bool AnalystSheet::Analyze(std::string_view field) {
  AnalystField new_field;
  if (new_field.Analyze(field)) {
    fields_.emplace_back(std::move(new_field));
  } else {
    LOG_ERROR("sheet title : {}, analyze error, filed : {}", sheet_title_,
              field);
    return false;
  }

  // 测试
  //PrintStorage();

  return true;
}

bool AnalystSheet::GenerateJsonData(rapidjson::Document &document,
                                    rapidjson::Value &row_data, size_t index,
                                    std::string_view data) {
  TPN_ASSERT(index < fields_.size(), "index error, index: {}, fields size: {}",
             index, fields_.size());

  return fields_[index].GenerateJsonData(document, row_data, data);
}

std::string_view AnalystSheet::GetSheetTitle() const { return sheet_title_; }

void AnalystSheet::PrintStorage() const {
  fmt::print("sheet title : {}\n", sheet_title_);
  for (auto &&field : fields_) {
    field.PrintStorage();
  }
}

Analyst::Analyst() {}

Analyst::~Analyst() {}

bool Analyst::Analyze(xlnt::worksheet &worksheet) {
  LOG_INFO("analyst start analyze worksheet : {}", worksheet.title());

  auto &&ranges = worksheet.rows();
  if (ranges.length() < 1) {
    LOG_INFO("analyst empty worksheet : {}", worksheet.title());
    return false;
  }

  std::string title_raw = GetSheetTitle(worksheet.title());
  TPN_ASSERT(!title_raw.empty(), "sheet title error, title : {}",
             worksheet.title());

  auto &&row = ranges[0];
  for (auto &&cell : row) {
    Analyze(title_raw, cell.to_string());
  }

  LOG_INFO("analyst finish analyze worksheet : {}", worksheet.title());
  return true;
}

bool Analyst::Analyze(std::string_view sheet_title, std::string_view field) {
  std::string title_key(sheet_title.data(), sheet_title.length());
  auto iter = sheet_umap_.find(title_key);
  if (sheet_umap_.end() == iter) {
    AnalystSheet sheet(sheet_title);
    sheet_umap_.emplace(title_key, std::move(sheet));
  }

  return sheet_umap_[title_key].Analyze(field);
}

void Analyst::PrintStorage() const {
  for (auto &&[_, val] : sheet_umap_) {
    val.PrintStorage();
  }
}

bool Analyst::GenerateJsonData(rapidjson::Document &document,
                               rapidjson::Value &row_data,
                               std::string_view sheet_title, size_t index,
                               std::string_view data) {
  TPN_ASSERT(row_data.IsObject(),
             "row data should be object, title: {}, data: {}", sheet_title,
             data);

  std::string title_key(sheet_title.data(), sheet_title.length());
  auto iter = sheet_umap_.find(title_key);
  TPN_ASSERT(sheet_umap_.end() != iter,
             "data not in analyst, title: {}, data: {}", sheet_title, data);

  return iter->second.GenerateJsonData(document, row_data, index, data);
}

}  // namespace xlsx

}  // namespace tpn
