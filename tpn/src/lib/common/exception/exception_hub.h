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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_EXCEPTION_EXCEPTION_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_EXCEPTION_EXCEPTION_HUB_H_

#include <exception>
#include <stdexcept>

#include "define.h"
#include "debug_hub.h"

namespace tpn {

/// 异常内部父类
class TPN_COMMON_API TpnException : public std::exception {
 public:
  ~TpnException() noexcept = default;

  /// 异常信息
  /// 重载标准库异常
  ///  @return 异常信息
  const char *what() const noexcept override { return msg_.c_str(); }

 protected:
  /// 异常信息句柄
  ///  @return 异常信息句柄
  std::string &Message() { return msg_; }

 protected:
  std::string msg_;  ///< 异常信息
};

/// 文件异常
class TPN_COMMON_API FileException final : public TpnException {
 public:
  /// 异常信息
  ///  @param[in]		msg			异常信息
  FileException(std::string_view msg);
  ~FileException() noexcept final = default;
};

#if defined(TPN_NO_EXCEPTION)
#  define TPN_THROW(ex)             \
    do {                            \
      TPN_ASSERT(false, ex.what()); \
    } while (0)
#else
#  define TPN_THROW(ex)              \
    do {                             \
      TPN_WARNING(false, ex.what()); \
      throw(ex);                     \
    } while (0)
#endif

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_EXCEPTION_EXCEPTION_HUB_H_
