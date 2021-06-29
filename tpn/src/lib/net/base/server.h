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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_

#include <atomic>
#include <memory>
#include <functional>
#include <type_traits>

#include "crtp_object.h"
#include "net_common.h"
#include "io_pool.h"
#include "session_mgr.h"
#include "custom_allocator.h"

namespace tpn {

namespace net {

template <typename Derived, typename SessionType>
class ServerBase : public CRTPObject<Derived, false>, public IoPool {
 public:
  using Super = CRTPObject<Derived, false>;
  using Self  = ServerBase<Derived, SessionType>;

  explicit ServerBase(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2)
      : Super(),
        IoPool(concurrency_hint),
        io_handle_(GetIoHandleByIndex(0)),
        session_mgr_(io_handle_),
        rallocator_(),
        wallocator_() {}

  ~ServerBase() {}

  TPN_INLINE bool Start() { return true; }

  TPN_INLINE void Stop() {}

  TPN_INLINE bool IsStarted() {
    return this->state_ == NetState::kNetStateStarted;
  }

  TPN_INLINE bool IsStopped() {
    return this->state_ == NetState::kNetStateStopped;
  }

  TPN_INLINE size_t GetSessionCount() { return this->session_mgr_.GetSize(); }

  TPN_INLINE Derived &ForEachSession(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    this->session_mgr_.ForEach(fn);
    return this->GetDerivedObj();
  }

  TPN_INLINE std::shared_ptr<SessionType> FindSessionIf(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    return std::shared_ptr<SessionType>(this->session_mgr_.FindIf(fn));
  }

  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

 protected:
  TPN_INLINE auto &GetReadAllocator() { return this->rallocator_; }

  TPN_INLINE auto &GetWriteAllocator() { return this->wallocator_; }

  TPN_INLINE SessionMgr<SessionType> &GetSessionMgr() {
    return this->session_mgr_;
  }

  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

 protected:
  IoHandle &io_handle_;  ///< 包含(io_context和strand)，用来处理接受事件
  std::atomic<NetState> state_ = NetState::kNetStateStopped;  ///< 服务状态
  SessionMgr<SessionType> session_mgr_;  ///< 会话管理器
  std::shared_ptr<void>
      counter_sptr_;  ///< 用来确保服务器在所有会话停止后才停止
  HandlerMemory<SizeOp<>, std::false_type>
      rallocator_;  ///< 用户自定义内存用来处理 acceptor
  HandlerMemory<SizeOp<>, std::true_type>
      wallocator_;  ///< 用户自定义内存用来处理 send/write/post
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVER_H_
