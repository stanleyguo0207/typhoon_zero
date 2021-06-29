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

#include "define.h"
#include "fmt_wrap.h"
#include "copyable_wrap.h"
#include "function_traits.h"

namespace tpn {

/// 初等模板识别为非std::string类型
template <typename, typename = std::void_t<>>
struct StringTraits : std::false_type {};

/// 特化模板识别为std::string类型
template <typename T>
struct StringTraits<
    T, std::void_t<typename T::value_type, typename T::traits_type,
                   typename T::allocator_type,
                   typename std::enable_if_t<std::is_same_v<
                       T, std::basic_string<typename T::value_type,
                                            typename T::traits_type,
                                            typename T::allocator_type>>>>>
    : std::true_type {};

/// 是否为std::string
template <typename T>
inline constexpr bool is_string_v = StringTraits<T>::value;

/// 初等模板识别为非std::string_view类型
template <typename, typename = std::void_t<>>
struct StringViewTraits : std::false_type {};

/// 初等模板识别为非std::string_view类型
template <typename T>
struct StringViewTraits<
    T, std::void_t<typename T::value_type, typename T::traits_type,
                   typename std::enable_if_t<std::is_same_v<
                       T, std::basic_string_view<typename T::value_type,
                                                 typename T::traits_type>>>>>
    : std::true_type {};

/// 是否为std::string_view
template <typename T>
inline constexpr bool is_string_view_v = StringViewTraits<T>::value;

/// 初等模板识别 非拷贝包裹
template <typename, typename = void>
struct CopyableTraits : std::false_type {};

/// 特化识别 拷贝包裹
///  @tparam T    可以拷贝包裹的类型
template <typename T>
struct CopyableTraits<
    T, std::void_t<typename T::value_type,
                   typename std::enable_if_t<std::is_same_v<
                       T, CopyableWrap<typename T::value_type>>>>>
    : std::true_type {};

/// 拷贝包裹判断
template <typename T>
inline constexpr bool is_copyable_wrapper_v = CopyableTraits<T>::value;

/// 判断是否为继承自fmt::compile_string
///  @tparam	FormatString		模式串
template <typename FormatString>
inline constexpr bool is_compile_string_v =
    fmt::is_compile_string<FormatString>::value;

/// 判断是否为继承自fmt::compiled_string
///  @tparam	FormatString		模式串
template <typename FormatString>
inline constexpr bool is_compiled_string_v =
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
inline constexpr bool is_buffer_has_construct_with_size_v =
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
inline constexpr bool is_buffer_has_max_size_v = BufferHasMaxSize<T>::value;

/// 基础模板识别无流操作的类
template <typename, typename, typename = void>
struct HasStreamOperator : std::false_type {};

/// 特化识别拥有流操作的类
template <typename L, typename R>
struct HasStreamOperator<L, R, std::void_t<decltype(L{} << R{})>>
    : std::true_type {};

/// 流操作符识别
template <typename L, typename R>
inline constexpr bool has_stream_operator_v = HasStreamOperator<L, R>::value;

/// 基础模板识别无赋值操作的类
template <typename, typename, typename = void>
struct HasEqualOperator : std::false_type {};

template <typename L, typename R>
struct HasEqualOperator<L, R, std::void_t<decltype(L{} = R{})>>
    : std::true_type {};

/// 赋值操作识别
template <typename L, typename R>
inline constexpr bool has_equal_operator_v = HasEqualOperator<L, R>::value;

/// 初等模板识别模板不可实例化
///  @tparam  T            模板类
///  @tparam  AlwaysVoid   空
///  @tparam  Args         模板类参数
template <template <typename...> typename T, typename AlwaysVoid,
          typename... Args>
struct TemplateInstantiable : std::false_type {};

/// 特化识别模板类可实例化
///  @tparam  T            模板类
///  @tparam  Args         模板类参数
template <template <typename...> typename T, typename... Args>
struct TemplateInstantiable<T, std::void_t<T<Args...>>, Args...>
    : std::true_type {};

/// 判断模板是否可以实例化
///  @tparam  T            模板类
///  @tparam  Args         模板类参数
/// @example
/// @code
///  is_template_instantiable_v<std::vector, double>;
///  is_template_instantiable_v<std::optional, int, int>;
/// @endcode
template <template <typename...> typename T, typename... Args>
inline constexpr bool is_template_instantiable_v =
    TemplateInstantiable<T, void, Args...>::value;

/// 初等模板识别T不是U的实例
///  @tparam  U    模板类型
///  @tparam  T    模板实例类型
template <template <typename...> typename U, typename T>
struct TemplateInstanceOf : std::false_type {};

/// 初等模板识别T是U的实例
///  @tparam  U    模板类型
///  @tparam  Args 模板参数
template <template <typename...> typename U, typename... Args>
struct TemplateInstanceOf<U, U<Args...>> : std::true_type {};

/// 识别Args...是否为U的实例
///  @tparam  U    模板类型
///  @tparam  Args 模板参数
/// @example
/// @code
///  is_template_instance_of_v<std::vector, std::vector<int>>;
/// @endcode
template <template <typename...> typename U, typename... Args>
inline constexpr bool is_template_instance_of_v =
    TemplateInstanceOf<U, Args...>::value;

/// 特化识别tuple类型
///  @tparam  T    模板类型
template <typename T>
struct TemplateInstanceOfTuple : TemplateInstanceOf<std::tuple, T> {};

/// 识别T是否为tuple类型
///  @tparam  T    模板类型
template <typename T>
inline constexpr bool is_tuple_v = TemplateInstanceOfTuple<T>::value;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_TRAITS_HUB_H_
