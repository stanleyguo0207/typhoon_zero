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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_ANALYST_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_ANALYST_H_

#include <string>
#include <string_view>
#include <vector>
#include <unordered_map>

#include "xlsx2data_common.h"

namespace tpn {

namespace xlsx {

/// 复合类型结构
/// first   分词符号
/// second  分词类型
/// u32-u32
/// 1001-800
/// 会被解析为 <"-", {kXlsxDataTypeU32,kXlsxDataTypeU32}>
/// u32-u32|
/// 1001-800|1002-900
/// 会被解析为 <"|-", {kXlsxDataTypeU32,kXlsxDataTypeU32}>
/// u32-u32;|
/// 1001-800;1002-300|2001-800;1002-500;2003-600
/// 会被解析为 <"|;-", {kXlsxDataTypeU32,kXlsxDataTypeU32}>
using AnalystComplexField = std::pair<std::string, std::vector<XlsxDataType>>;

/// 分析字段
/// 字段名 @ 字段类型 @ 字段约束 @ 导出类型 @ 字段检查
class TPN_XLSX2DATA_API AnalystField {
 public:
  AnalystField();

  ~AnalystField();

  /// 分析字段
  /// @分割
  ///  @param[in]   field           要解析的字段内容
  ///  @return 分析成功返回true
  bool Analyze(std::string_view field);

  /// 打印数据
  /// @note 测试用
  void PrintStorage() const;

 private:
  std::string name_;                                    ///< 字段名
  XlsxDataType type_{XlsxDataType::kXlsxDataTypeNone};  ///< 字段类型
  XlsxDataConstraintType constraint_type_{
      XlsxDataConstraintType::kXlsxDataConstraintTypeNone};  ///< 约束
  XlsxDataExportType export_type_{
      XlsxDataExportType::kXlsxDataExportTypeBoth};  ///< 导出类型
  AnalystComplexField
      complex_field_;  ///< 复合类型字段，只有当type_为kXlsxDataTypeComplexObj或者kXlsxDataTypeComplexArr生效
};

/// 分析表
class TPN_XLSX2DATA_API AnalystSheet {
 public:
  /// 构造函数
  ///  @param[in]  sheet_title     工作表名
  AnalystSheet(std::string_view sheet_title);

  ~AnalystSheet();

  /// 分析字段
  ///  @param[in]   field           要解析的字段内容
  ///  @return 分析成功返回true
  bool Analyze(std::string_view field);

  /// 获取表名
  ///  @return 表名
  std::string_view GetSheetTitle() const;

  /// 打印数据
  /// @note 测试用
  void PrintStorage() const;

 private:
  std::string sheet_title_;           ///< 表名
  std::vector<AnalystField> fields_;  ///< 字段
};

/// 分析器
class TPN_XLSX2DATA_API Analyst {
 public:
  Analyst();

  ~Analyst();

  /// 分析字段
  /// 字段会追加到之前分析的表 fields_中
  ///  @param[in]   sheet_title     要解析的工作表名
  ///  @param[in]   field           要解析的字段内容
  ///  @return 分析成功返回true
  bool Analyze(std::string_view sheet_title, std::string_view field);

  /// 打印数据
  /// @note 测试用
  void PrintStorage() const;

 private:
  std::unordered_map<std::string_view, AnalystSheet> sheet_umap_;  ///< 表结构
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_ANALYST_H_
