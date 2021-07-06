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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_

#include "net_common.h"
#include "server.h"
#include "tcp_session.h"

namespace tpn {

namespace net {

TPN_NET_FORWARD_DECL_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_BASE_CLASS
TPN_NET_FORWARD_DECL_TCP_SERVER_CLASS

/// 网络层tcp服务器基类
///  @tparam  Derived     tcp服务器子类
///  @tparam  SessionType 网络层会话类型
template <typename Derived, typename SessionType>
class TcpServerBase : public ServerBase<Derived, SessionType> {
  TPN_NET_FRIEND_DECL_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_BASE_CLASS
  TPN_NET_FRIEND_DECL_TCP_SERVER_CLASS

 public:
  using session_type = SessionType;

  using Super = ServerBase<Derived, SessionType>;
  using Self  = TcpServerBase<Derived, SessionType>;

  /// 构造函数
  ///  @param[in]   concurrency_hint    io对象池大小 默认 cpu个数 x2
  ///  @param[in]   buffer_max          缓冲区最大长度
  ///  @param[in]   buffer_prepare      缓冲区准备长度
  explicit TcpServerBase(
      size_t concurrency_hint = std::thread::hardware_concurrency() * 2,
      size_t buffer_max       = (std::numeric_limits<size_t>::max)(),
      size_t buffer_prepare   = kTcpFrameSize)
      : Super(concurrency_hint),
        acceptor_(this->io_handle_.GetIoContext()),
        acceptor_timer_(this->io_handle_.GetIoContext()),
        counter_timer_(this->io_handle_.GetIoContext()),
        buffer_max_(buffer_max),
        buffer_prepare_(buffer_prepare) {}

  /// 析构函数
  ~TcpServerBase() { this->Stop(); }

  /// 启动tcp服务器
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <typename StrOrInt>
  TPN_INLINE bool Start(StrOrInt &&service) {
    return this->Start(std::string_view{}, std::forward<StrOrInt>(service));
  }

  /// 启动tcp服务器
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <typename String, typename StrOrInt>
  TPN_INLINE bool Start(String &&host, StrOrInt &&service) {
    NET_DEBUG("TcpServerBase Start {}:{} state {}", host, service,
              ToNetStateStr(this->state_));
    return this->GetDerivedObj().DoStart(std::forward<String>(host),
                                         std::forward<StrOrInt>(service));
  }

  /// 关闭tco服务器
  TPN_INLINE void Stop() {
    NET_DEBUG("TcpServerBase Stop state {}", ToNetStateStr(this->state_));
    this->GetDerivedObj().DoStop(asio::error::operation_aborted);
    this->IoPoolStop();
  }

  /// 服务器是否启动
  ///  @return 启动返回true
  TPN_INLINE bool IsStarted() {
    return (Super::IsStarted() && this->acceptor_.is_open());
  }

  /// 服务器是否关闭
  ///  @return 关闭返回true
  TPN_INLINE bool IsStopped() {
    return (Super::IsStopped() && this->acceptor_.is_open());
  }

  /// 获取接受器
  TPN_INLINE asio::ip::tcp::acceptor &GetAcceptor() { return this->acceptor_; }

 protected:
  /// tcp服务器执行开始
  ///  @tparam      String      字符串
  ///  @tparam      StrOrInt    字符串或整数
  ///  @param[in]   host        标识地址的字符串。可以是描述性名称或数字地址字符串。
  ///  @param[in]   service     标识请求的服务的字符串。
  ///                           可以是描述性名称，也可以是与端口号相对应的数字字符串。
  ///  @return 启动成功返回true
  template <typename String, typename StrOrInt>
  TPN_INLINE bool DoStart(String &&host, StrOrInt &&service) {
    NET_DEBUG("TcpServerBase DoStart {}:{} state{}", host, service,
              ToNetStateStr(this->state_));

    NetState expected = NetState::kNetStateStopped;
    if (!this->state_.compare_exchange_strong(expected,
                                              NetState::kNetStateStarting)) {
      NET_ERROR("TcpServerBase net already started state {}",
                ToNetStateStr(this->state_));
      SetLastError(asio::error::already_started);
      return false;
    }

    Super::Start();

    try {
      ClearLastError();

      // 启动对象池
      this->IoPoolStart();

      if (this->IsIoPoolStopped()) {
        NET_ERROR("TcpServerBase io_pool start error");
        SetLastError(asio::error::shut_down);
        return false;
      }

      // 确保引用计数为0时 做最后的关闭处理
      this->counter_sptr_ =
          std::shared_ptr<void>(reinterpret_cast<void *>(1), [this](void *) {
            this->GetDerivedObj().Post([this]() mutable {
              NET_DEBUG("TcpServerBase counter final stop state {}",
                        ToNetStateStr(this->state_));
              NetState expected = NetState::kNetStateStopping;
              if (this->state_.compare_exchange_strong(
                      expected, NetState::kNetStateStopped)) {
                NET_DEBUG("TcpServerBase counter final stop state {}",
                          ToNetStateStr(this->state_));
                this->GetDerivedObj().HandleStop(asio::error::operation_aborted,
                                                 this->GetSelfSptr());
              } else {
                TPN_ASSERT(false, "TcpServerBase counter final stop error");
              }
            });
          });

      this->acceptor_.close(s_ec_ignore);

      std::string h = ToString(std::forward<String>(host));
      std::string p = ToString(std::forward<StrOrInt>(service));

      // 解析地址端口
      asio::ip::tcp::resolver resolver(this->io_handle_.GetIoContext());
      asio::ip::tcp::endpoint endpoint =
          *resolver
               .resolve(h, p,
                        asio::ip::resolver_base::flags::passive |
                            asio::ip::resolver_base::flags::address_configured)
               .begin();

      NET_DEBUG("TcpServerBase {}:{} resolver {}:{}", h, p,
                endpoint.address().to_string(), endpoint.port());

      // 接收器开启
      this->acceptor_.open(endpoint.protocol());

      // 当您在linux系统中关闭套接字并立即启动套接字时，您将得到如下所示的“地址正在使用”，
      // 并且绑定失败，但是我很抱歉，我之前已经正确地关闭了套接字，为什么会这样？
      // 原因是套接字选项“ TIME_WAIT”，尽管您关闭了套接字，但是系统没有释放套接字，
      // 直到2到4秒后才释放套接字，所以我们可以使用SO_REUSEADDR选项来避免此问题，如下所示
      this->acceptor_.set_option(asio::ip::tcp::acceptor::reuse_address(true));

      // 通知启动成功
      this->GetDerivedObj().FireInit();

      // 绑定端口
      this->acceptor_.bind(endpoint);
      // 监听
      this->acceptor_.listen();

      NET_DEBUG("TcpServerBase DoStart HandleStart");
      this->GetDerivedObj().HandleStart(std::error_code{});

      NET_DEBUG("TcpServerBase DoStart {}:{} state {} done", host, service,
                ToNetStateStr(this->state_));
      return (this->IsStarted());
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase DoStart HandleStart error {}", e.code());
      this->GetDerivedObj().HandleStart(e.code());
    }
    return false;
  }

  /// tcp服务器处理启动
  ///  @param[in]   ec    错误码
  TPN_INLINE void HandleStart(std::error_code ec) {
    NET_DEBUG("TcpServerBase HandleStart error {} state {}", ec,
              ToNetStateStr(this->state_));

    try {
      NetState expected = NetState::kNetStateStarting;
      if (!ec && !this->state_.compare_exchange_strong(
                     expected, NetState::kNetStateStarted)) {
        NET_WARN("TcpServerBase HandleStart not starting state {}",
                 ToNetStateStr(this->state_));
        ec = asio::error::operation_aborted;
      }

      SetLastError(ec);

      // 无论启动成功或是失败，都需要通知Start事件
      this->GetDerivedObj().FireStart(ec);

      expected = NetState::kNetStateStarted;
      if (!ec && !this->state_.compare_exchange_strong(
                     expected, NetState::kNetStateStarted)) {
        NET_WARN("TcpServerBase HandleStart not started state {}",
                 ToNetStateStr(this->state_));
        asio::detail::throw_error(asio::error::operation_aborted);
      }

      asio::detail::throw_error(ec);

      NET_DEBUG("TcpServerBase HandleStart PostWrap PostAccept");
      this->GetDerivedObj().Post(
          [this]() mutable { this->GetDerivedObj().PostAccept(); });
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase HandleStart error {} state {}", e.code(),
                ToNetStateStr(this->state_));
      SetLastError(e);
      this->GetDerivedObj().DoStop(e.code());
    }
  }

  /// tcp服务器处理关闭
  ///  @param[in]   ec    错误码
  TPN_INLINE void DoStop(const std::error_code &ec) {
    NET_DEBUG("TcpServerBase DoStop error {} state {}", ec,
              ToNetStateStr(this->state_));

    // 启动中停止
    NetState expected = NetState::kNetStateStarting;
    if (this->state_.compare_exchange_strong(expected,
                                             NetState::kNetStateStopping)) {
      NET_DEBUG("TcpServerBase DoStop from {} change to {} state {}",
                ToNetStateStr(NetState::kNetStateStarting),
                ToNetStateStr(expected), ToNetStateStr(this->state_));
      return this->GetDerivedObj().PostStop(ec, this->GetSelfSptr(), expected);
    }

    // 已经启动停止
    expected = NetState::kNetStateStarted;
    if (this->state_.compare_exchange_strong(expected,
                                             NetState::kNetStateStopping)) {
      NET_DEBUG("TcpServerBase DoStop from {} change to {} state {}",
                ToNetStateStr(NetState::kNetStateStarted),
                ToNetStateStr(expected), ToNetStateStr(this->state_));
      return this->GetDerivedObj().PostStop(ec, this->GetSelfSptr(), expected);
    }
  }

  /// tcp服务器提交关闭
  ///  @param[in]   ec          错误码
  ///  @param[in]   self_ptr    延长生命周期的智能指针
  ///  @param[in]   old_state   转移到stopping之前的状态
  TPN_INLINE void PostStop(const std::error_code &ec,
                           std::shared_ptr<Derived> self_ptr,
                           NetState old_state) {
    NET_DEBUG("TcpServerBase PostStop error {} old_state {} state {}", ec,
              ToNetStateStr(old_state), ToNetStateStr(this->state_));

    // asio 不允许在多线程中操作同一个套接字，如果你在一个线程中关闭套接字，
    // 而另一个线程正在调用套接字的 async_... 函数，它会崩溃。
    // 所以我们必须注意操作套接字。 当需要关闭套接字时，
    // 我们使用链来发布一个事件，确保套接字的关闭操作在同一个线程中。
    this->GetDerivedObj().Post(
        [this, ec, this_ptr = std::move(self_ptr), old_state]() mutable {
          IgnoreUnused(this, old_state);

          SetLastError(ec);

          // 启动接受定时器
          this->counter_timer_.expires_after(NanoSeconds::max());
          this->counter_timer_.async_wait(asio::bind_executor(
              this->io_handle_.GetStrand(), [](const std::error_code &) {}));

          NET_DEBUG("TcpServerBase PostStop to apply all session to stop");
          // 停止所有会话，Session::Stop 必须没有阻塞，否则可能会导致循环锁定。
          this->GetDerivedObj().ApplyAllSession(
              [](std::shared_ptr<SessionType> &session_sptr) mutable {
                session_sptr->Stop();
              });

          // 引用计数归0
          this->counter_sptr_.reset();
        });
  }

  /// tcp服务器处理关闭
  ///  @param[in]   ec          错误码
  ///  @param[in]   self_ptr    延长生命周期的智能指针
  TPN_INLINE void HandleStop(const std::error_code &ec,
                             std::shared_ptr<Derived> this_ptr) {
    NET_DEBUG("TcpServerBase HandleStop error {} state {}", ec,
              ToNetStateStr(this->state_));

    IgnoreUnused(ec, this_ptr);

    // 通知停止
    this->GetDerivedObj().FireStop(ec);

    this->acceptor_timer_.cancel(s_ec_ignore);
    this->counter_timer_.cancel(s_ec_ignore);

    // 调用父类的关闭程序
    Super::Stop();

    // 调用acceptor的close函数通知HandleAccept函数响应error > 0，
    // 那么监听socket可以得到通知退出必须保证close函数已经被调用，
    // 否则_handle_accept永远不会返回
    this->acceptor_.close(s_ec_ignore);
  }

  /// 创造一个tcp会话
  ///  @tapram      Args        会话子类所需额外参数预留类型
  ///  @param[in]   args...     会话子类所需额外参数预留
  template <typename... Args>
  TPN_INLINE std::shared_ptr<SessionType> MakeSession(Args &&...args) {
    NET_DEBUG("TcpServerBase MakeSession state {}",
              ToNetStateStr(this->state_));
    return std::make_shared<SessionType>(
        std::forward<Args>(args)..., this->GetIoHandleByIndex(),
        this->session_mgr_, this->buffer_max_, this->buffer_prepare_);
  }

  /// 提交接受
  TPN_INLINE void PostAccept() {
    NET_DEBUG("TcpServerBase PostAccept state {}", ToNetStateStr(this->state_));

    if (!this->IsStarted()) {
      NET_ERROR("TcpServerBase PostAccept state {}",
                ToNetStateStr(this->state_));
      return;
    }

    try {
      std::shared_ptr<SessionType> session_sptr =
          this->GetDerivedObj().MakeSession();

      NET_DEBUG("TcpServerBase PostAccept new session {}",
                session_sptr->GetHashKey());

      auto &socket = session_sptr->GetSocket().lowest_layer();
      this->acceptor_.async_accept(
          socket, asio::bind_executor(
                      this->io_handle_.GetStrand(),
                      MakeAllocator(this->rallocator_,
                                    [this, sptr = std::move(session_sptr)](
                                        const std::error_code &ec) mutable {
                                      this->GetDerivedObj().HandleAccept(
                                          ec, std::move(sptr));
                                    })));
    } catch (std::system_error &e) {
      NET_ERROR("TcpServerBase PostAccept error {}", e.code());

      SetLastError(e);

      this->acceptor_timer_.expires_after(Seconds(1));
      this->acceptor_timer_.async_wait(asio::bind_executor(
          this->io_handle_.GetStrand(),
          MakeAllocator(
              this->rallocator_, [this](const std::error_code &ec) mutable {
                SetLastError(ec);
                if (ec) {
                  NET_ERROR("TcpServerBase PostAccept async_wait error {}", ec);
                  return;
                }
                asio::post(this->io_handle_.GetStrand(),
                           MakeAllocator(this->rallocator_, [this]() mutable {
                             this->GetDerivedObj().PostAccept();
                           }));
              })));
    }
  }

  /// tcp服务器处理接收
  ///  @param[in]   ec            错误码
  ///  @param[in]   session_sptr  网络会话
  TPN_INLINE void HandleAccept(const std::error_code &ec,
                               std::shared_ptr<SessionType> session_sptr) {
    NET_DEBUG("TcpServerBase HandleAccept error {} state {} session {} {}", ec,
              ToNetStateStr(this->state_), session_sptr->GetHashKey(),
              ToNetStateStr(session_sptr->GetNetState()));

    SetLastError(ec);

    if (asio::error::operation_aborted == ec) {
      NET_DEBUG("TcpServerBase HandleAccept aborted and do stop");
      this->GetDerivedObj().DoStop(ec);
      return;
    }

    if (!ec) {
      if (this->IsStarted()) {
        NET_DEBUG("TcpServerBase HandleAccept session {} start {}",
                  session_sptr->GetHashKey(),
                  ToNetStateStr(session_sptr->GetNetState()));
        session_sptr->counter_sptr_ = this->counter_sptr_;
        session_sptr->Start();
      }
    }

    NET_DEBUG("TcpServerBase HandleAccept error {} state {} session {} {}", ec,
              ToNetStateStr(this->state_), session_sptr->GetHashKey(),
              ToNetStateStr(session_sptr->GetNetState()));
    this->GetDerivedObj().PostAccept();
  }

  /// tcp服务器初始化通知
  TPN_INLINE void FireInit() {}

  /// tcp服务器启动通知
  TPN_INLINE void FireStart(std::error_code ec) {}

  /// tcp服务器停止通知
  TPN_INLINE void FireStop(std::error_code ec) {}

 protected:
  asio::ip::tcp::acceptor acceptor_;   ///< 接受者
  asio::steady_timer acceptor_timer_;  //< 用来处理接受者异常定时器
  asio::steady_timer counter_timer_;  ///< 确保持有的io_handle直到所有的会话关闭
  size_t buffer_max_{(std::numeric_limits<size_t>::max)()};  ///< 缓冲区最大值
  size_t buffer_prepare_{kTcpFrameSize};  ///< 缓冲区初始大小
};

/// tcp服务器桥梁
template <typename SessionType>
class TcpServerBridge
    : public TcpServerBase<TcpServerBridge<SessionType>, SessionType> {
 public:
  using TcpServerBase<TcpServerBridge<SessionType>, SessionType>::TcpServerBase;
};

/// tcp服务器
using TcpServer = TcpServerBridge<TcpSession>;

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_TCP_TCP_SERVER_H_
