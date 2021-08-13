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
#include <unordered_map>

namespace tpn {

namespace xlsx {

/// 分析字段
class TPN_XLSX2DATA_API AnalystField {
 public:
 private:
};

/// 分析表
class TPN_XLSX2DATA_API AnalystSheet {
 public:
 private:
  std::unordered_map<std::string, AnalystField> field_map_;  ///< 字段结构
};

/// 分析器
class TPN_XLSX2DATA_API Analyst {
 public:
 private:
  std::unordered_map<std::string, AnalystSheet> sheet_map_;  ///< 表结构
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_UTILITY_ANALYST_H_
