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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_JSON_GENERATOR_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_JSON_GENERATOR_H_

#include <string_view>
#include <rapidjson/document.h>

#include "file_helper.h"

#include "xlsx2data_common.h"

namespace tpn {

namespace xlsx {

/// json文件生成器
class TPN_XLSX2DATA_API JsonGenerator {
 public:
  JsonGenerator();
  ~JsonGenerator();

  /// 加载配置
  ///  @param[out]  error     读取数据错误信息
  ///  @return 加载成功返回true
  bool Load(std::string &error);

  /// 分析数据
  ///  @param[in]   worksheet         工作表
  ///  @return 成功返回true
  bool Analyze(xlnt::worksheet &worksheet);

  /// 生成数据
  ///  @return 成功返回true
  bool Generate();

 private:
  rapidjson::Document document_;  ///< json数据
  FileHelper json_file_;          ///< json文件句柄
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_JSON_GENERATOR_H_
