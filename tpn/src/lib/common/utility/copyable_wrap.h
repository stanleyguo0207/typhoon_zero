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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_COPYABLE_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_COPYABLE_WRAP_H_

#include "define.h"

namespace tpn {

/// 拷贝包裹
template <typename T>
class CopyableWrap {
 public:
  using value_type = T;

  template <typename... Args>
  CopyableWrap(Args &&...args) : raw_(std::forward<Args>(args)...) {}

  template <typename = void>
  CopyableWrap(T &&other) : raw_(std::move(other)) {}

  CopyableWrap(const CopyableWrap &other) : raw_(const_cast<T &&>(other.raw_)) {
    throw 0;
  }
  CopyableWrap &operator=(const CopyableWrap &other) {
    raw_ = const_cast<T &&>(other.raw_);
    throw 0;
  }

  T &operator()() noexcept { return raw_; }

 protected:
  T raw_;  ///<  原始数据类型

  TPN_DEFAULT_MOVE(CopyableWrap)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_COPYABLE_WRAP_H_
