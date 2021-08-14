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
  kXlsxDataTypeNone       = 0,   ///< 默认
  kXlsxDataTypeDouble     = 1,   ///< double
  kXlsxDataTypeFloat      = 2,   ///< float
  kXlsxDataTypeI32        = 3,   ///< i32
  kXlsxDataTypeI64        = 4,   ///< i64
  kXlsxDataTypeU32        = 5,   ///< u32
  kXlsxDataTypeU64        = 6,   ///< u64
  kXlsxDataTypeBool       = 7,   ///< bool
  kXlsxDataTypeStr        = 8,   ///< str
  kXlsxDataTypeDesc       = 9,   ///< # or desc
  kXlsxDataTypeComplexObj = 10,  ///< complex obj
  kXlsxDataTypeComplexArr = 11,  ///< complex array
};

/// 约束类型
enum class XlsxDataConstraintType : uint8_t {
  kXlsxDataConstraintTypeNone       = 0,  ///< 无约束
  kXlsxDataConstraintTypePrimaryKey = 1,  ///< 主键 !
  kXlsxDataConstraintTypeNotEmpty   = 2,  ///< 非空 *
};

/// 导出类型
enum class XlsxDataExportType : uint8_t {
  kXlsxDataExportTypeBoth   = 0,  ///< 都导出
  kXlsxDataExportTypeClient = 1,  ///< 仅客户端导出 c
  kXlsxDataExportTypeServer = 2,  ///< 仅服务器导出 s
};

/// 是否为复杂类型分隔符
/// 支持 ! " & + - : ; = ? ^ | ~
///  @param[in]   ch    分隔符
///  @reutrn 是复杂类型分隔符返回true
TPN_XLSX2DATA_API bool IsTypeNameComplexDelimiter(char ch);

/// 是否为复杂类型对象
/// 支持 ! " & + - = ? ^ | ~
/// 常用 - |
///  @param[in]   type_name     表约定类型
///  @return 包含特殊解析字符的，视为复杂类型，返回true
/// @note u32-u32
TPN_XLSX2DATA_API bool IsTypeNameComplexObj(std::string_view type_name);

/// 是否为复杂类型对象数组
/// 最后一位如果是特殊字符则为数组
/// 支持 ! " & + - = ? ^ | ~
/// 常用 - |
///  @param[in]   type_name     表约定类型
///  @return 包含特殊解析字符的，视为复杂类型，返回true
/// @note u32-u32|
TPN_XLSX2DATA_API bool IsTypeNameComplexArr(std::string_view type_name);

/// 根据类型名获取类型
///  @param[in]   type_name     表约定类型
///  @return 返回约定类型对应的内部自定义类型
TPN_XLSX2DATA_API XlsxDataType GetTypeByTypeName(std::string_view type_name);

/// 根据类型名获取约束类型
///  @param[in]   type_name     表约定类型
///  @return 返回约定类型对应的内部约束类型
TPN_XLSX2DATA_API XlsxDataConstraintType
GetConstraintTypeByTypeName(std::string_view type_name);

/// 根据类型名获取导出类型
///  @param[in]   type_name     表约定类型
///  @return 返回约定类型对应的内部导出类型
TPN_XLSX2DATA_API XlsxDataExportType
GetExportTypeByTypeName(std::string_view type_name);

/// 获取proto文件DataHubMap变量名
///  @return DataHubMap变量名
TPN_XLSX2DATA_API std::string_view GetMapVarName();

/// 获取proto文件DataHubEntry repeated 变量名
///  @return DataHubEntry repeated 变量名
TPN_XLSX2DATA_API std::string_view GetArrVarName();

/// 获取字段分隔符
///  @return 字段分隔符
TPN_XLSX2DATA_API std::string_view GetFieldDelimiters();

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_
