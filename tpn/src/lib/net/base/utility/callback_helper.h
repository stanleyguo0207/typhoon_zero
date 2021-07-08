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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CALLBACK_HELPER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CALLBACK_HELPER_H_

#include "net_common.h"

namespace tpn {

namespace net {

/// 回调辅助类
struct CallbackHelper {
  /// 处理Func(size_t)
  ///  @tparam      Func          发送回调函数类型
  ///  @param[in]   func          发送回调函数
  ///  @param[in]   bytes_sent    发送了多少字节
  template <typename Func>
  static TPN_INLINE std::enable_if_t<std::is_same_v<
      decltype(std::declval<Func>()(size_t(0)), std::true_type()),
      std::true_type>>
  Call(Func &func, size_t bytes_sent) {
    func(bytes_sent);
  }

  /// 处理Func()
  ///  @tparam      Func          发送回调函数类型
  ///  @param[in]   func          发送回调函数
  template <typename Func>
  static TPN_INLINE std::enable_if_t<std::is_same_v<
      decltype(std::declval<Func>()(), std::true_type()), std::true_type>>
  Call(Func &func, size_t /* bytes_sent */) {
    func();
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CALLBACK_HELPER_H_
