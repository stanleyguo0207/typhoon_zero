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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_RECONNECT_TIMER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_RECONNECT_TIMER_H_

#include <atomic>

#include "chrono_wrap.h"
#include "net_common.h"
#include "net_error.h"
#include "io_pool.h"

namespace tpn {

namespace net {

/// 重新连接定时器
/// 这个定时器只有客户端使用
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class ReconnectTimer {
 public:
  /// 构造函数
  ///  @param[in]  io_handle    timer执行的io句柄
  explicit ReconnectTimer(IoHandle &io_handle)
      : reconnect_timer_(io_handle.GetIoContext()) {
    this->reconnect_timer_canceled_.clear();
    this->reconnect_is_running_.clear();
  }

  ~ReconnectTimer() = default;

  /// 自动连接
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   enable    重连定时器是否可用
  ///  @param[in]   delay     重连定时器生效持续时间，enable为false时，无效
  ///  @return CRTP调用链对象
  template <typename Rep, typename Period>
  TPN_INLINE Derived &AutoReconnect(bool enable,
                                    std::chrono::duration<Rep, Period> delay) {
    NET_DEBUG("ReconnectTimer AutoReconnect {} delay {}", enable, delay);
    this->reconnect_enable_ = enable;
    this->reconnect_delay_  = delay;
    return (CRTP_CAST(this));
  }

  /// 自动连接
  ///  @param[in]   enable    重连定时器是否可用
  ///  @return CRTP调用链对象
  template <typename = void>
  TPN_INLINE Derived &AutoReconnect(bool enable) {
    NET_DEBUG("ReconnectTimer AutoReconnect {}", enable);
    this->reconnect_enable_ = enable;
    return (CRTP_CAST(this));
  }

 protected:
  /// 创建一个重连定时器
  ///  @tparam      Callback    重连定时器触发回调函数类型
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   callback    重连定时触发回调
  template <typename Callback>
  TPN_INLINE void MakeReconnectTimer(std::shared_ptr<Derived> this_ptr,
                                     Callback &&callback) {
    if (this->reconnect_is_running_.test_and_set()) {
      NET_WARN("ReconnectTimer MakeReconnectTimer reconnect is running");
      return;
    }

    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ReconnectTimer MakeReconnectTimer callback");

    derive.PostReconnectTimer(std::move(this_ptr),
                              std::forward<Callback>(callback),
                              NanoSeconds::max());  // 292 年
  }

  /// 处理重连定时器
  ///  @tparam      Callback    重连定时器触发回调函数类型
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   callback    重连定时触发回调
  template <typename Callback>
  TPN_INLINE void HandleReconnectTimer(const std::error_code &ec,
                                       std::shared_ptr<Derived> this_ptr,
                                       Callback &&callback) {
    if (this->reconnect_timer_canceled_.test_and_set()) {
      NET_WARN(
          "ReconnectTimer HandleReconnectTimer reconnect timer is canceled");
      this->reconnect_is_running_.clear();
      return;
    }

    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ReconnectTimer HandleReconnectTimer error {} callback", ec);

    this->reconnect_timer_canceled_.clear();

    if (asio::error::operation_aborted == ec) {
      derive.PostReconnectTimer(std::move(this_ptr),
                                std::forward<Callback>(callback),
                                this->reconnect_delay_);
    } else {
      if (this->reconnect_enable_) {
        NET_DEBUG(
            "ReconnectTimer HandleReconnectTimer reconnect enable and call "
            "callback");
        callback();
      }

      derive.PostReconnectTimer(std::move(this_ptr),
                                std::forward<Callback>(callback),
                                NanoSeconds::max());
    }
  }

  /// 停止重连定时器
  TPN_INLINE void StopReconnectTimer() {
    NET_DEBUG("ReconnectTimer StopReconnectTimer");
    this->reconnect_timer_canceled_.test_and_set();
    this->reconnect_timer_.cancel(s_ec_ignore);
  }

  /// 唤醒重连定时器
  TPN_INLINE void WakeReconnectTimer() {
    NET_DEBUG("ReconnectTimer WakeReconnectTimer enable {}",
              this->reconnect_enable_);
    if (this->reconnect_enable_) {
      this->reconnect_timer_.cancel(s_ec_ignore);
    }
  }

 private:
  /// 提交一个重连定时器
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tparam      Callback    重连定时器触发回调函数类型
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  ///  @param[in]   callback    重连定时触发回调
  ///  @param[in]   delay       重连定时器生效持续时间，enable为false时，无效
  template <class Rep, class Period, class Callback>
  TPN_INLINE void PostReconnectTimer(std::shared_ptr<Derived> this_ptr,
                                     Callback &&callback,
                                     std::chrono::duration<Rep, Period> delay) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ReconnectTimer PostReconnectTimer delay {} callback", delay);

    this->reconnect_timer_.expires_after(delay);
    this->reconnect_timer_.async_wait(
        asio::bind_executor(derive.GetIoHandle().GetStrand(),
                            [&derive, self_ptr = std::move(this_ptr),
                             callback = std::forward<Callback>(callback)](
                                const std::error_code &ec) mutable {
                              NET_DEBUG(
                                  "ReconnectTimer PostReconnectTimer is time "
                                  "to HandleReconnectTimer error {}",
                                  ec);
                              derive.HandleReconnectTimer(
                                  ec, std::move(self_ptr), std::move(callback));
                            }));
  }

 protected:
  asio::steady_timer reconnect_timer_;                 ///<  重连定时器
  std::atomic_flag reconnect_timer_canceled_;          ///<  重连取消
  SteadyClock::duration reconnect_delay_{Seconds(1)};  ///<  重连延迟
  bool reconnect_enable_{true};                        ///<  重连可用标志
  std::atomic_flag reconnect_is_running_;  ///<  重连进行中标志
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_RECONNECT_TIMER_H_
