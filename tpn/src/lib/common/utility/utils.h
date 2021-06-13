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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_UTILS_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_UTILS_H_

#include <string>
#include <string_view>

#include "define.h"

namespace tpn {

/// utf8转utf16
///  @param[in]		utf8str		utf8字符串
///  @param[out]	wstr			utf16字符串
///  @return 成功返回true，异常返回false
TPN_COMMON_API bool Utf8ToWstr(std::string_view utf8strv, std::wstring &wstr);

/// utf8转utf16
///  @param[in]		utf8str		utf8字符串地址
///  @param[in]		utf8size	utf8字符串长度
///  @param[out]	wstr			utf16字符串地址
///  @param[out]	wsize			utf16字符串长度
///  @return 成功返回true，异常返回false
TPN_COMMON_API bool Utf8ToWstr(const char *utf8str, size_t utf8size,
                               wchar_t *wstr, size_t &wsize);

/// utf8转utf16
///  @param[in]		utf8str		utf8字符串
///  @param[out]	wstr			utf16字符串地址
///  @param[out]	wsize			utf16字符串长度
///  @return 成功返回true，异常返回false
TPN_COMMON_API bool Utf8ToWstr(std::string_view utf8strv, wchar_t *wstr,
                               size_t &wsize);

/// utf16转utf8
///  @param[in]		wstr			utf16字符串
///  @param[out]	utf8str		utf8字符串
///  @return 成功返回true，异常返回false
TPN_COMMON_API bool WstrToUtf8(std::wstring_view wstrv, std::string &utf8str);

/// utf16转utf8
///  @param[in]		wstr			utf16字符串地址
///  @param[in]		wsize			utf16字符串长度
///  @param[out]	utf8str		utf8字符串
///  @return 成功返回true，异常返回false
TPN_COMMON_API bool WstrToUtf8(wchar_t *wstr, size_t wsize,
                               std::string &utf8str);

/// 获取utf8字符串长度
///  @param[out]	utf8str		utf8字符
///  @return 原utf8字符串长度。如果发生异常，原字符串将被清空，并且返回0
TPN_COMMON_API size_t Utf8Length(std::string &utf8str);

/// 截断utf8字符串
///  @param[out]	utf8str		utf8字符串
///  @param[in]		len				要截断的长度
TPN_COMMON_API void Utf8Truncate(std::string &utf8str, size_t len);

/// 枚举转换为底层类型
///  @tparam			T				枚举类型
///  @param[in]		value		枚举值
///  @return 枚举值的底层类型数据
template <typename T>
inline constexpr decltype(auto) EnumToUnderlyType(const T &value) {
  return static_cast<std::underlying_type_t<T>>(value);
}

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_UTILS_H_
