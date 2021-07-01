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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_

#include <atomic>
#include <memory>

#include "crtp_object.h"
#include "buffer_wrap.h"
#include "net_common.h"
#include "io_pool.h"
#include "listener.h"
#include "socket_wrap.h"
#include "post_wrap.h"
#include "connect.h"
#include "disconnect.h"
#include "alive_time.h"
#include "connect_time.h"
#include "connect_timer.h"
#include "silence_timer.h"
#include "event_queue.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

template <typename Derived, typename ArgsType>
class SessionBase : public CRTPObject<Derived>,
                    public PostWrap<Derived, ArgsType>,
                    public Connect<Derived, ArgsType>,
                    public Disconnect<Derived, ArgsType>,
                    public AliveTime<Derived, ArgsType>,
                    public ConnectTime<Derived, ArgsType>,
                    public ConnectTimeout<Derived, ArgsType>,
                    public SilenceTimer<Derived, ArgsType>,
                    public EventQueue<Derived, ArgsType>,
                    public Socket<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS

 public:
  using buffer_type = typename ArgsType::buffer_type;

  using Super = CRTPObject<Derived>;
  using Self  = SessionBase<Derived, ArgsType>;

  template <typename... Args>
  explicit SessionBase(IoHandle &rw_io_handle, Listener &listener,
                       SessionMgr<Derived> &session_mgr, size_t buffer_max,
                       size_t buffer_prepare, Args &&...args)
      : Super(),
        PostWrap<Derived, ArgsType>(),
        Connect<Derived, ArgsType>(),
        Disconnect<Derived, ArgsType>(),
        AliveTime<Derived, ArgsType>(),
        ConnectTime<Derived, ArgsType>(),
        ConnectTimeout<Derived, ArgsType>(rw_io_handle),
        SilenceTimer<Derived, ArgsType>(rw_io_handle),
        EventQueue<Derived, ArgsType>(),
        Socket<Derived, ArgsType>(std::forward<Args>(args)...),
        io_handle_(rw_io_handle),
        listener_(listener),
        session_mgr_(session_mgr),
        buffer_(buffer_max, buffer_prepare) {}

  ~SessionBase() = default;

  TPN_INLINE void Stop() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "SessionBase Stop not running in this thread PostWrap to all "
          "context, session key: {}",
          this->GetDerivedObj().GetHashKey());
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Stop(); });
      return;
    }

    NET_DEBUG("SessionBase Stop state {}, session key: {}",
              ToNetStateStr(this->state_), this->GetDerivedObj().GetHashKey());

    this->StopSilenceTimer();

    this->StopConnectTimeoutTimer(asio::error::operation_aborted);

    this->StopAllPostedTasks();

    this->counter_sptr_.reset();
  }

  TPN_INLINE bool IsStarted() {
    return (NetState::kNetStateStarted == this->state_ &&
            this->socket_.lowest_layer().is_open());
  }

  TPN_INLINE bool IsStopped() {
    return (NetState::kNetStateStopped == this->state_ &&
            !this->socket_.lowest_layer().is_open());
  }

  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

  TPN_INLINE BufferWrap<buffer_type> &GetBuffer() { return this->buffer_; }

 protected:
  TPN_INLINE void Start() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "SessionBase Start not running in this thread PostWrap to all "
          "context, session key: {}",
          this->GetDerivedObj().GetHashKey());
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Start(); });
      return;
    }

    NET_DEBUG("SessionBase Start state {}, session key: {}",
              ToNetStateStr(this->state_), this->GetDerivedObj().GetHashKey());

    this->PostConnectTimeoutTimer(this->connect_timeout_, this->GetSelfSptr());
  }

  TPN_INLINE Listener &GetListener() { return this->listener_; }

  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

  TPN_INLINE SessionMgr<Derived> &GetSessionMgr() { return this->session_mgr_; }

 protected:
  IoHandle &io_handle_;  ///< 包含(io_context和strand)，用来处理接受事件
  Listener &listener_;                                        ///< 监听器
  std::atomic<NetState> state_ = NetState::kNetStateStopped;  ///< 服务状态
  SessionMgr<Derived> &session_mgr_;  ///< 会话管理器
  std::shared_ptr<void>
      counter_sptr_;  ///< 用来确保服务器在所有会话停止后才停止
  bool in_session_mgr_{false};      ///< 是否在已连接的会话管理器中
  BufferWrap<buffer_type> buffer_;  ///< 缓冲区
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_
