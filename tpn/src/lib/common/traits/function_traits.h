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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_FUNCTION_TRAITS_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_FUNCTION_TRAITS_H_

#include <functional>
#include <type_traits>
#include <tuple>

#include "define.h"

namespace tpn {

/// @li  function type                           ==> Ret(Args...)
/// @li  function pointer                        ==> Ret(*)(Args...)
/// @li  function reference                      ==> Ret(&)(Args...)
/// @li  pointer to no-static member function    ==> Ret(T::*)(Args...)
/// @li  function object and functor             ==> &T::operator()
/// @li  function with generic operator call     ==> template <typename... Args> &T::operator()
template <typename, typename = void>
struct FunctionTraits : std::false_type {};

template <typename Ret, typename... Args>
struct FunctionTraits<Ret(Args...)> : std::true_type {
 public:
  static constexpr std::size_t argc = sizeof...(Args);

  using function_type     = Ret(Args...);
  using return_type       = Ret;
  using stl_function_type = std::function<function_type>;
  using pointer           = Ret (*)(Args...);
  using class_type        = void;

  template <std::size_t I>
  struct args {
    static_assert(I < argc,
                  "index is out of range, index must less than sizeof Args");
    using type = typename std::tuple_element_t<I, std::tuple<Args...>>;
  };

  using tuple_type = std::tuple<Args...>;
  using pod_tuple_type =
      std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
};

template <typename Class, typename Ret, typename... Args>
struct FunctionTraits<Class, Ret(Args...)> : public std::true_type {
 public:
  static constexpr std::size_t argc = sizeof...(Args);

  using function_type     = Ret(Args...);
  using return_type       = Ret;
  using stl_function_type = std::function<function_type>;
  using pointer           = Ret (*)(Args...);
  using class_type        = Class;

  template <std::size_t I>
  struct args {
    static_assert(I < argc,
                  "index is out of range, index must less than sizeof Args");
    using type = typename std::tuple_element_t<I, std::tuple<Args...>>;
  };

  using tuple_type = std::tuple<Args...>;
  using pod_tuple_type =
      std::tuple<std::remove_cv_t<std::remove_reference_t<Args>>...>;
};

/// 常规函数指针
template <typename Ret, typename... Args>
struct FunctionTraits<Ret (*)(Args...)> : FunctionTraits<Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...)>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) &>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const &>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile &>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile &>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) &&>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const &&>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile &&>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile &&>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) &noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const &noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile &noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile &noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) &&noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const &&noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) volatile &&noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

template <typename Ret, typename Class, typename... Args>
struct FunctionTraits<Ret (Class::*)(Args...) const volatile &&noexcept>
    : FunctionTraits<Class, Ret(Args...)> {};

/// lambda函数
template <typename Callable>
struct FunctionTraits<Callable,
                      std::void_t<decltype(&Callable::operator()), char>>
    : FunctionTraits<decltype(&Callable::operator())> {};

/// std::function
template <typename Ret, typename... Args>
struct FunctionTraits<std::function<Ret(Args...)>>
    : FunctionTraits<Ret(Args...)> {};

/// 转化成std::function
template <typename Func>
typename FunctionTraits<Func>::stl_function_type ToStlFunction(Func &&lambda) {
  return static_cast<typename FunctionTraits<Func>::stl_function_type>(
      std::forward<Func>(lambda));
}

/// 转化为函数指针
template <typename Func>
typename FunctionTraits<Func>::pointer ToFunctionPointer(const Func &lambda) {
  return static_cast<typename FunctionTraits<Func>::pointer>(lambda);
}

/// 是否为可调用对象
template <typename Func>
inline constexpr bool is_callable_v = FunctionTraits<std::decay_t<Func>>::value;

template <typename Func, typename Void, typename... Args>
struct IsTemplateCallbale : std::false_type {};

template <typename Func, typename... Args>
struct IsTemplateCallbale<
    Func,
    std::void_t<decltype(std::declval<std::decay_t<Func> &>()(
                    (std::declval<Args>())...)),
                char>,
    Args...> : std::true_type {};

/// 是否为模板可调用对象
template <typename Func, typename... Args>
inline constexpr bool is_template_callbale_v =
    IsTemplateCallbale<Func, void, Args...>::value;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_TRAITS_FUNCTION_TRAITS_H_
