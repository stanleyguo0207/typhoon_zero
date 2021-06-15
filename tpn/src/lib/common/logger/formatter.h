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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_FORMATTER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_FORMATTER_H_

#include "log_common.h"
#include "log_msg.h"
#include "fmt_wrap.h"
#include "pattern.h"

/// 源文件定位信息格式化
template <>
struct fmt::formatter<SourceLocation> : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const SourceLocation &soc_loc, FormatContext &ctx) {
    return fmt::format_to(
        ctx.out(), "{file}:{line} {func}", "file"_a = soc_loc.file_name(),
        "line"_a = soc_loc.line(), "func"_a = soc_loc.function_name());
  }
};

/// 日志信息格式化
template <typename T>
struct fmt::formatter<
    T, std::enable_if_t<std::is_base_of<tpn::log::LogMsg, T>::value, char>>
    : fmt::formatter<std::string_view> {
  template <typename FormatContext>
  auto format(const tpn::log::LogMsg &msg, FormatContext &ctx) {
    return g_log_pattern->Format(msg, ctx);
  }
};

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_FORMATTER_H_
