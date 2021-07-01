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

#include "net_common.h"
#include "session.h"
#include "tcp_keepalive.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS
TPN_NET_FORWARD_DECL_TCP_SESSION_CLASS

struct TemplateArgsTcpSession {
  static constexpr bool is_session = true;
  static constexpr bool is_client  = false;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

template <typename Derived, typename ArgsType>
class TcpSessionBase : public SessionBase<Derived, ArgsType>,
                       public TcpKeepAlive<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
  TPN_NET_FRIEND_DECL_TCP_SESSION_CLASS

 public:
  using key_type    = size_t;
  using buffer_type = typename ArgsType::buffer_type;

  using Super = SessionBase<Derived, ArgsType>;
  using Self  = TcpSessionBase<Derived, ArgsType>;

  explicit TcpSessionBase(IoHandle &io_handle, Listener &listener,
                          SessionMgr<Derived> &session_mgr, size_t buffer_max,
                          size_t buffer_prepare)
      : Super(io_handle, listener, session_mgr, buffer_max, buffer_prepare,
              io_handle.GetIoContext()),
        TcpKeepAlive<Derived, ArgsType>(this->socket_),
        rallocator_(),
        wallocator_() {
    this->SetSilenceTimeout(MilliSeconds(kTcpSilenceTimeout));
    this->SetConnectTimeout(MilliSeconds(kTcpConnectTimeout));
  }

  ~TcpSessionBase() = default;

  TPN_INLINE void Stop() {
    NET_DEBUG("TcpSessionBase Stop state {}", ToNetStateStr(this->state_));
    this->GetDerivedObj().DoDisconnect(asio::error::operation_aborted);
  }

  TPN_INLINE const key_type GetHashKey() const {
    return reinterpret_cast<key_type>(this);
  }

 protected:
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
      // this->GetDerivedObj().FireAccept(this_ptr);

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

  TPN_INLINE void JoinSession(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase JoinSession state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    this->session_mgr_.Emplace(this_ptr, [this,
                                          this_ptr](bool inserted) mutable {
      if (inserted) {
        NET_DEBUG("TcpSessionBase JoinSession {} state {} key {} StartRecv",
                  inserted, ToNetStateStr(this->state_), this->GetHashKey());
        // this->GetDerivedObj().StartRecv(std::move(this_ptr));
      } else {
        NET_DEBUG("TcpSessionBase JoinSession {} state {} key {} DoDisconnect",
                  inserted, ToNetStateStr(this->state_), this->GetHashKey());
        this->GetDerivedObj().DoDisconnect(asio::error::address_in_use);
      }
    });
  }

  TPN_INLINE void DoStart(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpSessionBase DoStart state {} key {}",
              ToNetStateStr(this->state_), this->GetHashKey());

    this->GetDerivedObj().JoinSession(std::move(this_ptr));
  }

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

  TPN_INLINE auto &GetReadAllocator() { return this->rallocator_; }

  TPN_INLINE auto &GetWriteAllocator() { return this->wallocator_; }

 protected:
  HandlerMemory<SizeOp<>, std::false_type>
      rallocator_;  ///< 用户自定义内存用来处理 acceptor
  HandlerMemory<SizeOp<>, std::true_type>
      wallocator_;  ///< 用户自定义内存用来处理 send/write/post
};

class TcpSession : public TcpSessionBase<TcpSession, TemplateArgsTcpSession> {
 public:
  using TcpSessionBase<TcpSession, TemplateArgsTcpSession>::TcpSessionBase;
};

}  // namespace net
}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SESSION_H_
