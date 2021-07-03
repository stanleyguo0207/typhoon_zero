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

#include "net_common.h"
#include "client.h"
#include "tcp_keepalive.h"
#include "tcp_send_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_CLIENT_CLASS

struct TemplateArgsTcpClient {
  static constexpr bool is_session = false;
  static constexpr bool is_client  = true;

  using socket_type = asio::ip::tcp::socket;
  using buffer_type = asio::streambuf;
};

template <typename Derived, typename ArgsType>
class TcpClientBase : public ClientBase<Derived, ArgsType>,
                      public TcpKeepAlive<Derived, ArgsType>,
                      public TcpSendWrap<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_CLIENT_CLASS
 public:
  using buffer_type = typename ArgsType::buffer_type;

  using Super = ClientBase<Derived, ArgsType>;
  using Self  = TcpClientBase<Derived, ArgsType>;

  explicit TcpClientBase(
      size_t buffer_max     = (std::numeric_limits<size_t>::max)(),
      size_t buffer_prepare = kTcpFrameSize)
      : Super(1, buffer_max, buffer_prepare),
        TcpKeepAlive<Derived, ArgsType>(this->socket_),
        TcpSendWrap<Derived, ArgsType>() {
    this->SetConnectTimeout(MilliSeconds(kTcpConnectTimeout));
  };

  ~TcpClientBase() { this->Stop(); }

  template <typename String, typename StrOrInt>
  bool Start(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase Start {}:{}", host, port);
    return this->GetDerivedObj().template DoConnect<false>(
        std::forward<String>(host), std::forward<StrOrInt>(port));
  }

  template <typename String, typename StrOrInt>
  bool AsyncStart(String &&host, StrOrInt &&port) {
    NET_DEBUG("TcpClientBase async Start {}:{}", host, port);
    return this->GetDerivedObj().template DoConnect<true>(
        std::forward<String>(host), std::forward<StrOrInt>(port));
  }

  TPN_INLINE bool DoSend(ByteBuffer &&packet) {
    NET_DEBUG("TcpClientBase DoSend state {}", ToNetStateStr(this->state_));
    return this->GetDerivedObj().TcpSend(std::move(packet));
  }

  TPN_INLINE void Stop() {
    NET_DEBUG("TcpClientBase Stop");

    this->GetDerivedObj().Post(
        [this]() mutable { this->StopReconnectTimer(); });

    this->GetDerivedObj().DoDisconnect(
        asio::error::operation_aborted, std::make_shared<DeferWrap>([this]() {
          NET_DEBUG("TcpClientBase Stop DoDisconnect DoStop");
          this->GetDerivedObj().DoStop(asio::error::operation_aborted);
        }));

    this->IoPoolStop();
  }

  template <typename Func, typename... Objects>
  TPN_INLINE Derived &BindConnect(Func &&func, Objects &&...objs) {
    this->listener_.Bind(
        ListenerEvent::kListenerEventConnect,
        Observer<std::error_code>(std::forward<Func>(func),
                                  std::forward<Objects>(objs)...));
    return (this->GetDerivedObj());
  }

 protected:
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

      this->IoPoolStart();

      if (this->IsIoPoolStopped()) {
        NET_ERROR("TcpClientBase io_pool start error");
        SetLastError(asio::error::shut_down);
        return false;
      }

      this->GetDerivedObj().LoadReconnectTimer(host, port);

      this->GetDerivedObj().DoInit();

      Super::Start();

      return this->GetDerivedObj().template StartConnect<IsAsync>(
          std::forward<String>(host), std::forward<StrOrInt>(port),
          this->GetSelfSptr());
    } catch (std::system_error &e) {
      NET_DEBUG("TcpClientBase DoConnect error {}", e.code());
      this->GetDerivedObj().HandleConnect(e.code(),
                                          this->GetDerivedObj().GetSelfSptr());
    }

    return false;
  }

  TPN_INLINE void DoInit() { NET_DEBUG("TcpClientBase DoInit"); }

  TPN_INLINE void DoStart(std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase DoStart");
    this->UpdateAliveTime();
    this->ResetConnectTime();
  }

  TPN_INLINE void DoStop(const std::error_code &ec) {
    NET_DEBUG("TcpClientBase DoStop error {}", ec);
    this->GetDerivedObj().PostStop(ec, this->GetSelfSptr());
  }

  TPN_INLINE void PostStop(const std::error_code &ec,
                           std::shared_ptr<Derived> self_ptr) {
    NET_DEBUG("TcpClientBase PostStop error {}", ec);
    auto task = [this, ec, this_ptr = std::move(self_ptr)](
                    EventQueueGuard<Derived> &&guard) mutable {
      NET_DEBUG("TcpClientBase PostStop task to handleStop");
      SetLastError(ec);

      Super::Stop();

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

  TPN_INLINE void HandleStop(const std::error_code &ec,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase HandleStop error {}", ec);
    IgnoreUnused(ec, this_ptr);
  }

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
            auto task = [this, h, p](EventQueueGuard<Derived> &&guard) mutable {
              NET_DEBUG("TcpClientBase Reconnect Timer task to StartConnect");
              this->GetDerivedObj().template StartConnect<true>(
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

  TPN_INLINE void HandleDisconnect(const std::error_code &ec,
                                   std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpClientBase HandleDisconnect error {}", ec);

    IgnoreUnused(ec, this_ptr);

    this->socket_.lowest_layer().shutdown(asio::socket_base::shutdown_both,
                                          s_ec_ignore);
    this->socket_.lowest_layer().close(s_ec_ignore);
  }

  TPN_INLINE void FireConnect(std::shared_ptr<Derived> &this_ptr,
                              std::error_code ec) {
    NET_DEBUG("TcpClientBase FireConnect state {} error {}",
              ToNetStateStr(this->state_), ec);

    this->listener_.Notify(ListenerEvent::kListenerEventConnect, ec);
  }
};

class TcpClient : public TcpClientBase<TcpClient, TemplateArgsTcpClient> {
 public:
  using TcpClientBase<TcpClient, TemplateArgsTcpClient>::TcpClientBase;
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_CLIENT_H_
