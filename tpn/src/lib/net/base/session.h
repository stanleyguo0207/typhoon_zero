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
#include "net_common.h"
#include "io_pool.h"
#include "post_wrap.h"
#include "connect.h"
#include "disconnect.h"
#include "alive_time.h"
#include "connect_time.h"
#include "connect_timeout_timer.h"
#include "silence_timer.h"
#include "event_queue.h"
#include "socket_wrap.h"
#include "buffer_wrap.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS

/// 网络层会话基类
///  @tparam  Derived     会话子类
///  @tparam  ArgsType    会话模板参数
template <typename Derived, typename ArgsType>
class SessionBase : public CRTPObject<Derived>,
                    public PostWrap<Derived, ArgsType>,
                    public Connect<Derived, ArgsType>,
                    public Disconnect<Derived, ArgsType>,
                    public AliveTime<Derived, ArgsType>,
                    public ConnectTime<Derived, ArgsType>,
                    public ConnectTimeoutTimer<Derived, ArgsType>,
                    public SilenceTimer<Derived, ArgsType>,
                    public EventQueue<Derived, ArgsType>,
                    public Socket<Derived, ArgsType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS

 public:
  using buffer_type = typename ArgsType::buffer_type;

  using Super = CRTPObject<Derived>;
  using Self  = SessionBase<Derived, ArgsType>;

  /// 构造函数
  ///  @tparam      Args...         套接字创建所需参数类型
  ///  @param[in]   io_handle       io句柄
  ///  @param[in]   session_mgr     所在的会话管理器引用
  ///  @param[in]   buffer_max      缓冲区最大长度
  ///  @param[in]   buffer_prepare  缓冲区准备长度
  ///  @param[in]   args...         套接字所需参数
  template <typename... Args>
  explicit SessionBase(IoHandle &io_handle, SessionMgr<Derived> &session_mgr,
                       size_t buffer_max, size_t buffer_prepare, Args &&...args)
      : Super(),
        PostWrap<Derived, ArgsType>(),
        Connect<Derived, ArgsType>(),
        Disconnect<Derived, ArgsType>(),
        AliveTime<Derived, ArgsType>(),
        ConnectTime<Derived, ArgsType>(),
        ConnectTimeoutTimer<Derived, ArgsType>(io_handle),
        SilenceTimer<Derived, ArgsType>(io_handle),
        EventQueue<Derived, ArgsType>(),
        Socket<Derived, ArgsType>(std::forward<Args>(args)...),
        io_handle_(io_handle),
        session_mgr_(session_mgr),
        buffer_(buffer_max, buffer_prepare) {}

  ~SessionBase() = default;

  /// 网络会话基类停止
  TPN_INLINE void Stop() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "SessionBase Stop not running in this thread PostWrap to strand io, "
          "session key: {}",
          this->GetDerivedObj().GetHashKey());
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Stop(); });
      return;
    }

    NET_DEBUG("SessionBase Stop state {}, session key: {}",
              ToNetStateStr(this->state_), this->GetDerivedObj().GetHashKey());

    // 停止静默定时器
    this->StopSilenceTimer();

    // 停止超时定时器
    this->StopConnectTimeoutTimer(asio::error::operation_aborted);

    // 停止所有post中的任务
    this->StopAllPostedTasks();

    // 减少一个引用计数器
    this->counter_sptr_.reset();
  }

  /// 会话是否启动
  ///  @return 启动返回true
  TPN_INLINE bool IsStarted() {
    return (NetState::kNetStateStarted == this->state_ &&
            this->socket_.lowest_layer().is_open());
  }

  /// 会话是否关闭
  ///  @return 关闭返回true
  TPN_INLINE bool IsStopped() {
    return (NetState::kNetStateStopped == this->state_ &&
            !this->socket_.lowest_layer().is_open());
  }

  /// 获取io句柄
  ///  @return io句柄
  TPN_INLINE IoHandle &GetIoHandle() { return this->io_handle_; }

  /// 获取缓冲区
  ///  @return 缓冲区
  TPN_INLINE BufferWrap<buffer_type> &GetBuffer() { return this->buffer_; }

 protected:
  /// 网络会话启动
  TPN_INLINE void Start() {
    if (!this->io_handle_.GetStrand().running_in_this_thread()) {
      NET_DEBUG(
          "SessionBase Start not running in this thread PostWrap to strand io, "
          "session key: {}",
          this->GetDerivedObj().GetHashKey());
      this->GetDerivedObj().Post(
          [this, this_ptr = this->GetSelfSptr()]() mutable { this->Start(); });
      return;
    }

    NET_DEBUG("SessionBase Start state {}, session key: {}",
              ToNetStateStr(this->state_), this->GetDerivedObj().GetHashKey());

    // 提交一个超时定时器
    // 子类需要先设置超时时长 SetConnectTimeoutDuration
    this->PostConnectTimeoutTimer(this->connect_timeout_, this->GetSelfSptr());
  }

  /// 获取网络状态
  ///  @return 网络状态
  TPN_INLINE std::atomic<NetState> &GetNetState() { return this->state_; }

  /// 返回登记的会话管理器
  ///  @return 会话登记的会话管理器
  TPN_INLINE SessionMgr<Derived> &GetSessionMgr() { return this->session_mgr_; }

 protected:
  IoHandle &io_handle_;  ///< 包含(io_context和strand)，用来处理接受事件
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
