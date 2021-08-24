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

bool AnalystField::GenerateJsonData(rapidjson::Document &document,
                                    rapidjson::Value &row_data,
                                    std::string_view data) {
  // 检查约束
  switch (constraint_type_) {
    case XlsxDataConstraintType::kXlsxDataConstraintTypePrimaryKey:
    case XlsxDataConstraintType::kXlsxDataConstraintTypeNotEmpty: {
      TPN_ASSERT(!data.empty(), "{} constraint could'nt be empty", name_);
    } break;
    default: {
      if (data.empty()) {  // 允许为空
        return true;
      }
    } break;
  }

  return GenerateJsonData(document, row_data, type_, name_, data);
}

bool AnalystField::GenerateProtoData(Printer &printer, size_t index) {
  return GenerateProtoData(printer, type_, name_, index);
}

void AnalystField::GenerateCppFieldKeys(std::string &field_keys,
                                        std::string &field_comments) {
  if (!IsCppFieldKeys()) {
    return;
  }

  field_keys += fmt::format("{} {}, ", GetCppTypeByType(type_), name_);
  field_comments += fmt::format(" {}-{}", GetCppTypeByType(type_), name_);
}

bool AnalystField::IsCppFieldKeys() {
  if (XlsxDataConstraintType::kXlsxDataConstraintTypePrimaryKey !=
      constraint_type_) {
    return false;
  }
  if (!IsCppKeyType(type_)) {
    TPN_ASSERT(false, "type:{} couldn't be primary key", type_);
    return false;
  }

  return true;
}

std::string_view AnalystField::GetName() { return name_; }

tpn::xlsx::XlsxDataType AnalystField::GetType() { return type_; }

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

  delimiters += delimiter;
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

bool AnalystField::GenerateJsonData(rapidjson::Document &document,
                                    rapidjson::Value &row_data,
                                    XlsxDataType type, std::string_view name,
                                    std::string_view data) {
  std::string data_str(data.data(), data.length());
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
      if (!GenerateJsonDataComplexObj(document, val, data_str,
                                      complex_field_.first)) {
        return false;
      }
    } break;
    case XlsxDataType::kXlsxDataTypeComplexArr: {
      val.SetArray();
      if (!GenerateJsonDataComplexArr(document, val, data_str)) {
        return false;
      }
    } break;
    default:
      return false;
  }

  rapidjson::Value key;
  key.SetString(name.data(), name.length(), document.GetAllocator());
  row_data.AddMember(key.Move(), val.Move(), document.GetAllocator());

  return true;
}

bool AnalystField::GenerateJsonDataComplexObj(rapidjson::Document &document,
                                              rapidjson::Value &val_obj,
                                              std::string_view data,
                                              std::string_view delimiter) {
  TPN_ASSERT(1 == delimiter.length(),
             "{} obj delimiter only be one, {} is error, {}", name_, delimiter,
             data);
  auto data_vec = Tokenize(data, delimiter);
  TPN_ASSERT(data_vec.size() == complex_field_.second.size(),
             "{} data length error, {}", name_, data_vec);
  for (size_t i = 0; i < data_vec.size(); ++i) {
    GenerateJsonData(document, val_obj, complex_field_.second[i],
                     "p{}"_format(i + 1), data_vec[i]);
  }
  return true;
}

bool AnalystField::GenerateJsonDataComplexArr(rapidjson::Document &document,
                                              rapidjson::Value &val_arr,
                                              std::string_view data,
                                              size_t delimiter_index /*= 0*/) {
  // -2时需要转换为obj解析
  TPN_ASSERT(delimiter_index < complex_field_.first.length() - 1,
             "delimiter index error, {} < {}", delimiter_index,
             complex_field_.first);

  rapidjson::Value val;
  if (delimiter_index == complex_field_.first.length() - 2) {  // obj
    std::string delimiter_obj =
        std::string(1, complex_field_.first[complex_field_.first.length() - 1]);

    std::string delimiter =
        std::string(1, complex_field_.first[delimiter_index]);
    auto data_vec = Tokenize(data, delimiter);

    for (auto &&data_str : data_vec) {
      val.SetObject();
      GenerateJsonDataComplexObj(document, val, data_str, delimiter_obj);
      val_arr.PushBack(val.Move(), document.GetAllocator());
    }
  } else {
    std::string delimiter =
        std::string(1, complex_field_.first[delimiter_index]);
    std::string key_name = fmt::format("nest{}", delimiter_index + 1);
    auto data_vec        = Tokenize(data, delimiter);
    for (auto &&data_str : data_vec) {
      val.SetObject();
      rapidjson::Value tmp_arr(rapidjson::kArrayType);
      GenerateJsonDataComplexArr(document, tmp_arr, data_str,
                                 delimiter_index + 1);
      rapidjson::Value tmp_key;
      tmp_key.SetString(key_name.data(), key_name.length(),
                        document.GetAllocator());
      val.AddMember(tmp_key.Move(), tmp_arr.Move(), document.GetAllocator());
      val_arr.PushBack(val.Move(), document.GetAllocator());
    }
  }

  return true;
}

bool AnalystField::GenerateProtoData(Printer &printer, XlsxDataType type,
                                     std::string_view name, size_t index) {
  if (XlsxDataType::kXlsxDataTypeDesc == type) {
    return true;
  } else if (XlsxDataType::kXlsxDataTypeComplexObj == type) {
    printer.Println(fmt::format("message {} {{", CapitalizeFirstLetter(name)));
    printer.Indent();

    for (size_t i = 0; i < complex_field_.second.size(); ++i) {
      printer.Println(fmt::format("{0} p{1} = {1};",
                                  GetProto3TypeByType(complex_field_.second[i]),
                                  i + 1));
    }

    printer.Outdent();
    printer.Println("}");
    printer.Println(fmt::format("{} {} = {};", CapitalizeFirstLetter(name),
                                LowercaseString(name), index));
  } else if (XlsxDataType::kXlsxDataTypeComplexArr == type) {
    printer.Println(fmt::format("message {} {{", CapitalizeFirstLetter(name)));
    printer.Indent();

    for (size_t i = 0; i < complex_field_.first.size() - 2; ++i) {
      printer.Println(fmt::format("message {}Nest{} {{",
                                  CapitalizeFirstLetter(name), i + 1));
      printer.Indent();
    }

    for (size_t i = 0; i < complex_field_.second.size(); ++i) {
      printer.Println(fmt::format("{0} p{1} = {1};",
                                  GetProto3TypeByType(complex_field_.second[i]),
                                  i + 1));
    }

    for (size_t i = 0; i < complex_field_.first.size() - 2; ++i) {
      printer.Outdent();
      printer.Println("}");
      printer.Println(fmt::format("repeated {0}Nest{1} nest{1} = 1;",
                                  CapitalizeFirstLetter(name),
                                  complex_field_.first.size() - 2 - i));
    }

    printer.Outdent();
    printer.Println("}");
    printer.Println(fmt::format("repeated {} {} = {};",
                                CapitalizeFirstLetter(name),
                                LowercaseString(name), index));
  } else {
    printer.Println(
        fmt::format("{} {} = {};", GetProto3TypeByType(type), name, index));
  }

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

bool AnalystSheet::GenerateProtoData(Printer &printer, size_t index) {
  TPN_ASSERT(index < fields_.size(), "index error, index: {}, fields size: {}",
             index, fields_.size());

  return fields_[index].GenerateProtoData(printer, index + 1);
}

bool AnalystSheet::GenerateCppHeadData(Printer &printer) {
  std::string field_comments = "///";
  std::string field_keys     = "";

  std::vector<size_t> key_index_vec;
  for (size_t i = 0; i < fields_.size(); ++i) {
    if (fields_[i].IsCppFieldKeys()) {
      key_index_vec.emplace_back(i);
      fields_[i].GenerateCppFieldKeys(field_keys, field_comments);
    }
  }

  Trim(field_keys);
  if (field_keys.empty() || key_index_vec.empty()) {
    LOG_ERROR("{} not have primary key", sheet_title_);
    return false;
  }

  printer.Println("");
  printer.Println(" public:");
  printer.Indent();
  printer.Println(field_comments);
  std::string_view field_keys_strv(field_keys.data(), field_keys.length() - 1);
  printer.Println(fmt::format(
      "const {0}::{1} *Get{0}({2}) const;", GetProto3MessageName(sheet_title_),
      CapitalizeFirstLetter(sheet_title_), field_keys_strv));
  printer.Outdent();

  printer.Println(" private:");
  printer.Indent();
  if (1 == key_index_vec.size()) {
    printer.Println(fmt::format(
        "std::map<{}, {}::{}> {}_map_;",
        GetCppTypeByType(fields_[key_index_vec[0]].GetType()),
        GetProto3MessageName(sheet_title_), CapitalizeFirstLetter(sheet_title_),
        LowercaseString(sheet_title_)));
  } else {
    printer.Println(fmt::format("std::map<std::string, {}::{}> {}_map_;",
                                GetProto3MessageName(sheet_title_),
                                CapitalizeFirstLetter(sheet_title_),
                                LowercaseString(sheet_title_)));
  }

  printer.Outdent();

  return true;
}

bool AnalystSheet::GenerateCppSourceData(Printer &printer,
                                         Printer &init_printer,
                                         bool init_flag) {
  std::string field_comments = "///";
  std::string field_keys     = "";

  std::vector<size_t> key_index_vec;
  for (size_t i = 0; i < fields_.size(); ++i) {
    if (fields_[i].IsCppFieldKeys()) {
      key_index_vec.emplace_back(i);
      fields_[i].GenerateCppFieldKeys(field_keys, field_comments);
    }
  }

  if (field_keys.empty() || key_index_vec.empty()) {
    LOG_ERROR("{} not have primary key", sheet_title_);
    return false;
  }

  Trim(field_keys);

  printer.Println(field_comments);
  std::string_view field_keys_strv(field_keys.data(), field_keys.length() - 1);
  printer.Println(fmt::format("const {0}::{1} *{2}Get{0}({3}) const {{",
                              GetProto3MessageName(sheet_title_),
                              CapitalizeFirstLetter(sheet_title_),
                              GetCppDataHubMgrNameWithArea(), field_keys_strv));
  printer.Indent();
  if (1 == key_index_vec.size()) {
    printer.Println(
        fmt::format("auto map_key = {};", fields_[key_index_vec[0]].GetName()));
  } else {
    printer.Print("auto map_key = \"\" ");
    for (size_t i = 0; i < key_index_vec.size(); ++i) {
      printer.Outdent();
      printer.Print(
          fmt::format("+ ToString({})", fields_[key_index_vec[i]].GetName()));
      printer.Indent();
    }
    printer.Println(";");
  }
  printer.Outdent();

  printer.Indent();

  printer.Println(fmt::format("auto iter = {}_map_.find(map_key);",
                              LowercaseString(sheet_title_)));
  printer.Println(
      fmt::format("return {}_map_.end() == iter ? nullptr : &(iter->second);",
                  LowercaseString(sheet_title_)));

  printer.Outdent();
  printer.Println("}");

  if (init_flag) {
    init_printer.Println(fmt::format("    if (val.Is<{}>()) {{",
                                     GetProto3MessageName(sheet_title_)));
  } else {
    init_printer.Println(fmt::format("    else if (val.Is<{}>()) {{",
                                     GetProto3MessageName(sheet_title_)));
  }

  init_printer.Println(fmt::format("      {} {};",
                                   GetProto3MessageName(sheet_title_),
                                   LowercaseString(sheet_title_)));
  init_printer.Println(
      fmt::format("      val.UnpackTo(&{});", LowercaseString(sheet_title_)));
  init_printer.Println(fmt::format("      for (auto &&data : {}.datas()) {{",
                                   LowercaseString(sheet_title_)));
  //init_printer.Print("        std::string map_key = \"\" ");
  //for (size_t i = 0; i < key_index_vec.size(); ++i) {
  //  init_printer.Print(fmt::format("+ ToString(data.{}())",
  //                                 fields_[key_index_vec[i]].GetName()));
  //}
  //init_printer.Println(";");
  if (1 == key_index_vec.size()) {
    init_printer.Println(fmt::format("        auto map_key = data.{}();",
                                     fields_[key_index_vec[0]].GetName()));
  } else {
    init_printer.Print("        auto map_key = \"\" ");
    for (size_t i = 0; i < key_index_vec.size(); ++i) {
      init_printer.Print(fmt::format("+ ToString(data.{}())",
                                     fields_[key_index_vec[i]].GetName()));
    }
    init_printer.Println(";");
  }
  init_printer.Println(
      fmt::format("        TPN_ASSERT(0 == {}_map_.count(map_key), \"map key "
                  "repeated. {{}}\", map_key);",
                  LowercaseString(sheet_title_)));
  init_printer.Println(fmt::format("        {}_map_.emplace(map_key, data);",
                                   LowercaseString(sheet_title_)));
  init_printer.Println(fmt::format("      }}"));
  init_printer.Println(fmt::format("    }}"));

  return true;
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

bool Analyst::GenerateProtoData(Printer &printer, std::string_view sheet_title,
                                size_t index) {
  std::string title_key(sheet_title.data(), sheet_title.length());
  auto iter = sheet_umap_.find(title_key);
  TPN_ASSERT(sheet_umap_.end() != iter, "data not in analyst, title: {}",
             sheet_title);

  return iter->second.GenerateProtoData(printer, index);
}

bool Analyst::GenerateCppHeadData(Printer &printer,
                                  std::string_view sheet_title) {
  std::string title_key(sheet_title.data(), sheet_title.length());
  auto iter = sheet_umap_.find(title_key);
  TPN_ASSERT(sheet_umap_.end() != iter, "data not in analyst, title: {}",
             sheet_title);

  return iter->second.GenerateCppHeadData(printer);
}

bool Analyst::GenerateCppSourceData(Printer &printer, Printer &init_printer,
                                    bool init_flag,
                                    std::string_view sheet_title) {
  std::string title_key(sheet_title.data(), sheet_title.length());
  auto iter = sheet_umap_.find(title_key);
  TPN_ASSERT(sheet_umap_.end() != iter, "data not in analyst, title: {}",
             sheet_title);

  return iter->second.GenerateCppSourceData(printer, init_printer, init_flag);
}

}  // namespace xlsx

}  // namespace tpn
