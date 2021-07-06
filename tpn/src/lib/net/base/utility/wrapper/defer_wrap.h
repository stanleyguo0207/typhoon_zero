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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_DEFER_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_DEFER_WRAP_H_

#include <functional>

#include "net_common.h"

namespace tpn {

namespace net {

/// 延迟任务包裹
/// 析构时自动调用注册的延迟任务
class DeferWrap {
 public:
  DeferWrap() = default;

  ~DeferWrap() noexcept {
    if (this->task_) {
      (this->task_)();
    }
  }

  /// 构造函数
  ///  @tparam      Func    任务函数类型
  ///  @tparam      Args... 任务函数所需参数类型
  ///  @param[in]   func    任务函数
  ///  @param[in]   args... 任务函数所需参数
  template <typename Func, typename... Args>
  explicit DeferWrap(Func &&func, Args &&...args) {
    this->task_ =
        std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
  }

 protected:
  std::function<void()> task_;  ///< 包裹任务

  TPN_NO_COPYABLE(DeferWrap)
  TPN_DEFAULT_MOVE(DeferWrap)
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_DEFER_WRAP_H_
