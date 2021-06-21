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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_TRAITS_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_TRAITS_HUB_H_

#include <type_traits>

#include "fmt_wrap.h"
#include "define.h"

namespace tpn {

/// 判断是否为继承自fmt::compile_string
///  @tparam	FormatString		模式串
template <typename FormatString>
TPN_INLINE constexpr bool is_compile_string_v =
    fmt::is_compile_string<FormatString>::value;

/// 判断是否为继承自fmt::compiled_string
///  @tparam	FormatString		模式串
template <typename FormatString>
TPN_INLINE constexpr bool is_compiled_string_v =
    fmt::detail::is_compiled_string<FormatString>::value;

/// 初等模板识别一个size_t构造函数的缓冲区类型
template <typename, typename = std::void_t<>>
struct BufferHasConstructWithSize : std::false_type {};

/// 特化识别一个size_t构造函数的缓冲区类型
template <typename T>
struct BufferHasConstructWithSize<T, std::void_t<decltype(T(size_t(0)))>>
    : std::true_type {};

/// 是否带有一个size_t构造函数类
template <typename T>
TPN_INLINE constexpr bool is_buffer_has_construct_with_size_v =
    BufferHasConstructWithSize<T>::value;

/// 初等模板识别是否带有max_size()函数的缓冲区类型
template <typename, typename = std::void_t<>>
struct BufferHasMaxSize : std::false_type {};

/// 特化识别带有max_size()函数的缓冲区类型
template <typename T>
struct BufferHasMaxSize<T, std::void_t<decltype(std::declval<T>().max_size())>>
    : std::true_type {};

/// 是否带有一个max_size()函数的类
template <typename T>
TPN_INLINE constexpr bool is_buffer_has_max_size_v = BufferHasMaxSize<T>::value;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_TRAITS_HUB_H_
