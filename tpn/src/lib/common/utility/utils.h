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

#include <vector>
#include <string>
#include <string_view>

#include "define.h"
#include "traits_hub.h"

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
TPN_INLINE constexpr decltype(auto) EnumToUnderlyType(const T &value) {
  return static_cast<std::underlying_type_t<T>>(value);
}

/// 去除左边空格
///  @param[out]	src			操作字符串
///  @return 去除空格后的字符串
TPN_COMMON_API std::string &TrimLeft(std::string &src);

/// 去除左边空格
///  @param[out]	src			操作字符串
///  @return 去除空格后的字符串
TPN_COMMON_API std::string &TrimRight(std::string &src);

/// 去除左边空格
///  @param[out]	src			操作字符串
///  @return 去除空格后的字符串
TPN_COMMON_API std::string &Trim(std::string &src);

/// 分词
///  @param[in]		input					模式串
///  @param[in]		delimiters		分隔串
TPN_COMMON_API std::vector<std::string> Tokenize(std::string_view input,
                                                 std::string_view delimiters);

/// 分词器
class TPN_COMMON_API Tokenizer {
 public:
  using storage_type    = std::vector<const char *>;
  using size_type       = storage_type::size_type;
  using const_iterator  = storage_type::const_iterator;
  using reference       = storage_type::reference;
  using const_reference = storage_type::const_reference;

  /// 构造函数
  ///  @param[in]		src									模式串
  ///  @param[in]		delimiter						分割符
  ///  @param[in]		reserve							预留大小
  ///  @param[in]		keep_empty_strings	是否保留空字符串
  explicit Tokenizer(std::string_view src, char delimiter, uint32_t reserve = 0,
                     bool keep_empty_strings = true);
  ~Tokenizer() { delete[] str_; }

  const_iterator begin() const { return storage_.begin(); }

  const_iterator end() const { return storage_.end(); }

  size_type size() const { return storage_.size(); }

  reference operator[](size_type index) { return storage_[index]; }

  const_reference operator[](size_type index) const { return storage_[index]; }

 private:
  char *str_{nullptr};    ///< 源字符串
  storage_type storage_;  ///< 分词后存储器

  TPN_NO_IMPLICITABLE(Tokenizer)
};

/// 转化为std::string
template <typename T>
TPN_INLINE std::string ToString(T &&v) {
  using type = TPN_RMRFCV(T);
  std::string s;
  if constexpr (is_string_view_v<type>) {
    s = {v.data(), v.size()};
  } else if constexpr (std::is_integral_v<type>) {
    s = std::to_string(v);
  } else if constexpr (std::is_pointer_v<type>) {
    if (v) {
      s = v;
    }
  } else if constexpr (std::is_array_v<type>) {
    s = std::forward<T>(v);
  } else {
    s = std::forward<T>(v);
  }
  return s;
}

/// 转化为整型
template <typename IntegerType, typename T>
TPN_INLINE IntegerType ToInteger(T &&v) {
  using type = TPN_RMRFCV(T);
  if constexpr (std::is_integral_v<type>) {
    return static_cast<IntegerType>(v);
  } else {
    return static_cast<IntegerType>(std::stoull(ToString(std::forward<T>(v))));
  }
}

template <typename... Ts>
inline constexpr void IgnoreUnused(const Ts &...) {}

template <typename... Ts>
inline constexpr void IgnoreUnused() {}

/// 完美转发lambda标识
template <typename Func, typename... Args>
decltype(auto) ForwardAsLambda(Func &&func, Args &&...args) {
  return [func = std::forward<decltype(func)>(func),
          tup  = std::forward_as_tuple(args...)]() mutable {
    return std::apply(std::move(func), std::move(tup));
  };
}

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_UTILS_H_
