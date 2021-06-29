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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_MGR_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_MGR_H_

#include <mutex>
#include <shared_mutex>
#include <memory>
#include <algorithm>
#include <functional>
#include <unordered_map>
#include <type_traits>

#include "net_common.h"
#include "custom_allocator.h"

namespace tpn {

namespace net {

template <typename SessionType>
class SessionMgr {
 public:
  using key_type = typename SessionType::key_type;

  using Self = SessionMgr<SessionType>;

  explicit SessionMgr(IoHandle &acceptor_io_handle)
      acceptor_io_handle_(acceptor_io_handle) {
    this->sessions_.reserve(64);
  }

  ~SessionMgr() = default;

  TPN_INLINE void Emplace(std::shared_ptr<SessionType> session_sptr,
                          std::function<void(bool)> callback) {
    if (!session_sptr) {
      return;
    }

    if (!this->acceptor_io_handle_.GetStrand().running_in_this_thread()) {
      return asio::post(
          this->acceptor_io_handle_.GetStrand(),
          MakeAllocator(
              this->allocator_, [this, session_sptr = std::move(session_sptr),
                                 callback = std::move(callback)]() mutable {
                this->Emplace(std::move(session_sptr), std::move(callback));
              }));
    }

    bool inserted = false;

    {
      std::unique_lock<std::shared_mutex> guard(this->mutex_);
      inserted =
          this->sessions_.try_emplace(session_sptr->GetHashKey(), session_sptr)
              .second;
      session_sptr->in_session_mgr_ = inserted;
    }

    (callback)(inserted);
  }

  TPN_INLINE void Erase(std::shared_ptr<SessionType> session_sptr,
                        std::function<void(bool)> callback) {
    if (!session_sptr) {
      return;
    }

    if (!this->acceptor_io_handle_.GetStrand().running_in_this_thread()) {
      return asio::post(
          this->acceptor_io_handle_.GetStrand(),
          MakeAllocator(
              this->allocator_, [this, session_sptr = std::move(session_sptr),
                                 callback = std::move(callback)]() mutable {
                this->Erase(std::move(session_sptr), std::move(callback));
              }));
    }

    bool erased = false;

    {
      std::unique_lock<std::shared_mutex> guard(this->mutex_);
      if (session_sptr->in_session_mgr_) {
        erased = (this->sessions_.erased(session_sptr->GetHashKey()) > 0);
      }
    }

    (callback)(erased);
  }

  TPN_INLINE void Post(std::function<void()> task) {
    asio::post(this->acceptor_io_handle_.GetStrand(),
               MakeAllocator(this->allocator_, std::move(task)));
  }

  TPN_INLINE void ForEach(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    for (auto &[_, session_sptr] : this->sessions_) {
      fn(session_sptr);
    }
  }

  TPN_INLINE std::shared_ptr<SessionType> Find(const key_type &key) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    auto iter = this->sessions_.find(key);
    return (this->sessions_.end() == iter) ? std::shared_ptr<SessionType>()
                                           : iter->second;
  }

  TPN_INLINE std::shared_ptr<SessionType> FindIf(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    auto iter =
        std::find_if(this->sessions_.begin(), this->sessions_.end(),
                     [this, &fn](auto &pair) { return fn(pair.second); });
    return (this->sessions_.end() == iter) ? std::shared_ptr<SessionType>()
                                           : iter->second;
  }

  TPN_INLINE size_t GetSize() { return this->sessions_.size(); }

  TPN_INLINE bool IsEmpty() { return this->sessions_.empty(); }

 protected:
  std::unordered_map<key_type, std::shared_ptr<SessionType>>
      sessions_;             ///< 会话哈希树，存放已经连接的会话
  std::shared_mutex mutex_;  ///< 使用rwlock确保会话树线程安全
  IoHandle &acceptor_io_handle_;                       ///< 接受io句柄
  HandlerMemory<SizeOp<>, std::true_type> allocator_;  ///< 用户自定义内存分配
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_MGR_H_
