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

/// 网络会话管理器
///  @tparam  SessionType     网络会话类型
template <typename SessionType>
class SessionMgr {
 public:
  using key_type = typename SessionType::key_type;

  using Self = SessionMgr<SessionType>;

  /// 构造函数
  ///  @param[in]   io_handle     io句柄引用
  explicit SessionMgr(IoHandle &io_handle) : acceptor_io_handle_(io_handle) {
    this->sessions_.reserve(64);
  }

  ~SessionMgr() = default;

  /// 插入一个网络会话
  ///  @param[in]   session_sptr    网络层会话
  ///  @param[in]   callback        插入是否成功的回调函数
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

  /// 移除一个网络会话
  ///  @param[in]   session_sptr    网络层会话
  ///  @param[in]   callback        删除是否成功的回调函数
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
        erased = (this->sessions_.erase(session_sptr->GetHashKey()) > 0);
      }
    }

    (callback)(erased);
  }

  /// 提交一个任务
  ///  @param[in]   task    任务
  TPN_INLINE void Post(std::function<void()> task) {
    asio::post(this->acceptor_io_handle_.GetStrand(),
               MakeAllocator(this->allocator_, std::move(task)));
  }

  /// 对注册的网络会话进行函数操作
  ///  @param[in]   fn      函数操作 函数签名 void(std::shared_ptr<SessionType> &)
  TPN_INLINE void ApplyAll(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    for (auto &[_, session_sptr] : this->sessions_) {
      fn(session_sptr);
    }
  }

  /// 对注册过的网络会话查找
  ///  @param[in]   key     网络会话哈希key
  TPN_INLINE std::shared_ptr<SessionType> Find(const key_type &key) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    auto iter = this->sessions_.find(key);
    return (this->sessions_.end() == iter) ? std::shared_ptr<SessionType>()
                                           : iter->second;
  }

  /// 对注册过的网络会话进行条件查找
  ///  @param[in]   fn      函数操作 函数签名 void(std::shared_ptr<SessionType> &)
  TPN_INLINE std::shared_ptr<SessionType> FindIf(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    std::shared_lock<std::shared_mutex> guard(this->mutex_);
    auto iter =
        std::find_if(this->sessions_.begin(), this->sessions_.end(),
                     [this, &fn](auto &pair) { return fn(pair.second); });
    return (this->sessions_.end() == iter) ? std::shared_ptr<SessionType>()
                                           : iter->second;
  }

  /// 获取网络会话管理器中注册的会话数量
  ///  @return 网络会话管理器中注册的会话数量
  TPN_INLINE size_t GetSize() { return this->sessions_.size(); }

  /// 网络会话管理器中是否有会话注册过
  ///  @return 网络会话管理器为空时返回true
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
