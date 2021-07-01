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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_CONNECT_TIMER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_CONNECT_TIMER_H_

#include <atomic>

#include "chrono_wrap.h"
#include "net_common.h"
#include "io_pool.h"

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType = void>
class ConnectTimeout {
 public:
  explicit ConnectTimeout(IoHandle &connect_timeout_timer_io_handle)
      : connect_timeout_timer_(connect_timeout_timer_io_handle.GetIoContext()) {
    this->connect_timer_canceled_.clear();
  }

  ~ConnectTimeout() = default;

  TPN_INLINE SteadyClock::duration GetConnectTimeout() {
    return this->connect_timeout_;
  }

  template <typename Rep, typename Period>
  TPN_INLINE Derived &SetConnectTimeout(
      std::chrono::duration<Rep, Period> timeout) {
    this->connect_timeout_ = timeout;
    NET_DEBUG("ConnectTimeout set connect timeout {}", timeout);
    return (CRTP_CAST(this));
  }

 protected:
  template <typename Rep, typename Period>
  TPN_INLINE void PostConnectTimeoutTimer(
      std::chrono::duration<Rep, Period> duration,
      std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ConnectTimeout PostConnectTimeoutTimer {}", duration);

    this->connect_error_code_.clear();
    this->connect_timeout_flag_.store(false);

    this->connect_timeout_timer_.expires_after(duration);
    this->connect_timeout_timer_.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        [&derive,
         self_ptr = std::move(this_ptr)](const std::error_code &ec) mutable {
          NET_DEBUG("ConnectTimeout connect timeout error {}", ec);
          derive.HandleConnectTimeoutTimer(ec, std::move(self_ptr));
        }));
  }

  template <typename Rep, typename Period, typename Func>
  TPN_INLINE void PostConnectTimeoutTimer(
      std::chrono::duration<Rep, Period> duration,
      std::shared_ptr<Derived> this_ptr, Func &&func) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ConnectTimeout PostConnectTimeoutTimer {}", duration);

    this->connect_error_code_.clear();
    this->connect_timeout_flag_.store(false);

    this->connect_timeout_timer_.expires_after(duration);
    this->connect_timeout_timer_.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        [this, self_ptr = std::move(this_ptr),
         func = std::forward<Func>(func)](const std::error_code &ec) mutable {
          NET_DEBUG("ConnectTimeout connect timeout func error {}", ec);
          if (!ec) {
            this->connect_timeout_flag_.store(true);
          }

          this->connect_timer_canceled_.clear();

          func(ec);
        }));
  }

  TPN_INLINE void HandleConnectTimeoutTimer(const std::error_code &ec,
                                            std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ConnectTimeout HandleConnectTimeoutTimer error {}", ec);

    std::ignore = this_ptr;

    if (!ec) {
      this->connect_timeout_flag_.store(true);
    }

    if (ec == asio::error::operation_aborted ||
        this->connect_timer_canceled_.test_and_set()) {
      NET_DEBUG("ConnectTimeout HandleConnectTimeoutTimer abort or canceled");
      this->connect_timer_canceled_.clear();
      return;
    }

    this->connect_timer_canceled_.clear();

    if (!ec) {
      NET_DEBUG(
          "ConnectTimeout HandleConnectTimeoutTimer timeout DoDisconnect");
      derive.DoDisconnect(asio::error::timed_out);
    } else {
      NET_DEBUG(
          "ConnectTimeout HandleConnectTimeoutTimer error {} DoDisconnect",
          this->connect_error_code_ ? this->connect_error_code_ : ec);
      derive.DoDisconnect(this->connect_error_code_ ? this->connect_error_code_
                                                    : ec);
    }
  }

  TPN_INLINE void StopConnectTimeoutTimer(std::error_code ec) {
    NET_DEBUG("ConnectTimeout StopConnectTimeoutTimer error {}", ec);

    try {
      this->connect_error_code_ = ec;
      this->connect_timer_canceled_.test_and_set();
      this->connect_timeout_timer_.cancel(s_ec_ignore);
    } catch (std::system_error &) {
    } catch (std::exception &) {
    }
  }

  TPN_INLINE bool IsConnectTimeout() {
    return this->connect_timeout_flag_.load();
  }

  TPN_INLINE std::error_code GetConnectErrorCode() {
    return this->connect_error_code_;
  }

 protected:
  asio::steady_timer connect_timeout_timer_;           ///<  超时定时器
  std::atomic_flag connect_timer_canceled_;            ///<  超时取消
  SteadyClock::duration connect_timeout_{Seconds(5)};  ///<  超时时间
  std::error_code connect_error_code_;                 ///<  超时错误码
  std::atomic_bool connect_timeout_flag_{false};       ///<  超时标志
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_CONNECT_TIMER_H_
