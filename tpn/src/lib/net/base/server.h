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
#include "message_buffer.h"
#include "net_common.h"
#include "io_pool.h"
#include "session_mgr.h"
#include "custom_allocator.h"
#include "post_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

/// 网络层服务器基类
///  @tparam  Derived     服务器子类
///  @tparam  SessionType 网络层会话类型
template <typename Derived, typename SessionType>
class ServerBase : public CRTPObject<Derived, false>,
                   public IoPool,
                   public PostWrap<Derived> {
  TPN_NET_FRIEND_DECL_BASE_CLASS

 public:
  using Super = CRTPObject<Derived, false>;
  using Self  = ServerBase<Derived, SessionType>;

  /// 构造函数
  ///  @param[in]   concurrency_hint    io对象池大小 默认 cpu个数 x2
  explicit ServerBase(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2)
      : Super(),
        IoPool(concurrency_hint),
        PostWrap<Derived>(),
        io_handle_(GetIoHandleByIndex(0)),
        session_mgr_(io_handle_),
        rallocator_(),
        wallocator_() {}

  ~ServerBase() = default;

  /// 基类服务器启动
  TPN_INLINE bool Start() {
    NET_DEBUG("ServerBase Start state {}", ToNetStateStr(this->state_));
    return true;
  }

  /// 基类服务器关闭
  TPN_INLINE void Stop() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "ServerBase Stop not running in this thread PostWrap to strand io");
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Stop(); });
      return;
    }

    NET_DEBUG("ServerBase Stop state {}", ToNetStateStr(this->state_));

    // 停止所有post中的任务
    this->StopAllPostedTasks();
  }

  /// 服务器是否启动
  ///  @return 启动返回true
  TPN_INLINE bool IsStarted() {
    return NetState::kNetStateStarted == this->state_;
  }

  /// 服务器是否关闭
  ///  @return 关闭返回true
  TPN_INLINE bool IsStopped() {
    return NetState::kNetStateStopped == this->state_;
  }

  /// 获取服务器中注册的网络会话数量
  ///  @return 服务器中注册的网络会话数量
  TPN_INLINE size_t GetSessionCount() { return this->session_mgr_.GetSize(); }

  /// 对注册的网络会话进行函数操作
  ///  @param[in]   fn      函数操作 函数签名 void(std::shared_ptr<SessionType> &)
  ///  @return CRTP调用链对象
  TPN_INLINE Derived &ApplyAllSession(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    this->session_mgr_.ApplyAll(fn);
    return this->GetDerivedObj();
  }

  /// 对注册过的网络会话进行条件查找
  ///  @param[in]   fn      函数操作 函数签名 void(std::shared_ptr<SessionType> &)
  ///  @return 满足条件的对象，找不到返回空对象
  TPN_INLINE std::shared_ptr<SessionType> FindSessionIf(
      const std::function<void(std::shared_ptr<SessionType> &)> &fn) {
    return std::shared_ptr<SessionType>(this->session_mgr_.FindIf(fn));
  }

  /// 服务分发
  ///  @param[in]   session_sptr
  ///  @param[in]   service_hash
  ///  @param[in]   token
  ///  @param[in]   method_id
  ///  @param[in]   buffer
  TPN_INLINE void ServiceDispath(std::shared_ptr<SessionType> session_sptr,
                                 uint32_t service_hash, uint32_t token,
                                 uint32_t method_id, MessageBuffer buffer) {
    this->service_mgr_.Dispatch(session_sptr, service_hash, token, method_id,
                                std::move(buffer));
  }

  /// 获取网络接受器
  ///  @return 网络接受器
  TPN_INLINE auto &GetAcceptor() { return this->GetDerivedObj().GetAcceptor(); }

  /// 获取网络监听地址
  ///  @return 网络监听地址
  TPN_INLINE std::string GetListenAddress() {
    try {
      return this->GetAcceptor().local_endpoint().address().to_string();
    } catch (std::system_error &e) {
      NET_ERROR("ServerBase GetListenAddress error {}", e.code());
      SetLastError(e);
    }

    return std::string();
  }

  /// 获取网络监听端口
  ///  @return 网络监听端口
  TPN_INLINE unsigned short GetListenPort() {
    try {
      return this->GetAcceptor().local_endpoint().port();
    } catch (std::system_error &e) {
      NET_ERROR("ServerBase GetListenPort error {}", e.code());
      SetLastError(e);
    }

    return static_cast<unsigned short>(0);
  }

  /// 获取io句柄
  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

 protected:
  /// 获取网络状态
  ///  @return 当前服务器网络状态
  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

  /// 获取服务器会话管理器
  ///  @return 服务器会话管理器
  TPN_INLINE SessionMgr<SessionType> &GetSessionMgr() {
    return this->session_mgr_;
  }

  /// 获取用于 acceptor 的内存分配器
  TPN_INLINE auto &GetReadAllocator() { return this->rallocator_; }

  /// 获取用于 send/write/post 的内存分配器
  TPN_INLINE auto &GetWriteAllocator() { return this->wallocator_; }

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
