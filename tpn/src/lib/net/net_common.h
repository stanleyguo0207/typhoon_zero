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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_COMMON_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_COMMON_H_

#include "net_define.h"

#include "asio_wrap.h"
#include "log.h"

namespace tpn {

namespace net {

/// 网络状态
enum class NetState {
  kNetStateStopped = 0,  ///< 已停止
  kNetStateStoping,      ///< 停止中
  kNetStateStarting,     ///< 启动中
  kNetStateStarted,      ///< 已启动
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_COMMON_H_
