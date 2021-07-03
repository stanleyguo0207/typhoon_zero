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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CUSTOM_ALLOCATOR_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CUSTOM_ALLOCATOR_H_

#include <memory>
#include <type_traits>
#include <utility>
#include <atomic>

#include "define.h"

namespace tpn {

namespace net {

namespace {
static constexpr size_t s_default_allocator_size = 1024;
}  // namespace

/// 内存操作类，封装了每次操作内存的长度
template <size_t N = s_default_allocator_size>
struct SizeOp {
  static constexpr size_t Size = N;  ///<  每次操作内存的大小
};

/// 用于管理用于基于处理程序的自定义分配的内存的类，它包含一个内存块，可以为分配请求返回该内存块。
/// 如果申请的内存呗使用，或者申请的大小超过内存处理程序的持有的内存大小，交给全局堆初始化内存
///  @tparam SizeN        申请大小
///  @tparam IsAtomicUse  是否使用atomic_flag 默认为std::false_type
///  @sa https://think-async.com/Asio/asio-1.18.0/src/examples/cpp11/allocation/server.cpp
template <typename SizeN       = SizeOp<s_default_allocator_size>,
          typename IsAtomicUse = std::false_type>
class HandlerMemory;

template <typename SizeN>
class HandlerMemory<SizeN, std::false_type> {
 public:
  explicit HandlerMemory() : in_use_(false) {}

  inline void *allocate(size_t size) {
    if (!in_use_ && size < sizeof(storage_)) {
      in_use_ = true;
      return &storage_;
    } else {
      return ::operator new(size);
    }
  }

  inline void deallocate(void *pointer) {
    if (pointer == &storage_) {
      in_use_ = false;
    } else {
      ::operator delete(pointer);
    }
  }

 private:
  std::aligned_storage_t<SizeN::Size> storage_;
  bool in_use_{false};

  TPN_NO_COPYABLE(HandlerMemory)
};

template <typename SizeN>
class HandlerMemory<SizeN, std::true_type> {
 public:
  HandlerMemory() { in_use_.clear(); }

  inline void *allocate(size_t size) {
    if (!in_use_.test_and_set() && size < sizeof(storage_)) {
      return &storage_;
    } else {
      return ::operator new(size);
    }
  }

  inline void deallocate(void *pointer) {
    if (pointer == &storage_) {
      in_use_.clear();
    } else {
      ::operator delete(pointer);
    }
  }

 private:
  std::aligned_storage_t<SizeN::Size> storage_;
  std::atomic_flag in_use_;

  TPN_NO_COPYABLE(HandlerMemory)
};

///  @sa asio::detail::asio_handler_allocate
///  @interface allocate(size_t)     申请内存
///  @interface deallocate(void *)   归还内存
template <typename T, typename N, typename B>
class HandlerAllocator {
 public:
  using value_type = T;

  explicit HandlerAllocator(HandlerMemory<N, B> &memory) : memory_(memory) {}

  template <typename _T, typename _N, typename _B>
  HandlerAllocator(const HandlerAllocator<_T, _N, _B> &other) noexcept
      : memory_(other.memory_) {}

  inline bool operator==(const HandlerAllocator &other) const noexcept {
    return &memory_ == &other.memory_;
  }

  inline bool operator!=(const HandlerAllocator &other) const noexcept {
    return !(*this == other);
  }

  inline T *allocate(size_t n) const {
    return static_cast<T *>(memory_.allocate(sizeof(T) * n));
  }

  inline void deallocate(T *p, size_t /* n */) const {
    return memory_.deallocate(p);
  }

 private:
  template <typename, typename, typename>
  friend class HandlerAllocator;

  HandlerMemory<N, B> &memory_;
};

template <typename Handler, typename N, typename B>
class CustomAllocHandler {
 public:
  using allocator_type = HandlerAllocator<Handler, N, B>;

  CustomAllocHandler(HandlerMemory<N, B> &memory, Handler &&handler)
      : memory_(memory), handler_(std::forward<Handler>(handler)) {}

  inline allocator_type get_allocator() const noexcept {
    return allocator_type(memory_);
  }

  template <typename... Args>
  inline void operator()(Args &&...args) {
    handler_(std::forward<Args>(args)...);
  }

 private:
  HandlerMemory<N, B> &memory_;
  Handler handler_;
};

/// 包裹处理程序对象分配内存，切勿在多线程中使用，否则会导致程序崩溃。
///  @tparam      Handler   处理程序类型
///  @tparam      N         分配大小
///  @tparam      B         是否使用std::atomic_flag
///  @param[in]   memory    持有内存
///  @param[in]   handler   处理程序
///  @return 用户自定义内存分配程序
template <typename Handler, typename N, typename B>
inline CustomAllocHandler<Handler, N, B> MakeAllocator(
    HandlerMemory<N, B> &memory, Handler &&handler) {
  return CustomAllocHandler<Handler, N, B>(memory,
                                           std::forward<Handler>(handler));
}

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CUSTOM_ALLOCATOR_H_
