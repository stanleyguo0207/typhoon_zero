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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_POST_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_POST_WRAP_H_

#include <set>
#include <future>
#include <functional>

#include "net_common.h"
#include "io_pool.h"
#include "custom_allocator.h"

namespace tpn {

namespace net {

/// asio::post封装
///  @tparam  Derived     CRTP对象
///  @tparam  ArgsType    占位
template <typename Derived, typename ArgsType = void>
class PostWrap {
 public:
  PostWrap()  = default;
  ~PostWrap() = default;

  /// 提交函数对象以执行
  /// 此函数使用指定的执行程序提交对象以供执行。 函数对象已排队等待执行，
  /// 并且从 asio::post() 返回之前，不会从当前线程中调用该函数对象。
  ///  @tparam      Func    异步提交的函数类型
  ///  @param[in]   func    异步提交的函数
  /// @sa asio::post
  template <typename Func>
  inline Derived &Post(Func &&func) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Post {}", typeid(func).name());

    asio::post(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(), std::forward<Func>(func)));
    return (derive);
  }

  /// 提交函数对象以延迟指定时间执行
  /// 此函数使用指定的执行程序提交对象以供执行。 函数对象已排队等待执行，
  /// 并且从 asio::post() 返回之前，不会从当前线程中调用该函数对象。
  ///  @tparam      Func    任务函数类型
  ///  @tparam      Rep     表示计次数的算术类型
  ///  @tparam      Period  表示计次周期的 std::ratio （即每秒的次数）
  ///  @param[in]   func    任务函数
  ///  @param[in]   delay   延迟周期
  /// @sa asio::post
  template <typename Func, typename Rep, typename Period>
  inline Derived &Post(Func &&func, std::chrono::duration<Rep, Period> delay) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Post {} delay {}", typeid(func).name(), delay);

    auto timer = std::make_unique<asio::steady_timer>(
        derive.GetIoHandle().GetIoContext());

    asio::dispatch(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(
            derive.GetWriteAllocator(),
            [this, self = derive.GetSelfSptr(), key = timer.get()]() mutable {
              NET_DEBUG("PostWrap Post asio::dispatch {} to task set", key);
              this->timed_tasks_.emplace(key);
            }));

    timer.expires_after(delay);
    timer.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(
            derive.GetWriteAllocator(),
            [this, self = derive.GetSelfSptr(), timer = std::move(timer),
             func =
                 std::forward<Func>(func)](const std::error_code &ec) mutable {
              NET_DEBUG("PostWrap Post async_wait {} error {}",
                        typeid(func).name(), ec);
              func();
              this->timed_tasks_.erase(timer.get());
            })));

    return (derive);
  }

  /// 提交函数对象以执行
  /// 此函数使用指定的执行程序提交对象以供执行。 函数对象已排队等待执行，
  /// 并且从 asio::post() 返回之前，不会从当前线程中调用该函数对象。
  /// 注意：切勿在通信（例如：on_recv）线程中调用future的等待函数（例如：wait，get），这将导致死锁；
  ///  @tparam      Func        任务函数类型
  ///  @tparam      Allocator   分配器 用来将来调用分配内存使用
  ///  @param[in]   func        任务函数
  template <typename Func, typename Allocator>
  inline decltype(auto) Post(Func &&func, asio::use_future_t<Allocator>) {
    using ReturnType = std::invoke_result_t<Func>;

    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Post {} asio::use_future_t", typeid(func).name());

    std::packaged_task<ReturnType()> task(std::forward<Func>(func));

    std::future<ReturnType> ret = task.get_future();

    asio::post(derive.GetIoHandle().GetStrand(),
               MakeAllocator(derive.GetWriteAllocator(),
                             [task = std::move(task)]() mutable { task(); }));

    return ret;
  }

  /// 提交函数对象以延迟指定时间执行
  /// 此函数使用指定的执行程序提交对象以供执行。 函数对象已排队等待执行，
  /// 并且从 asio::post() 返回之前，不会从当前线程中调用该函数对象。
  /// 注意：切勿在通信（例如：on_recv）线程中调用future的等待函数（例如：wait，get），这将导致死锁；
  ///  @tparam      Func        任务函数类型
  ///  @tparam      Rep         表示计次数的算术类型
  ///  @tparam      Period      表示计次周期的 std::ratio （即每秒的次数）
  ///  @tparam      Allocator   分配器 用来将来调用分配内存使用
  ///  @param[in]   func        任务函数
  ///  @param[in]   delay       延迟周期
  template <typename Func, typename Rep, typename Period, typename Allocator>
  inline decltype(auto) Post(Func &&func,
                             std::chrono::duration<Rep, Period> delay,
                             asio::use_future_t<Allocator>) {
    using ReturnType = std::invoke_result_t<Func>;

    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Post {} asio::use_future_t delay {}",
              typeid(func).name(), delay);

    std::packaged_task<ReturnType()> task(std::forward<Func>(func));

    std::future<ReturnType> ret = task.get_future();

    auto timer = std::make_unique<asio::steady_timer>(
        derive.GetIoHandle().GetIoContext());

    asio::dispatch(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(), [this,
                                                   self = derive.GetSelfSptr(),
                                                   key =
                                                       timer.get()]() mutable {
          NET_DEBUG(
              "PostWrap Post asio::use_future_t asio::dispatch {} to task set",
              key);
          this->timed_tasks_.emplace(key);
        }));

    timer.expires_after(delay);
    timer.async_wait(asio::bind_executor(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(
            derive.GetWriteAllocator(),
            [this, self = derive.GetSelfSptr(), timer = std::move(timer),
             task = std::move(task)](const std::error_code &ec) mutable {
              NET_DEBUG("PostWrap Post async_wait {} error {}",
                        typeid(task).name(), ec);
              task();
              this->timed_tasks_.erase(timer.get());
            })));

    return ret;
  }

  /// 提交函数对象以执行
  /// 此函数使用对象的关联执行程序提交对象以供执行。 功能对象排队等待执行。
  /// 如果当前线程是该线程的线程，则该函数将立即执行，否则该任务将异步发布到该线程以执行。
  ///  @tparam      Func    异步提交的函数类型
  ///  @param[in]   func    异步提交的函数
  template <typename Func>
  inline Derived &Dispatch(Func &&func) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Dispatch {}", typeid(func).name());

    asio::dispatch(
        derive.GetIoHandle().GetStrand(),
        MakeAllocator(derive.GetWriteAllocator(), std::forward<Func>(func)));
    return (derive);
  }

  /// 提交函数对象以执行
  /// 此函数使用对象的关联执行程序提交对象以供执行。 功能对象排队等待执行。
  /// 如果当前线程是该线程的线程，则该函数将立即执行，否则该任务将异步发布到该线程以执行。
  /// 注意：切勿在通信（例如：on_recv）线程中调用future的等待函数（例如：wait，get），这将导致死锁；
  ///  @tparam      Func        异步提交的函数类型
  ///  @tparam      Allocator   分配器 用来将来调用分配内存使用
  ///  @param[in]   func        异步提交的函数
  template <typename Func, typename Allocator>
  inline decltype(auto) Dispatch(Func &&func, asio::use_future_t<Allocator>) {
    using ReturnType = std::invoke_result_t<Func>;

    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostWrap Dispatch asio::use_future_t {}", typeid(func).name());

    std::packaged_task<ReturnType()> task(std::forward<Func>(func));

    std::future<ReturnType> ret = task.get_future();

    if (derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      task();
    } else {
      asio::post(derive.GetIoHandle().GetStrand(),
                 MakeAllocator(derive.GetWriteAllocator(),
                               [task = std::move(task)]() mutable { task(); }));
    }

    return ret;
  }

  /// 停止所有计划的定时器
  inline Derived &StopAllPostedTasks() {
    Derived &derive = CRTP_CAST(this);

    if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "PostWrap StopAllPostedTasks not running in this thread asio::post "
          "to "
          "all");
      asio::post(
          derive.GetIoHandle().GetStrand(),
          MakeAllocator(derive.GetWriteAllocator(),
                        [this, this_ptr = derive.GetSelfSptr()]() mutable {
                          this->StopAllPostedTasks();
                        }));
      return (derive);
    }

    NET_DEBUG("PostWrap StopAllPostedTasks timed_task num {}",
              this->timed_tasks_.size());

    for (auto &timer : this->timed_tasks_) {
      timer->cancel(s_ec_ignore);
    }

    return (derive);
  }

 protected:
  std::set<asio::steady_timer *> timed_tasks_;  ///< 准备停止时，停止计时任务
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_POST_WRAP_H_
