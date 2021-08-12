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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_

#include <string_view>

#include <xlnt/xlnt.hpp>

#include "define.h"

namespace tpn {

namespace xlsx {

/// 自定义类型
enum class XlsxDataType : uint8_t {
  kXlsxDataTypeNone    = 0,   ///< 默认
  kXlsxDataTypeDouble  = 1,   ///< double
  kXlsxDataTypeFloat   = 2,   ///< float
  kXlsxDataTypeI32     = 3,   ///< i32
  kXlsxDataTypeI64     = 4,   ///< i64
  kXlsxDataTypeU32     = 5,   ///< u32
  kXlsxDataTypeU64     = 6,   ///< u64
  kXlsxDataTypeBool    = 7,   ///< bool
  kXlsxDataTypeStr     = 8,   ///< str
  kXlsxDataTypeDesc    = 9,   ///< #
  kXlsxDataTypeComplex = 10,  ///< complex
};

/// 是否为复杂类型
/// 支持 ! " & + - = ? ^ | ~
/// 常用 - | 
///  @param[in]   type_name     表约定类型
///  @return 包含特殊解析字符的，视为复杂类型，返回true
TPN_XLSX2DATA_API bool IsTypeNameComplex(std::string_view type_name);

/// 根据类型名获取类型
///  @param[in]   type_name     表约定类型
///  @return 返回约定类型对应的内部自定义类型
TPN_XLSX2DATA_API XlsxDataType GetTypeByTypeName(std::string_view type_name);

/// 获取proto文件DataHubMap变量名
///  @return DataHubMap变量名
TPN_XLSX2DATA_API std::string_view GetMapVarName();

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_
