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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_

#include <limits>

#include "server.h"
#include "tcp_session.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS

template <typename Derived, typename SessionType>
class TcpServerBase : public ServerBase<Derived, SessionType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS
 public:
  using Super       = ServerBase<Derived, SessionType>;
  using Self        = TcpServerBase<Derived, SessionType>;
  using SessionType = SessionType;

  explicit TcpServerBase(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2,
      size_t buffer_max       = (std::numeric_limits<size_t> max)(),
      size_t buffer_prepare   = kTcpFrameSize){}
      : Super(concurrency_hint),
        acceptor_(this->io_handle_.GetIoContext()),
        acceptor_timer_(this->io_handle_.GetIoContext()),
        counter_timer_(this->io_handle_.GetIoContext()),
        buffer_max_(buffer_max),
        buffer_prepare_(buffer_prepare) {}

  ~TcpServerBase() { this->Stop(); }

  template <typename StrOrInt>
  TPN_INLINE bool Start(StrOrInt &&service) {
    return this->Start(std::string_view{}, std::forward<StrOrInt>(service));
  }

  template <typename String, typename StrOrInt>
  TPN_INLINE bool Start(String &&host, StrOrInt &&service) {
    NET_DEBUG("TcpServerBase Start {}:{} state {}", host, service,
              ToNetStateStr(this->state_));
    return this->GetDerivedObj().DoStart(std::forward<String>(host),
                                         std::forward<StrOrInt>(service));
  }

  TPN_INLINE void Stop() {
    NET_DEBUG("TcpServerBase Stop state {}", ToNetStateStr(this->state_));
    this->GetDerivedObj().DoStop(asio::error::operation_aborted);
    this->IoPoolStop();
  }

  TPN_INLINE bool IsStarted() {
    return (Super::IsStarted() && this->acceptor_.is_open());
  }

  TPN_INLINE bool IsStopped() {
    return (Super::IsStopped() && this->acceptor_.is_open());
  }

  TPN_INLINE asio::ip::tcp::acceptor &GetAcceptor() { return this->acceptor_; }

 protected:
  template <typename String, typename StrOrInt>
  TPN_INLINE bool DoStart(String &&host, StrOrInt &&service) {
    NET_DEBUG(
        TcpServerBase DoStart{}
        : {} state{}, host, service, ToNetStateStr(this->state_));

    NetState expected = NetState::kNetStateStopped;
    if (!this->state_.compare_exchange_strong(expected,
                                              NetState::kNetStateStarting)) {
      NET_ERROR("TcpServerBase net already starting state {}",
                ToNetStateStr(this->state_));
      SetLastError(asio::error::already_started);
      return false;
    }

    try {
      ClearLastError();

      // 启动对象池
      this->IoPoolStart();

      if (this->IsIoPoolStopped()) {
        NET_ERROR("TcpServerBase io_pool start error");
        SetLastError(asio::error::shut_down);
        return false;
      }

      // 确保引用计数为0时 做最后的关闭处理
      this->counter_ptr_ =
          std::shared_ptr<void>(reinterpret_cast<void *>(1), [this](void *) {
            this->GetDerivedObj().Post([this]() mutable {
              NET_DEBUG("TcpServerBase counter final stop state {}",
                        ToNetStateStr(this->state_));
              NetState expected = NetState::kStateStopping;
              if (this->state_.compare_exchange_strong(
                      expected, NetState::kStateStopped)) {
                NET_DEBUG("TcpServerBase counter final stop state {}",
                          ToNetStateStr(this->state_));
                this->GetDerivedObj().HandleStop(asio::error::operation_aborted,
                                                 this->GetSelfSptr());
              } else {
                TPN_ASSERT(false);
              }
            });
          });

      this->acceptor_.close(s_ec_ignore);

      std::string h = ToString(std::forward<String>(host));
      std::string p = ToString(std::forward<StrOrInt>(service));

      // 解析地址端口
      asio::ip::tcp::resolver resolver(this->io_handle_.GetIoContext());
      asio::ip::tcp::endpoint endpoint =
          *resolver
               .resolve(h, p,
                        asio::ip::resolver_base::flags::passive |
                            asio::ip::resolver_base::flags::address_configured)
               .begin();

      NET_DEBUG("TcpServerBase {}:{} resolver {}:{}", h, p,
                endpoint.address().to_string(), endpoint.port());

      // 接收器开启
      this->acceptor_.open(endpoint.protocol());

      // 当您在linux系统中关闭套接字并立即启动套接字时，您将得到如下所示的“地址正在使用”，
      // 并且绑定失败，但是我很抱歉，我之前已经正确地关闭了套接字，为什么会这样？
      // 原因是套接字选项“ TIME_WAIT”，尽管您关闭了套接字，但是系统没有释放套接字，
      // 直到2到4秒后才释放套接字，所以我们可以使用SO_REUSEADDR选项来避免此问题，如下所示
      this->acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

      this->GetDerivedObj().FireInit();

      // 绑定端口
      this->acceptor_.bind(endpoint);
      // 监听
      this->acceptor_.listen();

      NET_DEBUG("TcpServerBase DoStart HandleStart");
      this->GetDerivedObj().HandleStart(std::error_code{},
                                        std::move(condition));

      NET_DEBUG("TcpServerBase DoStart {}:{} state {} done", host, service,
                ToNetStateStr(this->state_));

      return (this->IsStarted());
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase DoStart HandleStart error {}", e.code());
      this->GetDerivedObj().HandleStart(e.code(), std::move(condition));
    }
    return false;
  }

  TPN_INLINE void HandleStart(std::error_code ec) {}

  TPN_INLINE void DoStop(const std::error_code &ec) {}

  TPN_INLINE void PostStop(const std::error_code &ec,
                           std::shared_ptr<Derived> self_ptr,
                           NetState old_state) {}

  TPN_INLINE void HandleStop(const std::error_code &ec,
                             std::shared_ptr<Derived> this_ptr) {}

  template <typename... Args>
  TPN_INLINE std::shared_ptr<SessionType> MakeSession(Args &&...args) {}

  TPN_INLINE void PostAccept() {}

  TPN_INLINE void HandleAccept(const std::error_code &ec,
                               std::shared_ptr<SessionType> session_sptr) {}

 protected:
  asio::ip::tcp::acceptor acceptor_;   ///< 接受者
  asio::steady_timer acceptor_timer_;  //< 用来处理接受者异常定时器
  asio::steady_timer counter_timer_;  ///< 确保持有的io_handle直到所有的会话关闭
  size_t buffer_max_{(std::numeric_limits<size_t> max)()};  ///< 缓冲区最大值
  size_t buffer_prepare_{kTcpFrameSize};  ///< 缓冲区初始大小
};

/// tcp服务器桥梁
template <typename SessionType>
class TcpServerBridge
    : public TcpServerBase<TcpServerBridge<SessionType>, SessionType> {
 public:
  using TcpServerBase<TcpServerBridge<SessionType>, SessionType>::TcpServerBase;
};

/// tcp服务器
using TcpServer = TcpServerBridge<TcpSession>;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
