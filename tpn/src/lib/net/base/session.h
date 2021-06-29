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

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType>
class SessionBase : public CRTPObject<Derived> {
 public:
  using Super      = CRTPObject<Derived>;
  using Self       = SessionBase<Derived, ArgsType>;
  using BufferType = typename ArgsType::BufferType;

  explicit SessionBase(IoHandle &rw_io_handle, SessionMgr<Derived> &session_mgr,
                       size_type max, size_type prepare)
      : Super(),
        io_handle_(rw_io_handle),
        session_mgr_(session_mgr),
        buffer_(max, prepare) {}

  ~SessionBase() {}

  TPN_INLINE void Stop() { this->counter_sptr_.reset(); }

  TPN_INLINE bool IsStarted() {}

  TPN_INLINE bool IsStopped() {}

  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

  TPN_INLINE BufferWrap<BufferType> &GetBuffer() { return this->buffer_; }

 protected:
  TPN_INLINE void Start() {}

  TPN_INLINE SessionMgr<Derived> &GetSessionMgr() { return this->session_mgr_; }

  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

 protected:
  IoHandle &io_handle_;  ///< 包含(io_context和strand)，用来处理接受事件
  std::atomic<NetState> state_ = NetState::kNetStateStopped;  ///< 服务状态
  SessionMgr<SessionType> &session_mgr_;  ///< 会话管理器
  std::shared_ptr<void>
      counter_sptr_;  ///< 用来确保服务器在所有会话停止后才停止
  bool in_session_mgr_{false};     ///< 是否在已连接的会话管理器中
  BufferWrap<BufferType> buffer_;  ///< 缓冲区
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_
