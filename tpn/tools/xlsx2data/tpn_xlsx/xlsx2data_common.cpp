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

#include "xlsx2data_common.h"

#include <string>
#include <unordered_map>

namespace tpn {

namespace xlsx {

namespace {

/// 内部类型转枚举
static const std::unordered_map<std::string_view, XlsxDataType> s_name_2_type =
    {{"double", XlsxDataType::kXlsxDataTypeDouble},
     {"float", XlsxDataType::kXlsxDataTypeFloat},
     {"i32", XlsxDataType::kXlsxDataTypeI32},
     {"i64", XlsxDataType::kXlsxDataTypeI64},
     {"u32", XlsxDataType::kXlsxDataTypeU32},
     {"u64", XlsxDataType::kXlsxDataTypeU64},
     {"bool", XlsxDataType::kXlsxDataTypeBool},
     {"str", XlsxDataType::kXlsxDataTypeStr},
     {"#", XlsxDataType::kXlsxDataTypeDesc},
     {"complex", XlsxDataType::kXlsxDataTypeComplex}};

/// ! " & + - = ? ^ | ~
static constexpr uint8_t s_complex_sign[128] = {
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0};

static constexpr std::string_view s_map_var_name = "datas";

}  // namespace

bool IsTypeNameComplex(std::string_view type_name) {
  for (auto &&c : type_name) {
    if (1 == s_complex_sign[c]) {
      return true;
    }
  }

  return false;
}

XlsxDataType GetTypeByTypeName(std::string_view type_name) {
  auto iter = s_name_2_type.find(type_name);
  if (s_name_2_type.end() == iter) {
    if (IsTypeNameComplex(type_name)) {
      return XlsxDataType::kXlsxDataTypeComplex;
    }
    return XlsxDataType::kXlsxDataTypeNone;
  } else {
    return iter->second;
  }
}

std::string_view GetMapVarName() { return s_map_var_name; }

}  // namespace xlsx

}  // namespace tpn
