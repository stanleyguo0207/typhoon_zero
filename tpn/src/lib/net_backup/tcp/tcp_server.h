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

#include "utils.h"
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
  using session_type = SessionType;

  using Super = ServerBase<Derived, SessionType>;
  using Self  = TcpServerBase<Derived, SessionType>;

  explicit TcpServerBase(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2,
      size_t buffer_max       = (std::numeric_limits<size_t>::max)(),
      size_t buffer_prepare   = kTcpFrameSize)
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
    NET_DEBUG("TcpServerBase DoStart {}:{} state{}", host, service,
              ToNetStateStr(this->state_));

    NetState expected = NetState::kNetStateStopped;
    if (!this->state_.compare_exchange_strong(expected,
                                              NetState::kNetStateStarting)) {
      NET_ERROR("TcpServerBase net already starting state {}",
                ToNetStateStr(this->state_));
      SetLastError(asio::error::already_started);
      return false;
    }

    Super::Start();

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
      this->counter_sptr_ =
          std::shared_ptr<void>(reinterpret_cast<void *>(1), [this](void *) {
            this->GetDerivedObj().Post([this]() mutable {
              NET_DEBUG("TcpServerBase counter final stop state {}",
                        ToNetStateStr(this->state_));
              NetState expected = NetState::kNetStateStopping;
              if (this->state_.compare_exchange_strong(
                      expected, NetState::kNetStateStopped)) {
                NET_DEBUG("TcpServerBase counter final stop state {}",
                          ToNetStateStr(this->state_));
                this->GetDerivedObj().HandleStop(asio::error::operation_aborted,
                                                 this->GetSelfSptr());
              } else {
                TPN_ASSERT(false, "TcpServerBase counter final stop error");
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

      // this->GetDerivedObj().FireInit();

      // 绑定端口
      this->acceptor_.bind(endpoint);
      // 监听
      this->acceptor_.listen();

      NET_DEBUG("TcpServerBase DoStart HandleStart");
      this->GetDerivedObj().HandleStart(std::error_code{});

      NET_DEBUG("TcpServerBase DoStart {}:{} state {} done", host, service,
                ToNetStateStr(this->state_));

      return (this->IsStarted());
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase DoStart HandleStart error {}", e.code());
      this->GetDerivedObj().HandleStart(e.code());
    }
    return false;
  }

  TPN_INLINE void HandleStart(std::error_code ec) {
    NET_DEBUG("TcpServerBase HandleStart error {} state {}", ec,
              ToNetStateStr(this->state_));

    try {
      NetState expected = NetState::kNetStateStarting;
      if (!ec && !this->state_.compare_exchange_strong(
                     expected, NetState::kNetStateStarted)) {
        NET_WARN("TcpServerBase HandleStart not starting state {}",
                 ToNetStateStr(this->state_));
        ec = asio::error::operation_aborted;
      }

      SetLastError(ec);

      // 无论启动成功或是失败，都需要通知Start事件
      // this->GetDerivedObj().FireStart(ec);

      expected = NetState::kNetStateStarted;
      if (!ec && !this->state_.compare_exchange_strong(
                     expected, NetState::kNetStateStarted)) {
        NET_WARN("TcpServerBase HandleStart not started state {}",
                 ToNetStateStr(this->state_));
        asio::detail::throw_error(asio::error::operation_aborted);
      }

      asio::detail::throw_error(ec);

      NET_DEBUG("TcpServerBase HandleStart PostWrap PostAccept");
      this->GetDerivedObj().Post(
          [this]() mutable { this->GetDerivedObj().PostAccept(); });
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase HandleStart error {} state {}", e.code(),
                ToNetStateStr(this->state_));
      SetLastError(e);
      this->GetDerivedObj().DoStop(e.code());
    }
  }

  TPN_INLINE void DoStop(const std::error_code &ec) {
    NET_DEBUG("TcpServerBase DoStop error {} state {}", ec,
              ToNetStateStr(this->state_));

    // 启动中停止
    NetState expected = NetState::kNetStateStarting;
    if (this->state_.compare_exchange_strong(expected,
                                             NetState::kNetStateStopping)) {
      NET_DEBUG("TcpServerBase DoStop from {} change to {} state {}",
                ToNetStateStr(NetState::kNetStateStarting),
                ToNetStateStr(expected), ToNetStateStr(this->state_));
      return this->GetDerivedObj().PostStop(ec, this->GetSelfSptr(), expected);
    }

    // 已经启动停止
    expected = NetState::kNetStateStarted;
    if (this->state_.compare_exchange_strong(expected,
                                             NetState::kNetStateStopping)) {
      NET_DEBUG("TcpServerBase DoStop from {} change to {} state {}",
                ToNetStateStr(NetState::kNetStateStarted),
                ToNetStateStr(expected), ToNetStateStr(this->state_));
      return this->GetDerivedObj().PostStop(ec, this->GetSelfSptr(), expected);
    }
  }

  TPN_INLINE void PostStop(const std::error_code &ec,
                           std::shared_ptr<Derived> self_ptr,
                           NetState old_state) {
    NET_DEBUG("TcpServerBase PostStop error {} old_state {} state {}", ec,
              ToNetStateStr(old_state), ToNetStateStr(this->state_));

    this->GetDerivedObj().Post(
        [this, ec, this_ptr = std::move(self_ptr), old_state]() mutable {
          IgnoreUnused(this, old_state);

          SetLastError(ec);

          this->counter_timer_.expires_after(NanoSeconds::max());
          this->counter_timer_.async_wait(asio::bind_executor(
              this->io_handle_.GetStrand(), [](const std::error_code &) {}));

          NET_DEBUG("TcpServerBase PostStop to apply all session to stop");
          this->GetDerivedObj().ForEachSession(
              [](std::shared_ptr<SessionType> &session_sptr) mutable {
                session_sptr->Stop();
              });

          // 引用计数归0
          this->counter_sptr_.reset();
        });
  }

  TPN_INLINE void HandleStop(const std::error_code &ec,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpServerBase HandleStop error {} state {}", ec,
              ToNetStateStr(this->state_));

    IgnoreUnused(ec, this_ptr);

    // this->GetDerivedObj().FireStop(ec);

    this->acceptor_timer_.cancel(s_ec_ignore);
    this->counter_timer_.cancel(s_ec_ignore);

    Super::Stop();

    this->acceptor_.close(s_ec_ignore);
  }

  template <typename... Args>
  TPN_INLINE std::shared_ptr<SessionType> MakeSession(Args &&...args) {
    NET_DEBUG("TcpServerBase MakeSession state {}",
              ToNetStateStr(this->state_));
    return std::make_shared<SessionType>(
        std::forward<Args>(args)..., this->GetIoHandleByIndex(),
        this->listener_, this->session_mgr_, this->buffer_max_,
        this->buffer_prepare_);
  }

  TPN_INLINE void PostAccept() {
    NET_DEBUG("TcpServerBase PostAccept state {}", ToNetStateStr(this->state_));

    if (!this->IsStarted()) {
      NET_ERROR("TcpServerBase PostAccept state {}",
                ToNetStateStr(this->state_));
      return;
    }

    try {
      std::shared_ptr<SessionType> session_sptr =
          this->GetDerivedObj().MakeSession();

      NET_DEBUG("TcpServerBase PostAccept new session {}",
                session_sptr->GetHashKey());

      auto &socket = session_sptr->GetSocket().lowest_layer();
      this->acceptor_.async_accept(
          socket, asio::bind_executor(
                      this->io_handle_.GetStrand(),
                      MakeAllocator(this->rallocator_,
                                    [this, sptr = std::move(session_sptr)](
                                        const std::error_code &ec) mutable {
                                      this->GetDerivedObj().HandleAccept(
                                          ec, std::move(sptr));
                                    })));
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase PostAccept error {}", e.code());

      SetLastError(e);

      this->acceptor_timer_.expires_after(Seconds(1));
      this->acceptor_timer_.async_wait(asio::bind_executor(
          this->io_handle_.GetStrand(),
          MakeAllocator(
              this->rallocator_, [this](const std::error_code &ec) mutable {
                SetLastError(ec);
                if (ec) {
                  NET_ERROR("TcpServerBase PostAccept async_wait error {}", ec);
                  return;
                }
                asio::post(this->io_handle_.GetStrand(),
                           MakeAllocator(this->rallocator_, [this]() mutable {
                             this->GetDerivedObj().PostAccept();
                           }));
              })));
    }
  }

  TPN_INLINE void HandleAccept(const std::error_code &ec,
                               std::shared_ptr<SessionType> session_sptr) {
    NET_DEBUG("TcpServerBase HandleAccept error {} state {} session {} {}", ec,
              ToNetStateStr(this->state_), session_sptr->GetHashKey(),
              ToNetStateStr(session_sptr->GetNetState()));

    SetLastError(ec);

    if (asio::error::operation_aborted == ec) {
      NET_DEBUG("TcpServerBase HandleAccept aborted and do stop");
      this->GetDerivedObj().DoStop(ec);
      return;
    }

    if (!ec) {
      if (this->IsStarted()) {
        NET_DEBUG("TcpServerBase HandleAccept session {} start {}",
                  session_sptr->GetHashKey(),
                  ToNetStateStr(session_sptr->GetNetState()));
        session_sptr->counter_sptr_ = this->counter_sptr_;
        session_sptr->Start();
      }
    }

    NET_DEBUG("TcpServerBase HandleAccept error {} state {} session {} {}", ec,
              ToNetStateStr(this->state_), session_sptr->GetHashKey(),
              ToNetStateStr(session_sptr->GetNetState()));
    this->GetDerivedObj().PostAccept();
  }

 protected:
  asio::ip::tcp::acceptor acceptor_;   ///< 接受者
  asio::steady_timer acceptor_timer_;  //< 用来处理接受者异常定时器
  asio::steady_timer counter_timer_;  ///< 确保持有的io_handle直到所有的会话关闭
  size_t buffer_max_{(std::numeric_limits<size_t>::max)()};  ///< 缓冲区最大值
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
