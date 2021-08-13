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
      if (ret_vec[i].empty()) {
        LOG_ERROR("field analyze error, name empty {}", field);
        return false;
      }
      name_ = ret_vec[i];
    } else if (1 == i) {  // 字段类型
      type_ = GetTypeByTypeName(ret_vec[i]);
      if (XlsxDataType::kXlsxDataTypeNone == type_) {
        LOG_ERROR("field analyze error, type empty {}", field);
        return false;
      }
      if (XlsxDataType::kXlsxDataTypeComplexObj == type_ ||
          XlsxDataType::kXlsxDataTypeComplexArr == type_) {  // 复合类型需要解析
        std::string delimiters = "";
        int end                = ret_vec[i].length() - 1;
        while (end > 0 && IsTypeNameComplexDelimiter(ret_vec[i][end])) {
          delimiters += ret_vec[i][end];
          --end;
        }
        if (end <= 0) {  // 错误解析
          LOG_ERROR("field analyze error, type empty {}", field);
          return false;
        }
        std::string type_str  = ret_vec[i].substr(0, end + 1);
        std::string delimiter = "";  // 非法
        for (auto &&ch : type_str) {
          if (IsTypeNameComplexDelimiter(ch)) {
            delimiter = std::string(1, ch);
            break;
          }
        }
        if (delimiter.empty()) {
          LOG_ERROR("field analyze error, type empty {}", field);
          return false;
        }
        auto field_vec = Tokenize(type_str, delimiter);
        if (field_vec.empty()) {
          LOG_ERROR("field analyze error, type empty {}", field);
          return false;
        }
        std::vector<XlsxDataType> type_vec;
        for (auto &&type_name : field_vec) {
          auto tmp_type = GetTypeByTypeName(type_name);
          if (XlsxDataType::kXlsxDataTypeNone == tmp_type) {
            LOG_ERROR("field analyze error, type empty {}", field);
            return false;
          }
          type_vec.emplace_back(std::move(tmp_type));
        }
        complex_field_ = make_pair(std::move(delimiters), std::move(type_vec));
      }
    } else if (2 == i) {  // 字段约束
      constraint_type_ = GetConstraintTypeByTypeName(ret_vec[i]);
    } else if (3 == i) {  // 导出类型
      export_type_ = GetExportTypeByTypeName(ret_vec[i]);
    } else if (4 == i) {  // 字段检查
      // TODO 检查内容处理
    }
  }

  return true;
}

void AnalystField::PrintStorage() const {
  fmt::print("name: {}, type: {}, constraint: {}, export: {}\n", name_, type_,
             constraint_type_, export_type_);
  fmt::print("delimiters: {},", complex_field_.first);
  fmt::print("[");
  for (auto &&type : complex_field_.second) {
    fmt::print("{} ", type);
  }
  fmt::print("]\n");
}

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

bool Analyst::Analyze(std::string_view sheet_title, std::string_view field) {
  auto iter = sheet_umap_.find(sheet_title);
  if (sheet_umap_.end() == iter) {
    AnalystSheet sheet(sheet_title);
    sheet_umap_.emplace(sheet.GetSheetTitle(), std::move(sheet));
    iter = sheet_umap_.find(sheet_title);
  }
  return iter->second.Analyze(field);
}

void Analyst::PrintStorage() const {
  for (auto &&[_, val] : sheet_umap_) {
    val.PrintStorage();
  }
}

}  // namespace xlsx

}  // namespace tpn
