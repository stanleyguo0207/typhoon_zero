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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_

#include "message_buffer.h"
#include "rpc_type.pb.h"
#include "net_common.h"
#include "session.h"
#include "tcp_keepalive.h"
#include "tcp_recv.h"
#include "tcp_send_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS
TPN_NET_FORWARD_DECL_TCP_SESSION_CLASS

/// tcp网络会话模板参数
struct TemplateArgsTcpSession {
  static constexpr bool is_session = true;
  static constexpr bool is_client  = false;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

/// tcp网络会话基类
///  @tparam  Derived     tcp服务器子类
///  @tparam  ArgsType    网络会话模板参数
template <typename Derived, typename ArgsType>
class TcpSessionBase : public SessionBase<Derived, ArgsType>,
                       public TcpKeepAlive<Derived, ArgsType>,
                       public TcpRecv<Derived, ArgsType>,
                       public TcpSendWrap<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
  TPN_NET_FRIEND_DECL_TCP_SESSION_CLASS

 public:
  using key_type    = size_t;
  using buffer_type = typename ArgsType::buffer_type;

  using Super = SessionBase<Derived, ArgsType>;
  using Self  = TcpSessionBase<Derived, ArgsType>;

  using Super::Send;

  /// 构造函数
  ///  @param[in]   io_handle       io句柄
  ///  @param[in]   session_mgr     所在的会话管理器引用
  ///  @param[in]   buffer_max      缓冲区最大长度
  ///  @param[in]   buffer_prepare  缓冲区准备长度
  explicit TcpSessionBase(IoHandle &io_handle, SessionMgr<Derived> &session_mgr,
                          size_t buffer_max, size_t buffer_prepare)
      : Super(io_handle, session_mgr, buffer_max, buffer_prepare,
              io_handle.GetIoContext()),
        TcpKeepAlive<Derived, ArgsType>(this->socket_),
        TcpRecv<Derived, ArgsType>(),
        TcpSendWrap<Derived, ArgsType>(),
        rallocator_(),
        wallocator_() {
    this->SetSilenceTimeoutDuration(MilliSeconds(kTcpSilenceTimeout));
    this->SetConnectTimeoutDuration(MilliSeconds(kTcpConnectTimeout));
  }

  ~TcpSessionBase() = default;

  /// tcp会话关闭
  TPN_INLINE void Stop() {
    NET_DEBUG("TcpSessionBase Stop state {}", ToNetStateStr(this->state_));
    this->GetDerivedObj().DoDisconnect(asio::error::operation_aborted);
  }

  /// tcp会话哈希key
  TPN_INLINE const key_type GetHashKey() const {
    return reinterpret_cast<key_type>(this);
  }

 protected:
  /// tcp会话启动
  TPN_INLINE void Start() {
    NET_DEBUG("TcpSessionBase Start state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    try {
      NetState expected = NetState::kNetStateStopped;

      if (!this->state_.compare_exchange_strong(expected,
                                                NetState::kNetStateStarting)) {
        NET_ERROR("TcpSessionBase already starting state {}",
                  ToNetStateStr(this->state_));
        asio::detail::throw_error(asio::error::already_started);
      }

      std::shared_ptr<Derived> this_ptr = this->GetSelfSptr();

      // tcp session 启动
      this->GetDerivedObj().DoInit(this_ptr);

      // 通知接受器
      this->GetDerivedObj().FireAccept(this_ptr);

      expected = NetState::kNetStateStarting;
      if (!this->state_.compare_exchange_strong(expected,
                                                NetState::kNetStateStarting)) {
        NET_ERROR("TcpSessionBase not starting state {}",
                  ToNetStateStr(this->state_));
        asio::detail::throw_error(asio::error::already_started);
      }

      // 父类启动函数
      Super::Start();

      // 连接处理
      this->GetDerivedObj().HandleConnect(std::error_code{},
                                          std::move(this_ptr));

      NET_DEBUG("TcpSessionBase Start state {} key {} done",
                ToNetStateStr(this->state_), this->GetHashKey());
    } catch (std::system_error &e) {
      NET_ERROR("TcpSessionBase Start error {} key {}", e.code(),
                this->GetHashKey());

      SetLastError(e);

      this->GetDerivedObj().DoDisconnect(e.code());
    }
  }

  /// tcp会话初始化
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void DoInit(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase DoInit state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    IgnoreUnused(this_ptr);

    // 重置初始状态
    this->ResetConnectTime();
    // 更新存活时间
    this->UpdateAliveTime();

    // 设置默认保持连接选项
    this->SetKeepAliveOptions();
  }

  /// tcp开始接收数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void StartRecv(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase StartRecv state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    asio::post(
        this->GetIoHandle().GetStrand(),
        MakeAllocator(this->rallocator_,
                      [this, self_ptr = std::move(this_ptr)]() mutable {
                        NET_DEBUG("TcpSessionBase StartRecv PostRecv");
                        this->GetDerivedObj().GetBuffer().consume(
                            this->GetDerivedObj().GetBuffer().size());

                        this->GetDerivedObj().PostSilenceTimer(
                            this->silence_timeout_, self_ptr);

                        this->GetDerivedObj().PostRecv(std::move(self_ptr));
                      }));
  }

  /// tcp提交接收数据
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void PostRecv(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase PostRecv state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    this->GetDerivedObj().TcpPostRecv(std::move(this_ptr));
  }

  /// tcp处理接收数据
  ///  @param[in]   ec          错误码
  ///  @param[in]   bytes_recvd 收到的数据长度
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void HandleRecv(const std::error_code &ec, size_t bytes_recvd,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG(
        "TcpSessionBase HandleRecv state {} key {} error {} recv_bytes {}",
        ToNetStateStr(this->state_), this->GetHashKey(), ec, bytes_recvd);

    this->GetDerivedObj().TcpHandleRecv(ec, bytes_recvd, std::move(this_ptr));
  }

  /// 会话注册
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void JoinSession(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase JoinSession state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    this->session_mgr_.Emplace(this_ptr, [this,
                                          this_ptr](bool inserted) mutable {
      if (inserted) {
        NET_DEBUG("TcpSessionBase JoinSession {} state {} key {} StartRecv",
                  inserted, ToNetStateStr(this->state_), this->GetHashKey());
        this->GetDerivedObj().StartRecv(std::move(this_ptr));
      } else {
        NET_DEBUG("TcpSessionBase JoinSession {} state {} key {} DoDisconnect",
                  inserted, ToNetStateStr(this->state_), this->GetHashKey());
        this->GetDerivedObj().DoDisconnect(asio::error::address_in_use);
      }
    });
  }

  /// tcp会话启动处理
  TPN_INLINE void DoStart(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase DoStart state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    this->GetDerivedObj().JoinSession(std::move(this_ptr));
  }

  /// tcp会话关闭处理
  ///  @param[in]   ec          错误码
  TPN_INLINE void DoStop(const std::error_code &ec) {
    NET_DEBUG("TcpSessionBase DoStop state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    IgnoreUnused(ec);

    Super::Stop();

    // 调用底层套接字的关闭来通知 HandleRecv 函数响应，并且错误>0，则套接字得到通知退出
    // 调用Shutdown()，指示不会再向该套接字中写入数据
    this->socket_.lowest_layer().shutdown(asio::socket_base::shutdown_both,
                                          s_ec_ignore);
    this->socket_.lowest_layer().close(s_ec_ignore);
  }

  /// 处理断开连接
  /// 本函数重写了disconnect模块的HandleDisconnect的方法
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void HandleDisconnect(const std::error_code &ec,
                                   std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase HandleDisconnect state {} key {} error: {}",
              ToNetStateStr(this->state_), this->GetHashKey(), ec);
    IgnoreUnused(ec, this_ptr);

    this->GetDerivedObj().DoStop(ec);
  }

  /// 发送数据
  ///  @tparam      Callback  发送数据完回调类型
  ///  @param[in]   buffer    调用需要保证buffer满足底层拆包逻辑
  ///  @param[in]   callback  发送数据回调
  ///  @return 发送成功返回true
  template <typename Callback>
  TPN_INLINE bool DoSend(MessageBuffer &&buffer, Callback &&callback) {
    NET_DEBUG("TcpSessionBase DoSend state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    return this->GetDerivedObj().TcpSend(std::move(buffer),
                                         std::forward<Callback>(callback));
  }

  /// tcp会话通知接收数据
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   header      协议头
  ///  @param[in]   packet      协议体
  TPN_INLINE void [[maybe_unused]] FireRecv(std::shared_ptr<Derived> &this_ptr,
                                            protocol::Header &&header,
                                            MessageBuffer &&packet) {}

  /// tcp会话通知接收
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void
      [[maybe_unused]] FireAccept(std::shared_ptr<Derived> &this_ptr) {}

  /// tcp会话通知连接
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void
      [[maybe_unused]] FireConnect(std::shared_ptr<Derived> &this_ptr) {}

  /// tcp会话通知断开连接
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void
      [[maybe_unused]] FireDisconnect(std::shared_ptr<Derived> &this_ptr) {}

  /// 获取用于 recv/read 的内存分配器
  TPN_INLINE auto &GetReadAllocator() { return this->rallocator_; }

  /// 获取用于 send/write 的内存分配器
  TPN_INLINE auto &GetWriteAllocator() { return this->wallocator_; }

 protected:
  HandlerMemory<SizeOp<>, std::false_type>
      rallocator_;  ///< 用户自定义内存用来处理 acceptor
  HandlerMemory<SizeOp<>, std::true_type>
      wallocator_;  ///< 用户自定义内存用来处理 send/write/post
};

/// tcp会话
class TcpSession : public TcpSessionBase<TcpSession, TemplateArgsTcpSession> {
 public:
  using TcpSessionBase<TcpSession, TemplateArgsTcpSession>::TcpSessionBase;
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_
