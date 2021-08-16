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

#include <rapidjson/document.h>

#include "xlsx2data_common.h"
#include "helper.h"

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
  ///  @param[in]   field         要解析的字段内容
  ///  @return 分析成功返回true
  bool Analyze(std::string_view field);

  /// 生成json数据
  ///  @param[in]   document      用来调用 GetAllocator()
  ///  @param[in]   row_data      行数据
  ///  @param[in]   data          要解析的数据
  ///  @return 成功返回true
  bool GenerateJsonData(rapidjson::Document &document,
                        rapidjson::Value &row_data, std::string_view data);

  /// 生成proto描述数据
  ///  @param[in]   printer       打印器
  ///  @param[in]   name            要解析的字段顺序
  ///  @return 成功返回true
  bool GenerateProtoData(Printer &printer, size_t index);

  /// 打印数据
  /// @note 测试用
  void PrintStorage() const;

 private:
  /// 分析字段名
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeName(std::string_view strv);

  /// 分析复杂类型
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeComplexType(std::string_view strv);

  /// 分析类型
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeType(std::string_view strv);

  /// 分析约束类型
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeConstraintType(std::string_view strv);

  /// 分析导出类型
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeExportType(std::string_view strv);

  /// 分析检查
  ///  @param[in]   strv    输入
  ///  @return 成功返回true
  bool AnalyzeCheck(std::string_view strv);

  /// 生成json数据
  ///  @param[in]   document        用来调用 GetAllocator()
  ///  @param[in]   row_data        行数据
  ///  @param[in]   type            类型
  ///  @param[in]   name            要解析的字段名
  ///  @param[in]   data            要解析的数据
  ///  @return 成功返回true
  bool GenerateJsonData(rapidjson::Document &document,
                        rapidjson::Value &row_data, XlsxDataType type,
                        std::string_view name, std::string_view data);

  /// 生成json数据 复合对象
  ///  @param[in]   document        用来调用 GetAllocator()
  ///  @param[in]   val_obj         对象数据
  ///  @param[in]   data            要解析的数据
  ///  @param[in]   delimiter       分隔符
  ///  @return 成功返回true
  bool GenerateJsonDataComplexObj(rapidjson::Document &document,
                                  rapidjson::Value &val_obj,
                                  std::string_view data,
                                  std::string_view delimiter);

  /// 生成json数据 复合对象数组
  ///  @param[in]   document          用来调用 GetAllocator()
  ///  @param[in]   val_arr           数组数据
  ///  @param[in]   data              要解析的数据
  ///  @param[in]   delimiter_index   分隔符位置
  ///  @return 成功返回true
  bool GenerateJsonDataComplexArr(rapidjson::Document &document,
                                  rapidjson::Value &val_arr,
                                  std::string_view data,
                                  size_t delimiter_index = 0);

  /// 生成proto描述数据
  ///  @param[in]   printer       打印器
  ///  @param[in]   type            类型
  ///  @param[in]   name            要解析的字段名
  ///  @param[in]   name            要解析的字段顺序
  ///  @return 成功返回true
  bool GenerateProtoData(Printer &printer, XlsxDataType type,
                         std::string_view name, size_t index);

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
  AnalystSheet();

  /// 构造函数
  ///  @param[in]  sheet_title     工作表名
  AnalystSheet(std::string_view sheet_title);

  ~AnalystSheet();

  /// 分析字段
  ///  @param[in]   field           要解析的字段内容
  ///  @return 分析成功返回true
  bool Analyze(std::string_view field);

  /// 生成json数据
  ///  @param[in]   document      用来调用 GetAllocator()
  ///  @param[in]   row_data      行数据
  ///  @param[in]   index         数据表中下标
  ///  @param[in]   data          要解析的数据
  ///  @return 成功返回true
  bool GenerateJsonData(rapidjson::Document &document,
                        rapidjson::Value &row_data, size_t index,
                        std::string_view data);

  /// 生成proto描述数据
  ///  @param[in]   printer       打印器
  ///  @param[in]   index         数据表中下标
  ///  @return 成功返回true
  bool GenerateProtoData(Printer &printer, size_t index);

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

  /// 分析数据
  ///  @param[in]   worksheet     工作表
  ///  @return 成功返回true
  bool Analyze(xlnt::worksheet &worksheet);

  /// 生成json数据
  ///  @param[in]   document      用来调用 GetAllocator()
  ///  @param[in]   row_data      行数据
  ///  @param[in]   sheet_title   要解析的工作表名
  ///  @param[in]   index         数据表中下标
  ///  @param[in]   data          要解析的数据
  ///  @return 成功返回true
  bool GenerateJsonData(rapidjson::Document &document,
                        rapidjson::Value &row_data,
                        std::string_view sheet_title, size_t index,
                        std::string_view data);

  /// 生成proto描述数据
  ///  @param[in]   printer       打印器
  ///  @param[in]   sheet_title   要解析的工作表名
  ///  @param[in]   index         数据表中下标
  ///  @return 成功返回true
  bool GenerateProtoData(Printer &printer, std::string_view sheet_title,
                         size_t index);

 private:
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
  std::unordered_map<std::string, AnalystSheet> sheet_umap_;  ///< 表结构
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_ANALYST_H_
