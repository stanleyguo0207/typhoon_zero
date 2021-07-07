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

// base
#define TPN_NET_BASE_CLASS_DECL(Keyword)       \
  TEMPLATE_DECL_2 Keyword Connect;             \
  TEMPLATE_DECL_2 Keyword Disconnect;          \
  TEMPLATE_DECL_2 Keyword EventQueue;          \
  TEMPLATE_DECL_2 Keyword EventQueueGuard;     \
  TEMPLATE_DECL_2 Keyword LocalEndpoint;       \
  TEMPLATE_DECL_2 Keyword AliveTime;           \
  TEMPLATE_DECL_2 Keyword ConnectTime;         \
  TEMPLATE_DECL_2 Keyword ConnectTimeoutTimer; \
  TEMPLATE_DECL_2 Keyword ReconnectTimer;      \
  TEMPLATE_DECL_2 Keyword SilenceTimer;        \
  TEMPLATE_DECL_2 Keyword UserTimer;           \
  TEMPLATE_DECL_2 Keyword PostWrap;            \
  TEMPLATE_DECL_2 Keyword SendWrap;            \
  TEMPLATE_DECL_2 Keyword Socket;              \
  TEMPLATE_DECL_1 Keyword SessionMgr;          \
  TEMPLATE_DECL_1 Keyword ServiceMgr;

// tcp base
#define TPN_NET_TCP_BASE_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword TcpKeepAlive;      \
  TEMPLATE_DECL_2 Keyword TcpRecv;           \
  TEMPLATE_DECL_2 Keyword TcpSendWrap;       \
  TEMPLATE_DECL_2_BOOL Keyword SslContext;   \
  TEMPLATE_DECL_2 Keyword SslStream;

// tcp client
#define TPN_NET_TCP_CLIENT_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword ClientBase;          \
  TEMPLATE_DECL_2 Keyword TcpClientBase;       \
  TEMPLATE_DECL_2 Keyword TcpSslClientBase;

// tcp server
#define TPN_NET_TCP_SERVER_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword ServerBase;          \
  TEMPLATE_DECL_2 Keyword TcpServerBase;       \
  TEMPLATE_DECL_2 Keyword TcpSslServerBase;

// tcp session
#define TPN_NET_TCP_SESSION_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword SessionBase;          \
  TEMPLATE_DECL_2 Keyword TcpSessionBase;       \
  TEMPLATE_DECL_2 Keyword TcpSslSessionBase;

// udp base
#define TPN_NET_UDP_BASE_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword KcpStream;         \
  TEMPLATE_DECL_2 Keyword UdpSendWrap;

// udp client
#define TPN_NET_UDP_CLIENT_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword ClientBase;          \
  TEMPLATE_DECL_2 Keyword UdpClientBase;

// udp server
#define TPN_NET_UDP_SERVER_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword ServerBase;          \
  TEMPLATE_DECL_2 Keyword UdpServerBase;

// udp session
#define TPN_NET_UDP_SESSION_CLASS_DECL(Keyword) \
  TEMPLATE_DECL_2 Keyword SessionBase;          \
  TEMPLATE_DECL_2 Keyword UdpSessionBase;

// clang-format off

// base class forward
#define TPN_NET_FORWARD_DECL_BASE_CLASS         TPN_NET_BASE_CLASS_DECL(class)

// base class friend
#define TPN_NET_FRIEND_DECL_BASE_CLASS          TPN_NET_BASE_CLASS_DECL(friend class)

// tcp class forward
#define TPN_NET_FORWARD_DECL_TCP_BASE_CLASS     TPN_NET_TCP_BASE_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_TCP_CLIENT_CLASS   TPN_NET_TCP_CLIENT_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS   TPN_NET_TCP_SERVER_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_TCP_SESSION_CLASS  TPN_NET_TCP_SESSION_CLASS_DECL(class)

// tcp class friend
#define TPN_NET_FRIEND_DECL_TCP_BASE_CLASS      TPN_NET_TCP_BASE_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_TCP_CLIENT_CLASS    TPN_NET_TCP_CLIENT_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS    TPN_NET_TCP_SERVER_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_TCP_SESSION_CLASS   TPN_NET_TCP_SESSION_CLASS_DECL(friend class)

// udp class forward
#define TPN_NET_FORWARD_DECL_UDP_BASE_CLASS     TPN_NET_UDP_BASE_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_UDP_CLIENT_CLASS   TPN_NET_UDP_CLIENT_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_UDP_SERVER_CLASS   TPN_NET_UDP_SERVER_CLASS_DECL(class)
#define TPN_NET_FORWARD_DECL_UDP_SESSION_CLASS  TPN_NET_UDP_SESSION_CLASS_DECL(class)

// udp class friend
#define TPN_NET_FRIEND_DECL_UDP_BASE_CLASS      TPN_NET_UDP_BASE_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_UDP_CLIENT_CLASS    TPN_NET_UDP_CLIENT_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_UDP_SERVER_CLASS    TPN_NET_UDP_SERVER_CLASS_DECL(friend class)
#define TPN_NET_FRIEND_DECL_UDP_SESSION_CLASS   TPN_NET_UDP_SESSION_CLASS_DECL(friend class)

// clang-format on

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

/// 协议头长度固定2字节
static constexpr uint32_t kHeaderBytes = 2;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_COMMON_H_
