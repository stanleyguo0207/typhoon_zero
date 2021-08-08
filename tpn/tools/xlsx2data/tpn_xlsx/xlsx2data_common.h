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
  kXlsxDataTypeI8      = 3,   ///< i8
  kXlsxDataTypeI16     = 4,   ///< i16
  kXlsxDataTypeI32     = 5,   ///< i32
  kXlsxDataTypeI64     = 6,   ///< i64
  kXlsxDataTypeU8      = 7,   ///< u8
  kXlsxDataTypeU16     = 8,   ///< u16
  kXlsxDataTypeU32     = 9,   ///< u32
  kXlsxDataTypeU64     = 10,  ///< u64
  kXlsxDataTypeStr     = 11,  ///< str
  kXlsxDataTypeDesc    = 12,  ///< desc
  kXlsxDataTypeComplex = 13,  ///< complex
};

/// 是否为复杂类型
/// 支持 - | ; % #
///  @param[in]   type_name     表约定类型
///  @return 包含特殊解析字符的，视为复杂类型，返回true
TPN_XLSX2DATA_API bool IsTypeNameComplex(std::string_view type_name);

/// 根据类型名获取类型
///  @param[in]   type_name     表约定类型
///  @return 返回约定类型对应的内部自定义类型
TPN_XLSX2DATA_API XlsxDataType GetTypeByTypeName(std::string_view type_name);

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_
