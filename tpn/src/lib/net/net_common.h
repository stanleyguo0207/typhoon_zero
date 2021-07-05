//
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

#include <string_view>

#include "define.h"
#include "asio_wrap.h"
#include "log.h"
#include "net_error.h"

namespace tpn {

namespace net {

/// 网络状态
enum class NetState : uint8_t {
  kNetStateStopped = 0,  ///< 已停止
  kNetStateStopping,     ///< 停止中
  kNetStateStarting,     ///< 启动中
  kNetStateStarted,      ///< 已启动
};

/// 监听器网络事件
enum class ListenerEvent : uint8_t {
  kListenerEventRecv = 0,    ///< 接收消息
  kListenerEventConnect,     ///< 连接
  kListenerEventDisconnect,  ///< 断开连接
  kListenerEventAccept,      ///< 接收
  kListenerEventHandshake,   ///< 握手
  kListenerEventUpgrade,     ///< 升级
  kListenerEventInit,        ///< 初始化
  kListenerEventStart,       ///< 开始
  kListenerEventStop,        ///< 结束

  kListenerEventMax,  // 事件可用数量
};

/// 转换成网络状态
TPN_NET_API std::string_view ToNetStateStr(NetState state) noexcept;

/// 转换成网络事件
TPN_NET_API std::string_view ToListenerEventStr(ListenerEvent event) noexcept;

/// tcp握手默认超时时长 5 * 1000
static constexpr long kTcpHandshakeTimeout = 5000;
/// udp握手默认超时时长 5 * 1000
static constexpr long kUdpHandshakeTimeout = 5000;
/// http握手默认超时时长 5 * 1000
static constexpr long kHttpHandshakeTimeout = 5000;

/// tcp连接超时时长 5 * 1000
static constexpr long kTcpConnectTimeout = 5000;
/// udp连接超时时长 5 * 1000
static constexpr long kUdpConnectTimeout = 5000;
/// http连接超时时长 5 * 1000
static constexpr long kHttpConnectTimeout = 5000;

/// tcp静默超时时长 60 * 60 * 1000
static constexpr long kTcpSilenceTimeout = 3600000;
/// udp静默超时时长 60 * 1000
static constexpr long kUdpSilenceTimeout = 60000;
/// http静默超时时长 85 * 1000
static constexpr long kHttpSilenceTimeout = 85000;

/// http执行超时时长 5 * 1000
static constexpr long kHttpExecuteTimeout = 5000;

/// ssl关闭超时 5 * 1000
static constexpr long kSslShutdownTimeout = 5000;
/// websocket关闭超时 5 * 1000
static constexpr long kWsShutdownTimeout = 5000;

/// ssl握手超时 5 * 1000
static constexpr long kSslHandshakeTimeout = 5000;
/// websocket握手超时 5 * 1000
static constexpr long kWsHandshakeTimeout = 5000;

/// tcp帧大小
static constexpr size_t kTcpFrameSize = 1536;
/// udp帧大小
static constexpr size_t kUdpFrameSize = 1024;
/// http帧大小
static constexpr size_t kHttpFrameSize = 1536;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_COMMON_H_
