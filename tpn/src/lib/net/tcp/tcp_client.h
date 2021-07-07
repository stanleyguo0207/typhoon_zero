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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_

#include "message_buffer.h"
#include "rpc_type.pb.h"
#include "net_common.h"
#include "client.h"
#include "tcp_keepalive.h"
#include "tcp_recv.h"
#include "tcp_send_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_CLIENT_CLASS

/// tcp网络客户端模板参数
struct TemplateArgsTcpClient {
  static constexpr bool is_session = false;
  static constexpr bool is_client  = true;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

/// tcp网络客户端基类
///  @tparam  Derived     tcp客户端子类
///  @tparam  ArgsType    tcp网络客户端模板参数
template <typename Derived, typename ArgsType>
class TcpClientBase : public ClientBase<Derived, ArgsType>,
                      public TcpKeepAlive<Derived, ArgsType>,
                      public TcpRecv<Derived, ArgsType>,
                      public TcpSendWrap<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_CLIENT_CLASS

 public:
  using buffer_type = typename ArgsType::buffer_type;

  using Super = ClientBase<Derived, ArgsType>;
  using Self  = TcpClientBase<Derived, ArgsType>;

  using Super::Send;

  /// 构造函数
  ///  @param[in]   buffer_max      缓冲区最大长度 默认无限制
  ///  @param[in]   buffer_prepare  缓冲区准备长度 默认 @sa kTcpFrameSize
  explicit TcpClientBase(
      size_t buffer_max     = (std::numeric_limits<size_t>::max)(),
      size_t buffer_prepare = kTcpFrameSize)
      : Super(1, buffer_max, buffer_prepare),
        TcpKeepAlive<Derived, ArgsType>(this->socket_),
        TcpRecv<Derived, ArgsType>(),
        TcpSendWrap<Derived, ArgsType>() {
    this->SetConnectTimeoutDuration(MilliSeconds(kTcpConnectTimeout));
  };

  /// 析构函数
  ~TcpClientBase() { this->Stop(); }

  /// 启动tcp客户端
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <typename String, typename StrOrInt>
  bool Start(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase Start {}:{}", host, port);
    return this->GetDerivedObj().template DoConnect<false>(
        std::forward<String>(host), std::forward<StrOrInt>(port));
  }

  /// 异步启动tcp客户端
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <typename String, typename StrOrInt>
  bool AsyncStart(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase async Start {}:{}", host, port);
    return this->GetDerivedObj().template DoConnect<true>(
        std::forward<String>(host), std::forward<StrOrInt>(port));
  }

  /// tcp客户端关闭
  TPN_INLINE void Stop() {
    NET_DEBUG("TcpClientBase Stop");

    // Asio end socket functions: cancel, shutdown, close, release :
    // https://stackoverflow.com/questions/51468848/asio-end-socket-functions-cancel-shutdown-close-release
    // The proper steps are:
    // 1.Call shutdown() to indicate that you will not write any more data to the socket.
    // 2.Continue to (async-) read from the socket until you get either an error or the connection is closed.
    // 3.Now close() the socket (in the async read handler).
    // If you don't do this, you may end up closing the connection while the other side is still sending data.
    // This will result in an ungraceful close.
    this->GetDerivedObj().Post(
        [this]() mutable { this->StopReconnectTimer(); });

    this->GetDerivedObj().DoDisconnect(
        asio::error::operation_aborted, std::make_shared<DeferWrap>([this]() {
          NET_DEBUG("TcpClientBase Stop DoDisconnect DoStop");
          this->GetDerivedObj().DoStop(asio::error::operation_aborted);
        }));

    this->IoPoolStop();
  }

 protected:
  /// 启动tcp客户端
  ///  @tparam      IsAsync     异步启动为true
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <bool IsAsync, typename String, typename StrOrInt>
  bool DoConnect(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase DoConnect state {}", ToNetStateStr(this->state_));

    NetState expected = NetState::kNetStateStopped;
    if (!this->state_.compare_exchange_strong(expected,
                                              NetState::kNetStateStarting)) {
      NET_ERROR("TcpClientBase already starting state {}",
                ToNetStateStr(this->state_));
      SetLastError(asio::error::already_started);
      return false;
    }

    try {
      ClearLastError();

      // 启动对象池
      this->IoPoolStart();

      if (this->IsIoPoolStopped()) {
        NET_ERROR("TcpClientBase io_pool start error");
        SetLastError(asio::error::shut_down);
        return false;
      }

      // 加载重连定时器
      this->GetDerivedObj().LoadReconnectTimer(host, port);

      // 初始化
      this->GetDerivedObj().DoInit();

      // 基类启动
      Super::Start();

      return this->GetDerivedObj().template ClientStartConnect<IsAsync>(
          std::forward<String>(host), std::forward<StrOrInt>(port),
          this->GetSelfSptr());
    } catch (std::system_error &e) {
      NET_DEBUG("TcpClientBase DoConnect error {}", e.code());
      this->GetDerivedObj().HandleConnect(e.code(),
                                          this->GetDerivedObj().GetSelfSptr());
    }

    return false;
  }

  /// 初始化
  TPN_INLINE void DoInit() { NET_DEBUG("TcpClientBase DoInit"); }

  /// tcp开始接收数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void StartRecv(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase StartRecv state {}", ToNetStateStr(this->state_));

    this->GetDerivedObj().Post(
        [this, self_ptr = std::move(this_ptr)]() mutable {
          NET_DEBUG("TcpClientBase StartRecv PostRecv");
          this->GetDerivedObj().GetBuffer().consume(
              this->GetDerivedObj().GetBuffer().size());

          this->GetDerivedObj().PostRecv(std::move(self_ptr));
        });
  }

  /// tcp提交接收数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void PostRecv(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase PostRecv state {}", ToNetStateStr(this->state_));

    this->GetDerivedObj().TcpPostRecv(std::move(this_ptr));
  }

  /// tcp处理接收数据
  ///  @param[in]   ec          错误码
  ///  @param[in]   bytes_recvd 收到的数据长度
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void HandleRecv(const std::error_code &ec, size_t bytes_recvd,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase HandleRecv state {} error {} recv_bytes {}",
              ToNetStateStr(this->state_), ec, bytes_recvd);

    this->GetDerivedObj().TcpHandleRecv(ec, bytes_recvd, std::move(this_ptr));
  }

  /// 启动
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void DoStart(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase DoStart");
    this->UpdateAliveTime();
    this->ResetConnectTime();

    this->GetDerivedObj().StartRecv(std::move(this_ptr));
  }

  /// 关闭
  ///  @param[in]   ec    错误码
  TPN_INLINE void DoStop(const std::error_code &ec) {
    NET_DEBUG("TcpClientBase DoStop error {}", ec);
    this->GetDerivedObj().PostStop(ec, this->GetSelfSptr());
  }

  /// 提交关闭
  ///  @param[in]   ec          错误码
  ///  @param[in]   self_ptr    延长生命周期的智能指针
  TPN_INLINE void PostStop(const std::error_code &ec,
                           std::shared_ptr<Derived> self_ptr) {
    NET_DEBUG("TcpClientBase PostStop error {}", ec);
    auto task = [this, ec, this_ptr = std::move(self_ptr)](
                    EventQueueGuard<Derived> &&guard) mutable {
      NET_DEBUG("TcpClientBase PostStop task to handleStop");
      SetLastError(ec);

      // 父类关闭
      Super::Stop();

      // 处理关闭
      this->GetDerivedObj().HandleStop(ec, std::move(this_ptr));
    };

    NET_DEBUG("TcpClientBase PostStop event enqueue");
    this->GetDerivedObj().EventEnqueue(
        [this, t = std::move(task)](EventQueueGuard<Derived> &&guard) mutable {
          auto task = [guard = std::move(guard), t = std::move(t)]() mutable {
            t(std::move(guard));
          };

          NET_DEBUG("TcpClientBase PostStop event post task");
          this->GetDerivedObj().Post(std::move(task));
          return true;
        });
  }

  /// 处理关闭
  ///  @param[in]   ec          错误码
  ///  @param[in]   self_ptr    延长生命周期的智能指针
  TPN_INLINE void HandleStop(const std::error_code &ec,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase HandleStop error {}", ec);
    IgnoreUnused(ec, this_ptr);
  }

  /// 加载重连定时器
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  template <typename String, typename StrOrInt>
  void LoadReconnectTimer(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase LoadReconnectTimer {}:{}", host, port);

    this->GetDerivedObj().MakeReconnectTimer(
        this->GetSelfSptr(),
        [this, h = ToString(host), p = ToString(port)]() mutable {
          NET_DEBUG(
              "TcpClientBase LoadReconnectTimer MakeReconnectTimer state "
              "callback {}:{}",
              ToNetStateStr(this->state_), h, p);
          NetState expected = NetState::kNetStateStopped;
          if (this->state_.compare_exchange_strong(
                  expected, NetState::kNetStateStarting)) {
            // can't use h = std::move(h), p = std::move(p);
            // Otherwise, the value of h,p will be empty the next time the code goto here.
            auto task = [this, h, p](EventQueueGuard<Derived> &&guard) mutable {
              NET_DEBUG(
                  "TcpClientBase Reconnect Timer task to ClientStartConnect");
              this->GetDerivedObj().template ClientStartConnect<true>(
                  std::move(h), std::move(p), this->GetSelfSptr());
            };

            NET_DEBUG("TcpClientBase LoadReconnectTimer event enqueue");
            this->GetDerivedObj().EventEnqueue(
                [this, t = std::move(task)](
                    EventQueueGuard<Derived> &&guard) mutable {
                  auto task = [guard = std::move(guard),
                               t     = std::move(t)]() mutable {
                    t(std::move(guard));
                  };
                  NET_DEBUG("TcpClientBase LoadReconnectTimer post event");
                  this->GetDerivedObj().Post(std::move(task));
                  return true;
                });
          }
        });
  }

  /// 处理断开连接
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void HandleDisconnect(const std::error_code &ec,
                                   std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase HandleDisconnect error {}", ec);

    IgnoreUnused(ec, this_ptr);

    // 我们应该在 HandleDisconnect 函数中关闭套接字吗？
    // 否则当发送数据失败时，会导致DoDisconnect 函数被调用，然后导致自动重连执行，
    // 然后PostRecv 将返回一些错误，而PostRecv 将导致自动重连再次执行。

    // 调用socket的close函数通知HandleRecv函数响应error > 0，
    // 然后socket可以得到通知退出调用shutdown()表示不再往socket写入数据。
    this->socket_.lowest_layer().shutdown(asio::socket_base::shutdown_both,
                                          s_ec_ignore);
    // 调用close，否则HandleRecv永远不会返回
    this->socket_.lowest_layer().close(s_ec_ignore);
  }

  /// 发送数据
  ///  @tparam      Callback  发送数据完回调类型
  ///  @param[in]   buffer    调用需要保证buffer满足底层拆包逻辑
  ///  @param[in]   callback  发送数据回调
  ///  @return 发送成功返回true
  template <typename Callback>
  TPN_INLINE bool DoSend(MessageBuffer &&buffer, Callback &&callback) {
    NET_DEBUG("TcpClientBase DoSend");

    return this->GetDerivedObj().TcpSend(std::move(buffer),
                                         std::forward<Callback>(callback));
  }

  /// tcp客户端通知启动
  TPN_INLINE [[maybe_unused]] void FireInit() {}

  /// tcp客户端通知接收数据
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   header      协议头
  ///  @param[in]   packet      协议体
  TPN_INLINE [[maybe_unused]] void FireRecv(std::shared_ptr<Derived> &this_ptr,
                                            protocol::Header &&header,
                                            MessageBuffer &&packet) {}

  /// tcp客户端通知连接
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   ec          错误码
  TPN_INLINE [[maybe_unused]] void FireConnect(
      std::shared_ptr<Derived> &this_ptr, std::error_code ec) {}

  /// tcp客户端通知断开连接
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   ec          错误码
  TPN_INLINE [[maybe_unused]] void FireDisconnect(
      std::shared_ptr<Derived> &this_ptr, std::error_code ec) {}
};

/// tcp客户端
class TcpClient : public TcpClientBase<TcpClient, TemplateArgsTcpClient> {
 public:
  using TcpClientBase<TcpClient, TemplateArgsTcpClient>::TcpClientBase;
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_
