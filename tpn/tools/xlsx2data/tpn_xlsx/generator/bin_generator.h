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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_BIN_GENERATOR_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_BIN_GENERATOR_H_

#include "xlsx2data_common.h"

namespace tpn {

namespace xlsx {

/// bin文件生成器
class TPN_XLSX2DATA_API BinGenerator {
 public:
  BinGenerator();
  ~BinGenerator();

  /// 生成数据
  ///  @return 生成成功返回true
  bool Generate();
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_BIN_GENERATOR_H_
