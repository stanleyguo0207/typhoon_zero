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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEFINE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEFINE_H_

#include <cstdint>
#include <cstddef>
#include <cinttypes>

// only windows or linux
#define TPN_PLATFORM_WIN 0
#define TPN_PLATFORM_UNIX 1

// 平台
#if defined(_WIN64) || defined(__WIN32__) || defined(_WIN32)
#  define TPN_PLATFORM TPN_PLATFORM_WIN
#else
#  define TPN_PLATFORM TPN_PLATFORM_UNIX
#endif

// only msvc or gnu
#define TPN_COMPILER_MSVC 0
#define TPN_COMPILER_GNU 1

// 编译器
#if defined(_MSC_VER)
#  define TPN_COMPILER TPN_COMPILER_MSVC
#elif defined(__GNUC__)
#  define TPN_COMPILER TPN_COMPILER_GNU
#  define GCC_VERSION \
    (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#  error "FATAL ERROR: compiler not supported."
#endif

// endian type
#define TPN_LITTLE_ENDIAN 0
#define TPN_BIG_ENDIAN 1

// 字节序
#if !defined(TPN_ENDIAN)
#  if defined(TPN_USE_BIG_ENDIAN)
#    define TPN_ENDIAN TPN_BIG_ENDIAN
#  else
#    define TPN_ENDIAN TPN_LITTLE_ENDIAN
#  endif
#endif

// gnu format macros
#if (TPN_COMPILER == TPN_COMPILER_GNU)
#  if !defined(__STDC_FORMAT_MACROS)
#    define __STDC_FORMAT_MACROS
#  endif
#  if !defined(__STDC_CONSTANT_MACROS)
#    define __STDC_CONSTANT_MACROS
#  endif
#  if !defined(_GLIBCXX_USE_NANOSLEEP)
#    define _GLIBCXX_USE_NANOSLEEP
#  endif
#endif

// inline
#if !defined(TPN_COREDEBUG)
#  define TPN_INLINE inline
#else
#  if !defined(TPN_DEBUG)
#    define TPN_DEBUG
#  endif
#  define TPN_INLINE
#endif

// 格式检查开关
#if !defined(TPN_FORMAT_CHECK_DISABLE)
#  define TPN_FORMAT_CHECK (true)
#else
#  define TPN_FORMAT_CHECK (false)
#endif

// 动态库
#if defined(TPN_API_USE_DYNAMIC_LINKING)
#  if (TPN_COMPILER == TPN_COMPILER_MSVC)
#    define TPN_API_EXPORT __declspec(dllexport)
#    define TPN_API_IMPORT __declspec(dllimport)
#  elif (TPN_COMPILER == TPN_COMPILER_GNU)
#    define TPN_API_EXPORT __attribute__((visibility("default")))
#    define TPN_API_IMPORT
#  else
#    error "FATAL ERROR: compiler not supported."
#  endif
#else
#  define TPN_API_EXPORT
#  define TPN_API_IMPORT
#endif

// common api
#if defined(TPN_API_EXPORT_COMMON)
#  define TPN_COMMON_API TPN_API_EXPORT
#else
#  define TPN_COMMON_API TPN_API_IMPORT
#endif

// proto api
#if defined(TPN_API_EXPORT_PROTO)
#  define TPN_PROTO_API TPN_API_EXPORT
#else
#  define TPN_PROTO_API TPN_API_IMPORT
#endif

// net api
#if defined(TPN_API_EXPORT_NET)
#  define TPN_NET_API TPN_API_EXPORT
#else
#  define TPN_NET_API TPN_API_IMPORT
#endif

// 行结尾符
#if !defined(TPN_EOL)
#  if (TPN_PLATFORM == TPN_PLATFORM_WIN)
#    define TPN_EOL "\r\n"
#  else
#    define TPN_EOL "\n"
#  endif
#endif

// we always use stdlib std::max/std::min, undefine some not C++ standard defines (Win API and some other platforms)
#ifdef max
#  undef max
#endif

#ifdef min
#  undef min
#endif

// no copyable
#define TPN_NO_COPYABLE(TypeName)      \
 public:                               \
  TypeName(const TypeName &) = delete; \
  TypeName &operator=(const TypeName &) = delete;

// no moveable
#define TPN_NO_MOVEABLE(TypeName) \
 public:                          \
  TypeName(TypeName &&) = delete; \
  TypeName &operator=(TypeName &&) = delete;

// no implicitable
#define TPN_NO_IMPLICITABLE(TypeName) \
 public:                              \
  TypeName() = delete;                \
  TPN_NO_COPYABLE(TypeName)           \
  TPN_NO_MOVEABLE(TypeName)

// default copy
#define TPN_DEFAULT_COPY(TypeName)      \
 public:                                \
  TypeName(const TypeName &) = default; \
  TypeName &operator=(const TypeName &) = default;

// default move
#define TPN_DEFAULT_MOVE(TypeName)          \
 public:                                    \
  TypeName(TypeName &&) noexcept = default; \
  TypeName &operator=(TypeName &&) noexcept = default;

// singleton declaration
// xxx.h
// class TypeName {
// TPN_SINGLETON_DECL(TypeName)
//   ...
// }
#define TPN_SINGLETON_DECL(TypeName) \
 public:                             \
  static TypeName *Instance();       \
                                     \
 private:                            \
  TypeName()  = default;             \
  ~TypeName() = default;             \
  TPN_NO_COPYABLE(TypeName)          \
  TPN_NO_MOVEABLE(TypeName)

// singleton implement
// xxx.cpp
// TPN_SINGLETON_IMPL(TypeName)
#define TPN_SINGLETON_IMPL(TypeName) \
  TypeName *TypeName::Instance() {   \
    static TypeName s_instance;      \
    return &s_instance;              \
  }

// std::remove_reference_t<TypeName>
#define TPN_RMRF(TypeName) std::remove_reference_t<TypeName>

// std::remove_cv_t<TypeName>
#define TPN_RMCV(TypeName) std::remove_cv_t<TypeName>

// std::remove_cv_t<std::remove_reference_t<TypeName>>
#define TPN_RMRFCV(TypeName) TPN_RMCV(TPN_RMRF(TypeName))

// crtp to derive static_cast<Derived&>(*this)
#define CRTP_CAST(This) static_cast<Derived &>(*This)

// crtp to const derive static_cast<const Derived&>(*this)
#define CRTP_CONST_CAST(This) static_cast<const Derived &>(*This)

// template <typename>
#define TEMPLATE_DECL_1 template <typename>

// template <typename, typename>
#define TEMPLATE_DECL_2 template <typename, typename>

// template <typename, bool>
#define TEMPLATE_DECL_2_BOOL template <typename, bool>

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEFINE_H_
