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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_SILENCE_TIMER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_SILENCE_TIMER_H_

#include <atomic>
#include <memory>

#include "chrono_wrap.h"
#include "net_common.h"
#include "io_pool.h"

namespace tpn {

namespace net {

/// 静默定时器 收包间隔
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class SilenceTimer {
 public:
  /// 构造函数
  ///  @param[in]  io_handle    timer执行的io句柄
  explicit SilenceTimer(IoHandle &io_handle)
      : silence_timer_(io_handle.GetIoContext()) {
    this->silence_timer_canceled_.clear();
  }

  ~SilenceTimer() = default;

  /// 获取静默时长持续时间
  ///  @return 静默时长持续时间
  TPN_INLINE SteadyClock::duration GetSilenceTimeoutDuration() const {
    return this->silence_timeout_;
  }

  /// 设置静默持续时间 默认60分钟g
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   timeout     超时持续时间
  ///  @return CRTP调用链对象
  template <typename Rep, typename Period>
  TPN_INLINE Derived &SetSilenceTimeoutDuration(
      std::chrono::duration<Rep, Period> timeout) {
    NET_DEBUG("SilenceTimer SetSilenceTimeout session key:{} timeout {}",
              CRTP_CAST(this).GetHashKey(), time_out);
    this->silence_timeout_ = time_out;
    return (CRTP_CAST(this));
  }

 protected:
  /// 提交一个静默定时器
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   duration    超时持续时间
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  template <typename Rep, typename Period>
  TPN_INLINE void PostSilenceTimer(std::chrono::duration<Rep, Period> duration,
                                   std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("SilenceTimer PostSilenceTimer session {} duration {}",
              derive.GetHashKey(), duration);

    // 启动检查静音超时计时器
    if (duration > MilliSeconds(0)) {
      this->silence_timer_.expires_after(duration);
      this->silence_timer_.async_wait(asio::bind_executor(
          derive.GetIoHandle().GetStrand(),
          [&derive, self_ptr = std::move(this_ptr)](const std::error_code &ec) {
            NET_DEBUG(
                "SilenceTimer PostSilenceTimer session {} HandleSilenceTimer",
                derive.GetHashKey());
            derive.HandleSilenceTimer(ec, std::move(self_ptr));
          }));
    }
  }

  /// 处理静默定时器
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  TPN_INLINE void HandleSilenceTimer(const std::error_code &ec,
                                     std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("SilenceTimer HandleSilenceTimer session {} error {}",
              this_ptr->GetHashKey(), ec);

    if (asio::error::operation_aborted == ec ||
        this->silence_timer_canceled_.test_and_set()) {
      NET_WARN("SilenceTimer HandleSilenceTimer error {} or canceled", ec);
      return;
    }

    this->silence_timer_canceled_.clear();

    // 静默持续时间秒数不超过静默超时时间，请重新启动计时器事件，以避免此会话shared_ptr对象消失。
    if (derive.GetSilenceDuration() < this->silence_timeout_) {
      NET_DEBUG(
          "SilenceTimer HandleSilenceTimer session {} PostSilenceTimer again",
          derive.GetHashKey());
      derive.PostSilenceTimer(
          this->silence_timeout_ - derive.GetSilenceDuration(),
          std::move(this_ptr));
    } else {
      // 沉默超时已经消除，但是没有数据传输，不再发布计时器事件，因此在此会话中，
      // shared_ptr将消失，并且此处理程序返回后，对象将被自动销毁。
      NET_DEBUG(
          "SilenceTimer HandleSilenceTimer session {} timeout, so DoDisconnect",
          derive.GetHashKey());
      SetLastError(asio::error::timed_out);
      derive.DoDisconnect(asio::error::timed_out);
    }
  }

  /// 停止静默定时器
  TPN_INLINE void StopSilenceTimer() {
    NET_DEBUG("SilenceTimer StopSilenceTimer");
    this->silence_timer_canceled_.test_and_set();
    this->silence_timer_.cancel(s_ec_ignore);
  }

 protected:
  asio::steady_timer silence_timer_;                    ///<  静默定时器
  std::atomic_flag silence_timer_canceled_;             ///<  取消标志
  SteadyClock::duration silence_timeout_{Minutes(60)};  ///<  超时时间
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_SILENCE_TIMER_H_
