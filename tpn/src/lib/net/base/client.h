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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_CLIENT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_CLIENT_H_

#include <atomic>

#include "buffer_wrap.h"
#include "crtp_object.h"
#include "net_common.h"
#include "custom_allocator.h"
#include "io_pool.h"
#include "post_wrap.h"
#include "listener.h"
#include "connect.h"
#include "disconnect.h"
#include "alive_time.h"
#include "connect_time.h"
#include "connect_timer.h"
#include "reconnect_timer.h"
#include "event_queue.h"
#include "local_endpoint.h"
#include "defer_wrap.h"
#include "socket_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

template <typename Derived, typename ArgsType>
class ClientBase : public CRTPObject<Derived, false>,
                   public IoPool,
                   public PostWrap<Derived, ArgsType>,
                   public Connect<Derived, ArgsType>,
                   public Disconnect<Derived, ArgsType>,
                   public AliveTime<Derived, ArgsType>,
                   public ConnectTime<Derived, ArgsType>,
                   public ConnectTimeout<Derived, ArgsType>,
                   public ReconnectTimer<Derived, ArgsType>,
                   public EventQueue<Derived, ArgsType>,
                   public LocalEndpoint<Derived, ArgsType>,
                   public Socket<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS

 public:
  using buffer_type = typename ArgsType::buffer_type;

  using Super = CRTPObject<Derived, false>;
  using Self  = ClientBase<Derived, ArgsType>;

  template <typename... Args>
  explicit ClientBase(size_t concurrency_hint, size_t buffer_max,
                      size_t buffer_prepare, Args &&...args)
      : Super(),
        IoPool(concurrency_hint),
        PostWrap<Derived, ArgsType>(),
        Connect<Derived, ArgsType>(),
        Disconnect<Derived, ArgsType>(),
        AliveTime<Derived, ArgsType>(),
        ConnectTime<Derived, ArgsType>(),
        ConnectTimeout<Derived, ArgsType>(GetIoHandleByIndex(0)),
        ReconnectTimer<Derived, ArgsType>(GetIoHandleByIndex(0)),
        EventQueue<Derived, ArgsType>(),
        LocalEndpoint<Derived, ArgsType>(),
        Socket<Derived, ArgsType>(GetIoHandleByIndex(0).GetIoContext(),
                                  std::forward<Args>(args)...),
        io_handle_(GetIoHandleByIndex(0)),
        listener_(),
        rallocator_(),
        wallocator_(),
        buffer_(buffer_max, buffer_prepare) {}

  ~ClientBase() = default;

  TPN_INLINE bool Start(bool async_connect = true) { return true; }

  TPN_INLINE void Stop() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "ClientBase Stop not running in this thread PostWrap to strand io");
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Stop(); });
      return;
    }

    NET_DEBUG("ClientBase Stop state {}", ToNetStateStr(this->state_));

    // 停止重连定时器
    this->StopReconnectTimer();

    // 停止连接超时定时器
    this->StopConnectTimeoutTimer(asio::error::operation_aborted);

    // 关闭已经提交的异步任务
    this->StopAllPostedTasks();
  }

  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

  TPN_INLINE bool IsStarted() const {
    return (NetState::kNetStateStarted == this->state_ &&
            this->socket_.lowest_layer().is_open());
  }

  TPN_INLINE bool IsStopped() const {
    return (NetState::kNetStateStopped == this->state_ &&
            !this->socket_.lowest_layer().is_open());
  }

  TPN_INLINE BufferWrap<buffer_type> &GetBuffer() { return this->buffer_; }

 protected:
  TPN_INLINE Listener &GetListener() { return this->listener_; }

  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

  TPN_INLINE auto &GetReadAllocator() { return this->rallocator_; }

  TPN_INLINE auto &GetWriteAllocator() { return this->wallocator_; }

 protected:
  IoHandle &io_handle_;                                      ///< io_handle
  Listener listener_;                                        ///< 监听器
  std::atomic<NetState> state_{NetState::kNetStateStopped};  ///< 网络状态
  BufferWrap<buffer_type> buffer_;                           ///< 缓冲区
  HandlerMemory<> rallocator_;  ///< 用于接收的分配器
  HandlerMemory<SizeOp<>, std::true_type>
      wallocator_;  ///< 用于send/write/post的分配器
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_CLIENT_H_
