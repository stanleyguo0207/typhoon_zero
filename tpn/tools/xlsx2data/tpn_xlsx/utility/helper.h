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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_HELPER_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_HELPER_H_

#include <string>
#include <string_view>

#include "xlsx2data_common.h"
#include "fmt_wrap.h"

namespace tpn {

namespace xlsx {

/// sheet表名是否需要输出
///  @return 需要输出返回true
TPN_XLSX2DATA_API bool SheetTitleIsOutput(std::string_view title);

/// 获取data_hub内树节点名称
///  @param[in]   name    名称
///  @return 树节点名称
TPN_XLSX2DATA_API std::string GetDataHubItemName(std::string_view name);

/// 获取data_hub内树结构名称
///  @param[in]   name    名称
///  @return 树结构名称
TPN_XLSX2DATA_API std::string GetDataHubMapName(std::string_view name);

/// 格式控制
class TPN_XLSX2DATA_API Printer {
 public:
  Printer();
  ~Printer();

  // 向内 =》
  void Indent();

  // 向外 《=
  void Outdent();

  // 重置
  void Reset();

  // 打印到缓冲区
  void Print(std::string_view strv);

  // 打印到缓冲区，后加换行符
  void Println(std::string_view strv);

  // 获取缓冲区
  FmtMemoryBuf &GetBuf();

 private:
  FmtMemoryBuf buf_;  ///< 数据缓冲区
  int space_{0};      ///< 空格
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_HELPER_H_
