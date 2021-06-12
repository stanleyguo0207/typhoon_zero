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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_CONVERTER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_CONVERTER_H_

#include <bit>
#include <algorithm>

namespace tpn {

namespace byte_converter {

template <size_t T>
inline void Convert(char *val) {
  std::swap(*val, *(val + T - 1));
  Convert<T - 2>(val + 1);
}

template <>
inline void Convert<0>(char *) {}

template <>
inline void Convert<1>(char *) {}

template <typename T>
inline void Apply(T *val) {
  Convert<sizeof(T)>(reinterpret_cast<char *>(val));
}

}  // namespace byte_converter

template <typename T>
inline void EndianRefMakeLittle(T &val) {
  if constexpr (std::endian::native == std::endian::big) {
    byte_converter::Apply<T>(&val);
  }
}

template <typename T>
inline void EndianRefMakeBig(T &val) {
  if constexpr (std::endian::native == std::endian::little) {
    byte_converter::Apply<T>(&val);
  }
}

template <typename T>
inline void EndianPtrMakeLittle(void *val) {
  if constexpr (std::endian::native == std::endian::big) {
    byte_converter::Apply<T>(val);
  }
}

template <typename T>
inline void EndianPtrMakeBig(void *val) {
  if constexpr (std::endian::native == std::endian::little) {
    byte_converter::Apply<T>(val);
  }
}

/// 不允许对指针类型进行转换 会引发一个链接错误
template <typename T>
void EndianRefMakeLittle(T *);

/// 不允许对指针类型进行转换 会引发一个链接错误
template <typename T>
void EndianRefMakeBig(T *);

inline void EndianRefMakeLittle(uint8_t &) {}
inline void EndianRefMakeLittle(int8_t &) {}
inline void EndianRefMakeBig(uint8_t &) {}
inline void EndianRefMakeBig(int8_t &) {}

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_CONVERTER_H_
