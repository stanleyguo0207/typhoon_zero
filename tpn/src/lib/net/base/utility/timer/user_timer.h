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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_USER_TIMER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_USER_TIMER_H_

#include <atomic>
#include <functional>
#include <unordered_map>

#include "chrono_wrap.h"
#include "timer_define.h"
#include "net_common.h"
#include "io_pool.h"
#include "custom_allocator.h"

namespace tpn {

namespace net {

/// 用户自定义定时器索引
struct UserTimerKey {
  /// 构造函数
  ///  @param[in]   group_in    定时器组别
  ///  @param[in]   id_in       定时器id
  UserTimerKey(TimerGroup group_in, TimerId id_in)
      : group(group_in), id(id_in) {}

  /// 判断两个索引是否相同
  TPN_INLINE bool operator==(const UserTimerKey &other) const {
    return (other.group == this->group && other.id == this->id);
  }

  TimerGroup group;  ///< 组别
  TimerId id;        ///< id

  TPN_DEFAULT_COPY(UserTimerKey)
  TPN_DEFAULT_MOVE(UserTimerKey)
};

/// 用户自定义定时器索引哈希值
struct UserTimerKeyHash {
  TPN_INLINE uint32_t operator()(const UserTimerKey &key) const {
    return MAKE_UINT32_T(key.group, key.id);
  }
};

/// 用户自定义定时器索引相等panduan
struct UserTimerKeyEqual {
  TPN_INLINE bool operator()(const UserTimerKey &lhs,
                             const UserTimerKey &rhs) const {
    return lhs == rhs;
  }
};

/// 定时器永久循环
static constexpr int kUserTimerCycleForever = -1;

/// 用户自定义定时器对象
struct UserTimerObj {
  /// 构造函数
  ///  @param[in]   key_in    定时器索引
  ///  @param[in]   context   asio 上下文
  ///  @param[in]   task_in   定时任务
  UserTimerObj(UserTimerKey key_in, asio::io_context &context,
               std::function<void()> task_in,
               int cycle_in = kUserTimerCycleForever)
      : key(std::move(key_in)),
        timer(context),
        task(std::move(task_in)),
        cycle(cycle_in) {}

  UserTimerKey key;                                ///< 定时器索引
  asio::steady_timer timer;                        ///< 定时器
  std::function<void()> task;                      ///< 定时任务
  std::atomic<int> cycle{kUserTimerCycleForever};  ///< 重复次数
  bool exited{false};                              ///< 退出标志
};

/// 用户自定义定时器
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType = void>
class UserTimer {
 public:
  using UserTimerMap =
      std::unordered_map<UserTimerKey, std::shared_ptr<UserTimerObj>,
                         UserTimerKeyHash, UserTimerKeyEqual>;

  UserTimer()  = default;
  ~UserTimer() = default;

  /// 开启定时器
  /// 从当前时间开始，间隔interval触发定时器，不会停止
  /// 默认在kTimerGroupCommon组中
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   id        定时器id
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartTimer(TimerId id,
                             std::chrono::duration<Rep, Period> interval,
                             Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [this, &derive, this_ptr = derive.GetSelfSptr(),
                       key = UserTimerKey(TimerGroup::kTimerGroupCommon, id),
                       interval, task = std::move(task)]() mutable {
                        std::shared_ptr<UserTimerObj> timer_obj_sptr;
                        auto iter = this->user_timers_.find(key);
                        if (this->user_timers_.end() != iter) {
                          timer_obj_sptr       = iter->second;
                          timer_obj_sptr->task = std::move(task);
                        } else {
                          timer_obj_sptr = std::make_shared<UserTimerObj>(
                              key, derive.GetIoHandle().GetIoContext(),
                              std::move(task));

                          this->user_timers_[std::move(key)] = timer_obj_sptr;
                        }

                        derive.PostUserTimers(std::move(timer_obj_sptr),
                                              interval, std::move(this_ptr));
                      }));
  }

  /// 开启定时器
  /// 从当前时间开始，间隔interval触发定时器，不会停止
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   group     定时器组
  ///  @param[in]   id        定时器id
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartTimer(TimerGroup group, TimerId id,
                             std::chrono::duration<Rep, Period> interval,
                             Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [this, &derive, this_ptr = derive.GetSelfSptr(),
                       key  = UserTimerKey(group, id), interval,
                       task = std::move(task)]() mutable {
                        std::shared_ptr<UserTimerObj> timer_obj_sptr;
                        auto iter = this->user_timers_.find(key);
                        if (this->user_timers_.end() != iter) {
                          timer_obj_sptr       = iter->second;
                          timer_obj_sptr->task = std::move(task);
                        } else {
                          timer_obj_sptr = std::make_shared<UserTimerObj>(
                              key, derive.GetIoHandle().GetIoContext(),
                              std::move(task));

                          this->user_timers_[std::move(key)] = timer_obj_sptr;
                        }

                        derive.PostUserTimers(std::move(timer_obj_sptr),
                                              interval, std::move(this_ptr));
                      }));
  }

  /// 延迟开启定时器
  /// 从当前时间延迟delay时间开始，间隔interval触发定时器，不会停止
  /// 默认在kTimerGroupCommon组中
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   id        定时器id
  ///  @param[in]   delay     延迟启动时间
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartDelayTimer(TimerId id,
                                  std::chrono::duration<Rep, Period> delay,
                                  std::chrono::duration<Rep, Period> interval,
                                  Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(
            derive.GetWriteAllocator(),
            [this, &derive, this_ptr = derive.GetSelfSptr(),
             key = UserTimerKey(TimerGroup::kTimerGroupCommon, id), delay,
             interval, task = std::move(task)]() mutable {
              std::shared_ptr<UserTimerObj> timer_obj_sptr;
              auto iter = this->user_timers_.find(key);
              if (this->user_timers_.end() != iter) {
                timer_obj_sptr       = iter->second;
                timer_obj_sptr->task = std::move(task);
              } else {
                timer_obj_sptr = std::make_shared<UserTimerObj>(
                    key, derive.GetIoHandle().GetIoContext(), std::move(task));

                this->user_timers_[std::move(key)] = timer_obj_sptr;
              }

              derive.PostDelayUserTimers(std::move(timer_obj_sptr), delay,
                                         interval, std::move(this_ptr));
            }));
  }

  /// 延迟开启定时器
  /// 从当前时间延迟delay时间开始，间隔interval触发定时器，不会停止
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   group     定时器组
  ///  @param[in]   id        定时器id
  ///  @param[in]   delay     延迟启动时间
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartDelayTimer(TimerGroup group, TimerId id,
                                  std::chrono::duration<Rep, Period> delay,
                                  std::chrono::duration<Rep, Period> interval,
                                  Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(
            derive.GetWriteAllocator(),
            [this, &derive, this_ptr = derive.GetSelfSptr(),
             key  = UserTimerKey(group, id), delay, interval,
             task = std::move(task)]() mutable {
              std::shared_ptr<UserTimerObj> timer_obj_sptr;
              auto iter = this->user_timers_.find(key);
              if (this->user_timers_.end() != iter) {
                timer_obj_sptr       = iter->second;
                timer_obj_sptr->task = std::move(task);
              } else {
                timer_obj_sptr = std::make_shared<UserTimerObj>(
                    key, derive.GetIoHandle().GetIoContext(), std::move(task));

                this->user_timers_[std::move(key)] = timer_obj_sptr;
              }

              derive.PostDelayUserTimers(std::move(timer_obj_sptr), delay,
                                         interval, std::move(this_ptr));
            }));
  }

  /// 开启固定循环次数定时器
  /// 从当前时间开始，间隔interval触发定时器，执行cycle次
  /// 默认在kTimerGroupCommon组中
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   id        定时器id
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   cycle     循环次数 -1 为永久
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartCycleTimer(TimerId id,
                                  std::chrono::duration<Rep, Period> interval,
                                  int cycle, Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [this, &derive, this_ptr = derive.GetSelfSptr(),
                       key = UserTimerKey(TimerGroup::kTimerGroupCommon, id),
                       interval, cycle, task = std::move(task)]() mutable {
                        std::shared_ptr<UserTimerObj> timer_obj_sptr;
                        auto iter = this->user_timers_.find(key);
                        if (this->user_timers_.end() != iter) {
                          timer_obj_sptr       = iter->second;
                          timer_obj_sptr->task = std::move(task);
                        } else {
                          timer_obj_sptr = std::make_shared<UserTimerObj>(
                              key, derive.GetIoHandle().GetIoContext(),
                              std::move(task), cycle);

                          this->user_timers_[std::move(key)] = timer_obj_sptr;
                        }

                        derive.PostUserTimers(std::move(timer_obj_sptr),
                                              interval, std::move(this_ptr));
                      }));
  }

  /// 开启固定循环次数定时器
  /// 从当前时间开始，间隔interval触发定时器，执行cycle次
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   group     定时器组
  ///  @param[in]   id        定时器id
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   cycle     循环次数 -1 为永久
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartCycleTimer(TimerGroup group, TimerId id,
                                  std::chrono::duration<Rep, Period> interval,
                                  int cycle, Func &&func, Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [this, &derive, this_ptr = derive.GetSelfSptr(),
                       key  = UserTimerKey(group, id), interval, cycle,
                       task = std::move(task)]() mutable {
                        std::shared_ptr<UserTimerObj> timer_obj_sptr;
                        auto iter = this->user_timers_.find(key);
                        if (this->user_timers_.end() != iter) {
                          timer_obj_sptr       = iter->second;
                          timer_obj_sptr->task = std::move(task);
                        } else {
                          timer_obj_sptr = std::make_shared<UserTimerObj>(
                              key, derive.GetIoHandle().GetIoContext(),
                              std::move(task), cycle);

                          this->user_timers_[std::move(key)] = timer_obj_sptr;
                        }

                        derive.PostUserTimers(std::move(timer_obj_sptr),
                                              interval, std::move(this_ptr));
                      }));
  }

  /// 延迟开启固定循环次数定时器
  /// 从当前时间延迟delay时间开始，间隔interval触发定时器，执行cycle次
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   id        定时器id
  ///  @param[in]   delay     延迟启动时间
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   cycle     循环次数 -1 为永久
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartDelayCycleTimer(
      TimerId id, std::chrono::duration<Rep, Period> delay,
      std::chrono::duration<Rep, Period> interval, int cycle, Func &&func,
      Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(derive.GetIoHandle().GetStrand(),
               MakeAllocator(
                   derive.GetWriteAllocator(),
                   [this, &derive, this_ptr = derive.GetSelfSptr(),
                    key = UserTimerKey(TimerGroup::kTimerGroupCommon, id),
                    delay, interval, cycle, task = std::move(task)]() mutable {
                     std::shared_ptr<UserTimerObj> timer_obj_sptr;
                     auto iter = this->user_timers_.find(key);
                     if (this->user_timers_.end() != iter) {
                       timer_obj_sptr       = iter->second;
                       timer_obj_sptr->task = std::move(task);
                     } else {
                       timer_obj_sptr = std::make_shared<UserTimerObj>(
                           key, derive.GetIoHandle().GetIoContext(),
                           std::move(task), cycle);

                       this->user_timers_[std::move(key)] = timer_obj_sptr;
                     }

                     derive.PostDelayUserTimers(std::move(timer_obj_sptr),
                                                delay, interval,
                                                std::move(this_ptr));
                   }));
  }

  /// 延迟开启固定循环次数定时器
  /// 从当前时间延迟delay时间开始，间隔interval触发定时器，执行cycle次
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @tapram      Func
  ///  @tapram      Args...
  ///  @param[in]   group     定时器组
  ///  @param[in]   id        定时器id
  ///  @param[in]   delay     延迟启动时间
  ///  @param[in]   interval  触发间隔
  ///  @param[in]   cycle     循环次数 -1 为永久
  ///  @param[in]   func      定时任务
  ///  @param[in]   args...   定时任务参数
  template <typename Rep, typename Period, typename Func, typename... Args>
  TPN_INLINE void StartDelayCycleTimer(
      TimerGroup group, TimerId id, std::chrono::duration<Rep, Period> delay,
      std::chrono::duration<Rep, Period> interval, int cycle, Func &&func,
      Args &&...args) {
    Derived &derive = CRTP_CAST(this);

    std::function<void()> task =
        ForwardAsLambda(std::forward<Func>(func), std::forward<Args>(args)...);

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [this, &derive, this_ptr = derive.GetSelfSptr(),
                       key  = UserTimerKey(group, id), delay, interval, cycle,
                       task = std::move(task)]() mutable {
                        std::shared_ptr<UserTimerObj> timer_obj_sptr;
                        auto iter = this->user_timers_.find(key);
                        if (this->user_timers_.end() != iter) {
                          timer_obj_sptr       = iter->second;
                          timer_obj_sptr->task = std::move(task);
                        } else {
                          timer_obj_sptr = std::make_shared<UserTimerObj>(
                              key, derive.GetIoHandle().GetIoContext(),
                              std::move(task), cycle);

                          this->user_timers_[std::move(key)] = timer_obj_sptr;
                        }

                        derive.PostDelayUserTimers(std::move(timer_obj_sptr),
                                                   delay, interval,
                                                   std::move(this_ptr));
                      }));
  }

  /// 关闭定时器
  ///  @param[in]   id
  TPN_INLINE void StopTimer(TimerId id) {
    Derived &derive = CRTP_CAST(this);
    derive.StopTimer(TimerGroup::kTimerGroupCommon, std::move(id));
  }

  /// 关闭定时器
  ///  @param[in]   group
  ///  @param[in]   id
  TPN_INLINE void StopTimer(TimerGroup group, TimerId id) {
    Derived &derive = CRTP_CAST(this);

    if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      return asio::post(
          derive.GetIoHandle().GetStrand(),
          MakeAllocator(
              derive.GetWriteAllocator(),
              [this, this_ptr = derive.GetSelfSptr(), group = std::move(group),
               id = std::move(id)]() mutable {
                this->StopTimer(std::move(group), std::move(id));
              }));
    }

    UserTimerKey key(group, id);
    auto iter = this->user_timers_.find(key);
    if (this->user_timers_.end() != iter) {
      iter->second->exited = true;
      iter->second->timer.cancel(s_ec_ignore);
      this->user_timers_.erase(iter);
    }
  }

  /// 关闭定时器
  ///  @param[in]   group
  TPN_INLINE void StopGroupTimers(TimerGroup group) {
    Derived &derive = CRTP_CAST(this);

    if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      return asio::post(
          derive.GetIoHandle().GetStrand(),
          MakeAllocator(derive.GetWriteAllocator(),
                        [this, this_ptr = derive.GetSelfSptr(),
                         group]() mutable { this->StopGroupTimers(group); }));
    }

    for (auto &[key, timer_obj_sptr] : this->user_timers_) {
      if (group == static_cast<uint16_t>(key >> 16)) {
        timer_obj_sptr->exited = true;
        timer_obj_sptr->timer.cancel(s_ec_ignore);
      }
    }

    for (auto iter = this->user_timers_.begin();
         iter != this->user_timers_.end();) {
      if (group == iter->first.group) {
        iter->second->exited = true;
        iter->second->timer.cancel(s_ec_ignore);
        iter = this->user_timers_.erase(iter);
      } else {
        ++iter;
      }
    }
  }

  /// 关闭定时器
  TPN_INLINE void StopAllTimers() {
    Derived &derive = CRTP_CAST(this);

    if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      return asio::post(
          derive.GetIoHandle().GetStrand(),
          MakeAllocator(derive.GetWriteAllocator(),
                        [this, this_ptr = derive.GetSelfSptr()]() mutable {
                          this->StopAllTimers();
                        }));
    }

    for (auto &[_, timer_obj_sptr] : this->user_timers_) {
      timer_obj_sptr->exited = true;
      timer_obj_sptr->timer.cancel(s_ec_ignore);
    }

    this->user_timers_.clear();
  }

 protected:
  /// 提交一个用户自定义定时器
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   timer_obj_sptr
  ///  @param[in]   interval
  ///  @param[in]   this_ptr
  template <typename Rep, typename Period>
  TPN_INLINE void PostUserTimers(std::shared_ptr<UserTimerObj> timer_obj_sptr,
                                 std::chrono::duration<Rep, Period> interval,
                                 std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    if (timer_obj_sptr->exited) {
      return;
    }

    auto &timer = timer_obj_sptr->timer;

    timer.expires_after(interval);
    timer.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [&derive, timer_obj_sptr = std::move(timer_obj_sptr),
                       interval, self_ptr      = std::move(this_ptr)](
                          const std::error_code &ec) mutable {
                        derive.HandleUserTimers(ec, std::move(timer_obj_sptr),
                                                interval, std::move(self_ptr));
                      })));
  }

  /// 提交一个用户自定义延迟定时器
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   timer_obj_sptr
  ///  @param[in]   delay
  ///  @param[in]   interval
  ///  @param[in]   this_ptr
  template <typename Rep, typename Period>
  TPN_INLINE void PostDelayUserTimers(
      std::shared_ptr<UserTimerObj> timer_obj_sptr,
      std::chrono::duration<Rep, Period> delay,
      std::chrono::duration<Rep, Period> interval,
      std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    if (timer_obj_sptr->exited) {
      return;
    }

    auto &timer = timer_obj_sptr->timer;

    timer.expires_after(delay);
    timer.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(),
                      [&derive, timer_obj_sptr = std::move(timer_obj_sptr),
                       interval, self_ptr      = std::move(this_ptr)](
                          const std::error_code &ec) mutable {
                        derive.HandleUserTimers(ec, std::move(timer_obj_sptr),
                                                interval, std::move(self_ptr));
                      })));
  }

  /// 处理一个用户自定义定时器
  ///  @tapram      Rep
  ///  @tapram      Period
  ///  @param[in]   timer_obj_sptr
  ///  @param[in]   interval
  ///  @param[in]   this_ptr
  template <typename Rep, typename Period>
  TPN_INLINE void HandleUserTimers(const std::error_code &ec,
                                   std::shared_ptr<UserTimerObj> timer_obj_sptr,
                                   std::chrono::duration<Rep, Period> interval,
                                   std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    SetLastError(ec);

    (timer_obj_sptr->task)();

    if (asio::error::operation_aborted == ec) {
      derive.StopTimer(timer_obj_sptr->key.group, timer_obj_sptr->key.id);
      return;
    }

    if (kUserTimerCycleForever != timer_obj_sptr->cycle.load()) {
      if (timer_obj_sptr->cycle.load() <= 1) {
        derive.StopTimer(timer_obj_sptr->key.group, timer_obj_sptr->key.id);
        return;
      }
      --(timer_obj_sptr->cycle);
    }

    derive.PostUserTimers(std::move(timer_obj_sptr), interval,
                          std::move(this_ptr));
  }

 protected:
  UserTimerMap user_timers_;  ///< 用户自定义定时器
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIMER_USER_TIMER_H_
