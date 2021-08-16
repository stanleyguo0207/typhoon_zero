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

/// 获取license
///  @return License
TPN_XLSX2DATA_API std::string_view GetLicense();

/// 获取proto3 head
///  @return protoe head
TPN_XLSX2DATA_API std::string_view GetProto3Head();

/// 获取proto3 data_hub map结构
///  @return proto3 data_hub map结构
TPN_XLSX2DATA_API std::string_view GetProto3DataHubMap();

/// 根据类型获取proto3类型
///  @param[in]   type            类型
///  @return 返回proto3对应的内部自定义类型
TPN_XLSX2DATA_API std::string GetProto3TypeByType(XlsxDataType type);

/// 返回proto3的message名称
///  @param[in]   type_name       类型名称
///  @return proto3的message名称
TPN_XLSX2DATA_API std::string GetProto3MessageName(std::string_view type_name);

/// 是否为cpp允许的key的类型
/// 可以支持 double float 但是大多数场景不需要支持这两种类型，所以这里没有支持这两种类型
///  @param[in]   type            类型
///  @return 是返回true
TPN_XLSX2DATA_API bool IsCppKeyType(XlsxDataType type);

/// 根据类型获取cpp类型
///  @param[in]   type            类型
///  @return 返回cpp对应的内部自定义类型
TPN_XLSX2DATA_API std::string GetCppTypeByType(XlsxDataType type);

/// 获取cpp文件的管理器名称
//   @return cpp文件的管理器名称
TPN_XLSX2DATA_API std::string_view GetCppDataHubMgrName();

/// 获取cpp文件的管理器域名称
//   @return cpp文件的管理器域名称
TPN_XLSX2DATA_API std::string_view GetCppDataHubMgrNameWithArea();

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_XLSX2DATA_COMMON_H_
