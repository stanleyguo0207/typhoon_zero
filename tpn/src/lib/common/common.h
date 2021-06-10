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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_COMMON_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_COMMON_H_

#include "define.h"

namespace tpn {

/// 源文件定位信息
struct SrcLocInfo {
  constexpr SrcLocInfo() = default;
  constexpr SrcLocInfo(const char *filename_in, int line_in,
                       const char *funcname_in)
      : filename(filename_in), line(line_in), funcname(funcname_in) {}

  [[nodiscard]] constexpr bool Empty() const noexcept { return (0 == line); }

  const char *filename{nullptr};
  int line{0};
  const char *funcname{nullptr};
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_COMMON_H_
