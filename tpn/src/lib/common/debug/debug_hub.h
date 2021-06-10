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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEBUG_DEBUG_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEBUG_DEBUG_HUB_H_

#include "common.h"
#include "fmt_wrap.h"

namespace tpn {

void DBGLog(const char *tag, SrcLocInfo src_loc, const char *cond,
            fmt::string_view fmt, fmt::format_args args) {
  fmt::print(stderr, "\n{}:{} in {} {}:\ncond:{}\n", src_loc.filename,
             src_loc.line, src_loc.funcname, tag, cond);
  fmt::vprint(stderr, fmt, args);
  fmt::print(stderr, "\n");
}

/// 警告信息不会触发crash
///  @param[in]		src_loc	触发警告定位信息
///  @param[in]		cond		触发警告条件
///  @param[in]		format	额外携带信息格式
///  @param[in]		...			额外携带信息参数
template <typename FormatString, typename... Args>
TPN_INLINE void DBGWarning(SrcLocInfo src_loc, const char *cond,
                           const FormatString &fmt, Args &&...args) {
  DBGLog("WARNING", src_loc, cond, fmt,
         fmt::make_args_checked<Args...>(fmt, args...));
}

/// 断言信息会触发crash
/// 此函数一旦出发就不会返回
///  @param[in]		src_loc	触发断言定位信息
///  @param[in]		cond		触发断言条件
///  @param[in]		format	额外携带信息格式
///  @param[in]		...			额外携带信息参数
template <typename FormatString, typename... Args>
[[noreturn]] TPN_INLINE void DBGAssert(SrcLocInfo src_loc, const char *cond,
                                       const FormatString &fmt,
                                       Args &&...args) {
  DBGLog("ASSERTION FAILED", src_loc, cond, fmt,
         fmt::make_args_checked<Args...>(fmt, args...));
  *((volatile int *)nullptr) = 0;
  exit(1);
}

/// 信号中止处理函数
/// 此函数一旦出发就不会返回
///  @param[in]		signal	信号 例如 SIGABRT等
[[noreturn]] TPN_COMMON_API void AbortHandler(int signal);

}  // namespace tpn

#if (TPN_COMPILER_MSVC == TPN_COMPILER)
#  define ASSERT_BEGIN __pragma(warning(push)) __pragma(warning(disable : 4127))
#  define ASSERT_END __pragma(warning(pop))
#else
#  define ASSERT_BEGIN
#  define ASSERT_END
#endif

#define TPN_WARNING(cond, format, ...)                                   \
  ASSERT_BEGIN do {                                                      \
    if (!(cond)) {                                                       \
      tpn::DBGWarning(tpn::SrcLocInfo{__FILE__, __LINE__, __FUNCTION__}, \
                      #cond, FMT_STRING(format), __VA_ARGS__);           \
    }                                                                    \
  }                                                                      \
  while (0) ASSERT_END

#define TPN_ASSERT(cond, format, ...)                                          \
  ASSERT_BEGIN do {                                                            \
    if (!(cond)) {                                                             \
      tpn::DBGAssert(tpn::SrcLocInfo{__FILE__, __LINE__, __FUNCTION__}, #cond, \
                     FMT_STRING(format), __VA_ARGS__);                         \
    }                                                                          \
  }                                                                            \
  while (0) ASSERT_END

template <typename T>
inline decltype(auto) ASSERT_NOTNULL_IMPL(T *ptr, std::string_view expr) {
  TPN_ASSERT(ptr, "{}", expr);
  return ptr;
}

#define TPN_ASSERT_NOTNULL(ptr) ASSERT_NOTNULL_IMPL(ptr, #ptr)

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_DEBUG_DEBUG_HUB_H_
