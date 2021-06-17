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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_FMT_FMT_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_FMT_FMT_WRAP_H_

#include <fmt/chrono.h>
#include <fmt/color.h>
#include <fmt/core.h>
#include <fmt/format.h>
#include <fmt/ostream.h>
#include <fmt/printf.h>
#include <fmt/ranges.h>
#include <fmt/compile.h>
#include <fmt/locale.h>

namespace tpn {

using FmtStringView  = fmt::basic_string_view<char>;
using FmtWStringView = fmt::basic_string_view<wchar_t>;
using FmtMemoryBuf   = fmt::basic_memory_buffer<char, 250>;
using FmtWMemoryBuf  = fmt::basic_memory_buffer<wchar_t, 250>;

using namespace fmt::literals;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_FMT_FMT_WRAP_H_
