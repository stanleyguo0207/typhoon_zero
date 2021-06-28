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
///  @interface Allocate(size_t)     申请内存
///  @interface Deallocate(void *)   归还内存
///  @sa https://think-async.com/Asio/asio-1.18.0/src/examples/cpp11/allocation/server.cpp
template <typename SizeN       = SizeOp<s_default_allocator_size>,
          typename IsAtomicUse = std::false_type>
class HandlerMemory;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CUSTOM_ALLOCATOR_H_
