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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_GENERATOR_HUB_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_GENERATOR_HUB_H_

#include <vector>
#include <string>
#include <string_view>

#include "file_helper.h"
#include "xlsx2data_common.h"
#include "analyst.h"
#include "json_generator.h"
#include "proto_generator.h"
#include "cpp_generator.h"
#include "bin_generator.h"

namespace tpn {

namespace xlsx {

/// 生成器中枢
class TPN_XLSX2DATA_API GeneratorHub {
 public:
  /// 加载数据
  ///  @param[in]   path      配置文件相对路径
  ///  @param[out]  error     读取数据错误信息
  ///  @param[in]   reload    重新加载标志 默认false
  ///  @return 加载成功返回true
  bool Load(std::string_view path, std::string &error, bool reload = false);

  /// 重新加载数据
  ///  @param[out]  error     读取数据错误信息
  ///  @return 加载成功返回true
  bool Reload(std::string &error);

  /// 生成数据
  ///  @return 生成成功返回true
  bool Generate();

  /// 获取文件前缀
  ///  @return 文件前缀
  std::string_view GetFilePrefix() const;

  /// 获取分析器
  ///  @return 分析器
  Analyst &GetAnalyst();

 private:
  /// 生成分析器
  ///  @param[in]   worksheet     工作表
  ///  @return 成功返回true
  bool GenerateAnlayst(xlnt::worksheet &worksheet);

  /// 生成json
  ///  @param[in]   worksheet     工作表
  ///  @return 成功返回true
  bool GenerateJson(xlnt::worksheet &worksheet);

  /// 生成proto
  ///  @param[in]   worksheet     工作表
  ///  @return 成功返回true
  bool GenerateProto(xlnt::worksheet &worksheet);

  /// 生成cpp
  ///  @param[in]   worksheet     工作表
  ///  @return 成功返回true
  bool GenerateCpp(xlnt::worksheet &worksheet);

  /// 生成json文件
  ///  @return 成功返回true
  bool GenerateJsonFile();

  /// 生成cpp尾内容
  ///  @return 成功返回true
  bool GenerateCppTail();

  /// 生成protobuf bin文件
  ///  @return 成功返回true
  bool GeneraBin();

 private:
  std::string path_;                          ///< 数据文件夹路径
  std::vector<std::string> xlsx_file_paths_;  ///< 所有需要解析的数据路径
  std::string file_prefix_;                   ///< 文件前缀
  Analyst analyst_;                           ///< 分析器
  JsonGenerator json_gen_;                    ///< json生成器
  ProtoGenerator proto_gen_;                  ///< proto生成器
  CppGenerator cpp_gen_;                      ///< cpp生成器
  BinGenerator bin_gen_;                      ///< bin生成器

  TPN_SINGLETON_DECL(GeneratorHub)
};

}  // namespace xlsx

}  // namespace tpn

#define g_xlsx2data_generator tpn::xlsx::GeneratorHub::Instance()

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_GENERATOR_HUB_H_
