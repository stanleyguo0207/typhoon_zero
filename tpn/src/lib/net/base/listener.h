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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_LISTENER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_LISTENER_H_

#include <array>
#include <memory>
#include <functional>
#include <type_traits>

#include "net_common.h"
#include "utils.h"

namespace tpn {

namespace net {

/// 观察者基类
class ObserverBase {
 public:
  virtual ~ObserverBase() {}
};

/// 观察者
///  @tparam Args...  观察函数所需参数
template <typename... Args>
class Observer : public ObserverBase {
 public:
  using FuncType = std::function<void(Args...)>;

  /// 构造函数
  explicit Observer(const FuncType &func) : func_(func) {}

  /// 构造函数
  explicit Observer(FuncType &&func) : func_(std::move(func)) {}

  /// 拷贝构造函数
  explicit Observer(const Observer<Args...> &other) : func_(other.func_) {}

  /// 移动构造函数
  explicit Observer(Observer<Args...> &&other)
      : func_(std::move(other.func_)) {}

  /// 带观察对象的构造函数
  ///  @tparam      Func    观察函数类型
  ///  @tparam      Objects 观察对象类型
  ///  @param[in]   func    观察函数
  ///  @param[in]   objs    观察对象 0 或者 1个
  template <typename Func, typename... Objects>
  explicit Observer(Func &&func, Objects &&...objs) {
    static_assert(0 == sizeof...(Objects) || 1 == sizeof...(Objects),
                  "The class object parameters of FuncArgs &&... func_args can "
                  "only be none or one");
    this->Bind(std::forward<Func>(func), std::forward<Objects>(objs)...);
  }

  /// 绑定观察函数
  ///  @tparam      Func    观察函数类型
  ///  @param[in]   func    观察函数
  template <typename Func>
  inline void Bind(Func &&func) {
    this->func_ = FuncType(std::forward<Func>(func));
    NET_DEBUG("Observer bind {}", this->func_.target_type().name());
  }

  /// 带对象的绑定观察函数
  ///  @tparam      Func    观察函数类型
  ///  @tparam      Object  观察对象
  ///  @param[in]   func    观察函数
  ///  @param[in]   obj     观察对象 obj拥有func函数
  template <typename Func, typename Object>
  inline void Bind(Func &&func, Object &&obj) {
    if constexpr (std::is_pointer_v<Object>) {
      this->func_ = [fn = std::forward<Func>(func),
                     observer =
                         std::forward<Object>(obj)](Args &&...args) mutable {
        (observer->*fn)(std::forward<Args>(args)...);
      };
    } else if constexpr (std::is_reference_v<Object>) {
      this->func_ = [fn = std::forward<Func>(func),
                     observer =
                         std::forward<Object>(obj)](Args &&...args) mutable {
        (observer.*fn)(std::forward<Args>(args)...);
      };
    } else {
      // 非指针或非引用属于不合法行为
      TPN_ASSERT(false, "Listener Bind obj valid");
    }
    NET_DEBUG("Observer bind {}", this->func_.target_type().name());
  }

  /// 调用函数
  void operator()(Args &&...args) {
    if (this->func_) {
      this->func_(std::forward<Args>(args)...);
    }
  }

  /// 观察函数移动
  FuncType Move() { return std::move(this->func_); }

 private:
  FuncType func_;  ///< 观察函数
};

/// 监听器
class TPN_NET_API Listener {
 public:
  Listener()  = default;
  ~Listener() = default;

  /// 绑定监听对应事件对象
  ///  @tparam      T         观察者对象类型
  ///  @param[in]   event     事件
  ///  @param[in]   observer  观察者对象
  template <typename T>
  inline void Bind(ListenerEvent event, T &&observer) {
    NET_DEBUG("Listener bind {}", ToListenerEventStr(event));
    this->observers_[EnumToUnderlyType(event)] =
        std::unique_ptr<ObserverBase>(new T(std::forward<T>(observer)));
  }

  /// 对应事件通知
  ///  @tparam      Args      观察对象参数
  ///  @param[in]   event     事件
  ///  @param[in]   args      观察对象参数
  template <typename... Args>
  inline void Notify(ListenerEvent event, Args &&...args) {
    using ObserverType = Observer<Args...>;
    NET_DEBUG("Listener notify {}", ToListenerEventStr(event));

    ObserverType *observer_ptr = static_cast<ObserverType *>(
        this->observers_[EnumToUnderlyType(event)].get());
    if (observer_ptr) {
      (*observer_ptr)(std::forward<Args>(args)...);
    }
  }

  /// 获取指定事件的观察者
  ///  @param[in]   event     监听事件
  ///  @return 监听对应事件的观察者唯一指针
  inline std::unique_ptr<ObserverBase> &GetObserverByEvent(
      ListenerEvent event) {
    return this->observers_[EnumToUnderlyType(event)];
  }

 private:
  std::array<std::unique_ptr<ObserverBase>,
             EnumToUnderlyType(ListenerEvent::kListenerEventMax)>
      observers_;  ///< 各种事件监听器
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_LISTENER_H_
