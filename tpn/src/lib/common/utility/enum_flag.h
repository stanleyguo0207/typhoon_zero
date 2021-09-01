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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_ENUM_FLAG_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_ENUM_FLAG_H_

#include <type_traits>

#include "define.h"

template <typename T,
          std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
TPN_INLINE constexpr T operator&(T left, T right) {
  return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) &
                        static_cast<std::underlying_type_t<T>>(right));
}

template <typename T,
          std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
TPN_INLINE constexpr T &operator&=(T &left, T right) {
  return left = left & right;
}

template <typename T,
          std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
TPN_INLINE constexpr T operator|(T left, T right) {
  return static_cast<T>(static_cast<std::underlying_type_t<T>>(left) |
                        static_cast<std::underlying_type_t<T>>(right));
}

template <typename T,
          std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
TPN_INLINE constexpr T &operator|=(T &left, T right) {
  return left = left | right;
}

template <typename T,
          std::enable_if_t<std::is_enum_v<T>, std::nullptr_t> = nullptr>
TPN_INLINE constexpr T operator~(T value) {
  return static_cast<T>(~static_cast<std::underlying_type_t<T>>(value));
}

namespace tpn {

/// 枚举标志类
///  @tparam  T     T必须为枚举类型
template <typename T>
class EnumFlag {
  static_assert(std::is_enum_v<T>, "EnumFlag must be used only with enums");

 public:
  /// 构造函数
  ///  @param[in]   value     标记初始化的值
  constexpr EnumFlag(T value) : value_(value) {}

  /// 位与操作
  ///  @param[in]   other     要参与位于操作的标志
  ///  @return 位与操作后的标志
  constexpr EnumFlag &operator&=(EnumFlag other) {
    value_ &= other.value_;
    return *this;
  }

  /// 位与操作
  ///  @param[in]   left      参与位与的左操作数
  ///  @param[in]   right     参与位与的右操作数
  ///  @return 位与操作后的标志
  constexpr friend EnumFlag operator&(EnumFlag left, EnumFlag right) {
    return left &= right;
  }

  /// 位或操作
  ///  @param[in]   other     要参与位或操作的标志
  ///  @return 位或操作后的标志
  constexpr EnumFlag &operator|=(EnumFlag other) {
    value_ |= other.value_;
    return *this;
  }

  /// 位或操作
  ///  @param[in]   left      参与位或的左操作数
  ///  @param[in]   right     参与位或的右操作数
  ///  @return 位或操作后的标志
  constexpr friend EnumFlag operator|(EnumFlag left, EnumFlag right) {
    return left |= right;
  }

  /// 取反操作
  ///  @return 取反操作后的标志
  constexpr EnumFlag operator~() const {
    return static_cast<T>(~static_cast<std::underlying_type_t<T>>(value_));
  }

  /// 添加标志
  ///  @param[in]   flag      要添加的标志
  constexpr void AddFlag(EnumFlag flag) { value_ |= flag.value_; }

  /// 移除标志
  ///  @param[in]   flag      要移除的标志
  constexpr void RemoveFlag(EnumFlag flag) { value_ &= ~flag.value_; }

  /// 检查是否有指定标志
  ///  @param[in]   flag      要检查的标志
  ///  return 存在标志返回true
  constexpr bool HasFlag(T flag) const {
    using ut = std::underlying_type_t<T>;
    return static_cast<ut>(value_ & flag) != static_cast<ut>(0);
  }

  /// 检查是否有指定标志集合
  ///  @param[in]   flags     要检查的标志集合
  ///  return 存在标志集合返回true
  constexpr bool HasAllFlag(T flags) const { return (value_ & flags) == flags; }

  /// 类型转换
  constexpr operator T() const { return value_; }

  /// 使用底层类型返回
  constexpr std::underlying_type_t<T> AsUnderlyingType() const {
    return static_cast<std::underlying_type_t<T>>(value_);
  }

 private:
  T value_;  ///< 标志类底层数据
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_ENUM_FLAG_H_
