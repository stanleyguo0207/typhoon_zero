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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CRTP_OBJECT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CRTP_OBJECT_H_

#include <memory>

#include "define.h"
#include "debug_hub.h"
#include "exception_hub.h"

namespace tpn {

/// CRTP 机制底层对象 enable_shared_from_this版本
///  @tparam  Derived                     派生类
///  @tparam  enable_shared_from_this     是否需要延长生命周期标志
template <typename Derived, bool enable_shared_from_this = true>
class CRTPObject : public std::enable_shared_from_this<Derived> {
 protected:
  CRTPObject()  = default;
  ~CRTPObject() = default;

  inline const Derived &GetDerivedObj() const { return CRTP_CONST_CAST(this); }
  inline Derived &GetDerivedObj() { return CRTP_CAST(this); }

  inline std::shared_ptr<Derived> GetSelfSptr() {
    try {
      return this->GetDerivedObj().shared_from_this();
    } catch (const std::bad_weak_ptr &) {
      TPN_ASSERT(false, "std::bad_weak_ptr");
    } catch (const std::exception &) {
      TPN_ASSERT(false, "std::exception");
    }

    throw std::bad_weak_ptr{};
  }
};

/// CRTP 机制底层对象 非enable_shared_from_this版本
/// CRTPObject偏特化版本
///  @tparam  Derived                     派生类
template <typename Derived>
class CRTPObject<Derived, false> {
 protected:
  CRTPObject()  = default;
  ~CRTPObject() = default;

  inline const Derived &GetDerivedObj() const { return CRTP_CONST_CAST(this); }
  inline Derived &GetDerivedObj() { return CRTP_CAST(this); }

  inline std::shared_ptr<Derived> GetSelfSptr() {
    return std::shared_ptr<Derived>{};
  }
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CRTP_OBJECT_H_
