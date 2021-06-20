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
/// 因为clang暂时不支持c++2a的std::source_location,
/// 虽然GCC11已经支持，但是错误诊断过不去，暂时使用这个版本替代
/// 接口与std::source_location保持一致
struct SrcLocInfo {
  constexpr SrcLocInfo() = default;
  /// 构造函数
  ///  @param[in]		file_name_in			文件名
  ///  @param[in]		function_name_in	函数名
  ///  @param[in]		line_in						行号
  ///  @param[in]		column_in					列号
  constexpr SrcLocInfo(const char *file_name_in, const char *function_name_in,
                       std::uint_least32_t line_in,
                       std::uint_least32_t column_in = 0)
      : file_name_(file_name_in),
        function_name_(function_name_in),
        line_(line_in),
        column_(column_in) {}

  /// 获取文件名
  ///  @return 文件名
  constexpr const char *file_name() const noexcept {
    return file_name_ ? file_name_ : "";
  }

  /// 获取函数名
  ///  @return 函数名
  constexpr const char *function_name() const noexcept {
    return function_name_ ? function_name_ : "";
  }

  /// 获取行号
  ///  @return 行号
  constexpr std::uint_least32_t line() const noexcept { return line_; }

  /// 获取列号
  ///  @return 列号
  constexpr std::uint_least32_t column() const noexcept { return column_; }

 private:
  const char *file_name_{nullptr};      ///< 文件名
  const char *function_name_{nullptr};  ///< 函数名
  std::uint_least32_t line_{0};         ///< 行号
  std::uint_least32_t column_{0};       ///< 列号
};

/// buffer容量默认值
constexpr size_t kBufferDefaultSize = 512;

}  // namespace tpn

// 为将来兼容std::source_location做扩展
using SourceLocation = tpn::SrcLocInfo;

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_COMMON_H_
