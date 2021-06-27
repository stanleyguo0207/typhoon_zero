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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_IO_POOL_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_IO_POOL_H_

#include <memory>

#include "net_common.h"

namespace tpn {

namespace net {

/// asio操作句柄，用来封装asio上下文与线程串行保护
class TPN_NET_API IoHandle {
 public:
  IoHandle() : context_(1), strand_(context_) {}
  ~IoHandle() = default;

  inline asio::io_context &GetIoContext() { return this->context_; }
  inline asio::io_context::strand &GetStrand() { return this->strand_; }

 private:
  asio::io_context context_;         ///< asio::io_context
  asio::io_context::strand strand_;  ///< asio::io_context::strand
};

/// io_context对象池
class TPN_NET_API IoPool {
 public:
  /// 构造函数
  ///  @param[in]  concurrency_hint    对象池的大小，默认是两倍的cpu核数
  IoPool(size_t concurrency_hint = std::thread::hardware_concurrency() * 2);

  /// 析构函数
  ~IoPool();

  /// 启动io_context对象池
  ///  @return 是否启动成功
  bool IoPoolStart();

  /// 关闭io_context对象池
  /// 停止阻塞io_context对象池中的所有对象。直到所有发布的事件都已完成。在调用workers_.reset()
  /// 之后，当asio::post(strand,...)执行结束时，将检查strand的计数。如果计数等于0，则该线程
  /// 将结束。然后，对asio::post(strand,...)的调用将失败，并且不执行post事件。当我们退出程序时，
  /// 它将嵌套调用asio::post(strand,...)来发布许多事件，因此，当某个asio::post(strand,...)位于
  /// asio::post(strand,...)尚未执行时，该链可能已经关闭。这将导致嵌套asio::post(stand,...)
  /// 永远不会执行。
  void IoPoolStop();

  /// 检查io_context对象池是否已经启动
  /// @return 是否已经启动
  bool IsIoPoolStarted() const;

  /// 检查io_context对象池是否已经关闭
  /// @return 是否已经关闭
  bool IsIoPoolStopped() const;

  /// 获取一个可用的io_context对象
  ///  @param[in]   index     指定编号，默认取下一个可用的
  ///  @return 可用的io_context对象
  IoHandle &GetIoHandleByIndex(size_t index = static_cast<size_t>(-1));

 private:
  class IoPoolImpl;
  std::unique_ptr<IoPoolImpl> io_pool_uptr_;  ///< io_context 对象池
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_IO_POOL_H_
