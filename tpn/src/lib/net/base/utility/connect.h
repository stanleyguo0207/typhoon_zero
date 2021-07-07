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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CONNECT_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CONNECT_H_

#include "utils.h"
#include "net_common.h"

namespace tpn {

namespace net {

/// 连接模块基类
/// 客户端与会话均继承自此类
///  @tparam  Derived
///  @tparam  ArgsType
///  @tapram  is_session    是否为网络层会话
template <typename Derived, typename ArgsType, bool IsSession>
class ConnectBase;

/// 连接模块基类
/// 网络层会话
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType>
class ConnectBase<Derived, ArgsType, true> {};

/// 连接模块基类
/// 客户端
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType>
class ConnectBase<Derived, ArgsType, false> {
 public:
  using socket_type     = typename ArgsType::socket_type;  ///< 套接字类型
  using raw_socket_type = TPN_RMRF(socket_type);  ///< 套接字原生类型
  using resolver_type   = typename asio::ip::basic_resolver<
      typename raw_socket_type::protocol_type>;  ///< 解析器类型
  using endpoints_type = typename resolver_type::results_type;  ///< 端点类型
  using endpoints_iterator = typename endpoints_type::iterator;  ///< 端点迭代器

 protected:
  std::string host_;          ///<  远端地址
  std::string port_;          ///<  远端端口
  endpoints_type endpoints_;  ///<  端点
};

/// 连接模块
///  @tparam  Derived
///  @tparam  ArgsType
template <typename Derived, typename ArgsType>
class Connect : public ConnectBase<Derived, ArgsType, ArgsType::is_session> {
 public:
  using socket_type     = typename ArgsType::socket_type;  ///< 套接字类型
  using raw_socket_type = TPN_RMRF(socket_type);  ///< 套接字原生类型
  using resolver_type   = typename asio::ip::basic_resolver<
      typename raw_socket_type::protocol_type>;  ///< 解析器类型
  using endpoints_type = typename resolver_type::results_type;  ///< 端点类型
  using endpoints_iterator = typename endpoints_type::iterator;  ///< 端点迭代器

  using Self = Connect<Derived, ArgsType>;

  Connect()  = default;
  ~Connect() = default;

 protected:
  /// 连接模块开始连接
  /// 返回值特化为客户端
  ///  @tparam      IsAsync     是否为异步
  ///  @tparam      String      服务器地址类型
  ///  @tparam      StrOrInt    服务器端口类型
  ///  @tparam      IsSession   非网络会话
  ///  @param[in]   host        服务器地址
  ///  @param[in]   port        服务器端口
  ///  @param[in]   this_ptr    延长生命周期句柄
  template <bool IsAsync, typename String, typename StrOrInt,
            bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, bool> ClientStartConnect(
      String &&host, StrOrInt &&port, std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("Connect client [async {}] {}:{} => {}:{}", IsAsync,
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), host, port);

    try {
      if (derive.IsIoPoolStopped()) {
        NET_ERROR("Connect client [async {}] {}:{} => {}:{} io pool stopped",
                  IsAsync, derive.GetLocalEndpoint().address().to_string(),
                  derive.GetLocalEndpoint().port(), host, port);
        SetLastError(asio::error::shut_down);
        return false;
      }

      this->host_ = ToString(std::forward<String>(host));
      this->port_ = ToString(std::forward<StrOrInt>(port));

      // 获取底层套接字
      auto &socket = derive.GetSocket().lowest_layer();

      socket.close(s_ec_ignore);

      socket.open(derive.GetLocalEndpoint().protocol());

      // 设置端口复用
      socket.set_option(typename raw_socket_type::reuse_address(true));

      // tcp keeplive处理
      if constexpr (std::is_same_v<typename raw_socket_type::protocol_type,
                                   asio::ip::tcp>) {
        NET_DEBUG("Connect client [async {}] {}:{} => {}:{} tcp", IsAsync,
                  derive.GetLocalEndpoint().address().to_string(),
                  derive.GetLocalEndpoint().port(), host, port);
        // @sa TcpKeepAlive
        derive.SetKeepAliveOptions();
      } else {
        NET_DEBUG("Connect client [async {}] {}:{} => {}:{} not tcp", IsAsync,
                  derive.GetLocalEndpoint().address().to_string(),
                  derive.GetLocalEndpoint().port(), host, port);
        std::ignore = true;
      }

      // 监听器通知启动事件
      derive.FireInit();

      // 端口绑定
      socket.bind(derive.GetLocalEndpoint());

      if constexpr (IsAsync) {  // 异步连接
        // 开启超时定时器
        derive.Post([&derive, this_ptr]() mutable {
          NET_DEBUG(
              "Connect client [async {}] {}:{} post connect timeout timer",
              IsAsync, derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port());

          derive.PostConnectTimeoutTimer(
              derive.GetConnectTimeoutDuration(), this_ptr,
              [&derive, this_ptr](const std::error_code &ec) mutable {
                IgnoreUnused(this_ptr);

                NET_DEBUG(
                    "Connect client [async {}] {}:{} connect timeout timer "
                    "callback error_code: {}",
                    IsAsync, derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port(), ec);

                // 没有错误表明连接超时
                if (!ec) {
                  // 关闭套接字，async_connect将会返回 operation_aborted
                  derive.GetSocket().lowest_layer().close(s_ec_ignore);
                }
              });
        });

        derive.ClientPostResolve(std::move(this_ptr));

        return true;
      } else {  // 同步连接
        std::promise<std::error_code> promise;
        std::future<std::error_code> future = promise.get_future();

        // 开启一个超时定时器
        derive.Post([&derive, this_ptr,
                     promise = std::move(promise)]() mutable {
          NET_DEBUG("Connect client {}:{} sync post connect timeout timer",
                    derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port());
          derive.PostConnectTimeoutTimer(
              derive.GetConnectTimeoutDuration(), this_ptr,
              [&derive, this_ptr, promise = std::move(promise)](
                  const std::error_code &ec) mutable {
                IgnoreUnused(this_ptr);

                NET_DEBUG(
                    "Connect client [async {}] {}:{} connect timeout timer "
                    "callback error_code: {}",
                    IsAsync, derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port(), ec);

                // 没有错误表明连接超时
                if (!ec) {
                  // 关闭套接字，async_connect将会返回 operation_aborted
                  derive.GetSocket().lowest_layer().close(s_ec_ignore);
                }

                promise.set_value(derive.GetConnectErrorCode()
                                      ? derive.GetConnectErrorCode()
                                      : (ec ? ec : asio::error::timed_out));
              });
        });

        derive.ClientPostResolve(std::move(this_ptr));

        if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
          NET_DEBUG(
              "Connect client [async {}] {}:{} connect timeout timer "
              "callback error_code: {}",
              IsAsync, derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), future.get());
          SetLastError(future.get());
        } else {
          TPN_ASSERT(false, "Connect client PostResolve faile");
        }
        return derive.IsStarted();
      }

    } catch (std::system_error &e) {
      NET_DEBUG("Connect client [async {}] {}:{} exception error_code: {}",
                IsAsync, derive.GetLocalEndpoint().address().to_string(),
                derive.GetLocalEndpoint().port(), e.code());

      SetLastError(e);

      derive.HandleConnect(e.code(), std::move(this_ptr));
    }
    return false;
  }

  /// 连接模块提交解析地址
  /// 返回值特化为客户端
  ///  @tparam      IsSession   非网络会话
  ///  @param[in]   this_ptr    延长生命周期句柄
  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> ClientPostResolve(
      std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostResolve client {}:{} => {}:{}",
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), this->host_, this->port_);

    derive.Post([this, &derive, this_ptr = std::move(this_ptr)]() mutable {
      // 解析服务器地址
      std::unique_ptr<resolver_type> resolver_uptr =
          std::make_unique<resolver_type>(derive.GetIoHandle().GetIoContext());

      resolver_type *resolver_rptr = resolver_uptr.get();

      // 在 async_resolve 执行完成前，我们必须保持resolver对象
      // 所以在lambda表达式中捕获resolver_uptr
      // 开发中遇到一个Bug这里传了resolver_rptr结果回调回来的时候析构了
      // 注意这里要移动resolve_uptr
      resolver_rptr->async_resolve(
          this->host_, this->port_,
          asio::bind_executor(
              derive.GetIoHandle().GetStrand(),
              [this, &derive, this_ptr = std::move(this_ptr),
               resolver_uptr = std::move(resolver_uptr)](
                  const std::error_code &ec,
                  const endpoints_type &endpoints) mutable {
                NET_DEBUG(
                    "PostResolve client {}:{} => {}:{} async resolve error {}",
                    derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port(), this->host_, this->port_,
                    ec);

                SetLastError(ec);
                this->endpoints_ = endpoints;
                if (ec) {
                  // 有错误需要断开连接
                  derive.HandleConnect(ec, std::move(this_ptr));
                } else {
                  // 提交连接请求
                  derive.ClientPostConnect(ec, this->endpoints_.begin(),
                                           std::move(this_ptr));
                }
              }));
    });
  }

  /// 连接模块提交连接
  /// 返回值特化为客户端
  ///  @tparam      IsSession   非网络会话
  ///  @param[in]   ec          错误码
  ///  @param[in]   iter        端点迭代器
  ///  @param[in]   this_ptr    延长生命周期句柄
  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> ClientPostConnect(
      std::error_code ec, endpoints_iterator iter,
      std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("ClientPostConnect client {}:{} => {}:{} error {}",
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), this->host_, this->port_, ec);

    try {
      if (iter == this->endpoints_.end()) {
        NET_DEBUG("ClientPostConnect client {}:{} => {}:{} host unreachable",
                  derive.GetLocalEndpoint().address().to_string(),
                  derive.GetLocalEndpoint().port(), this->host_, this->port_);
        // 没有更多的端点可以尝试，关闭客户端
        derive.HandleConnect(ec ? ec : asio::error::host_unreachable,
                             std::move(this_ptr));
        return;
      }
      derive.GetSocket().lowest_layer().async_connect(
          iter->endpoint(),
          asio::bind_executor(
              derive.GetIoHandle().GetStrand(),
              MakeAllocator(
                  derive.GetReadAllocator(),
                  [&derive, iter, this_ptr](const std::error_code &ec) mutable {
                    NET_DEBUG(
                        "ClientPostConnect client {}:{} async connect error {}",
                        derive.GetLocalEndpoint().address().to_string(),
                        derive.GetLocalEndpoint().port(), ec);
                    SetLastError(ec);
                    if (ec && ec != asio::error::operation_aborted) {
                      // 连接错误 尝试下一个端点
                      derive.ClientPostConnect(ec, ++iter, std::move(this_ptr));
                    } else {
                      // 连接成功 处理连接成功
                      derive.HandleConnect(ec, std::move(this_ptr));
                    }
                  })));
    } catch (std::system_error &e) {
      NET_DEBUG("ClientPostConnect client {}:{} exception error {}",
                derive.GetLocalEndpoint().address().to_string(),
                derive.GetLocalEndpoint().port(), e.code());
      SetLastError(e);

      // 连接失败 处理连接
      derive.HandleConnect(e.code(), std::move(this_ptr));
    }
  }

  /// 连接模块处理连接
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void HandleConnect(const std::error_code ec,
                                std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("HandleConnect error {}", ec);

    SetLastError(ec);

    derive.DoneConnect(ec, std::move(this_ptr));
  }

  /// 连接模块连接完成处理
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期句柄
  TPN_INLINE void DoneConnect(std::error_code ec,
                              std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("DoneConnect error {}", ec);

    try {
      // 如果连接超时，则将错误重置为timed_out
      if (derive.IsConnectTimeout()) {
        ec = asio::error::timed_out;
      }

      // 无论连接成功还是失败或超时，都可以取消超时计时器。
      derive.StopConnectTimeoutTimer(ec);

      NetState expected;

      // 将状态设置为在FireConnect之前启动，因为用户可能在FireConnect中发送数据，
      // 并且如果未将状态设置为start则失败
      if (!ec) {
        expected = NetState::kNetStateStarting;
        if (!derive.GetNetState().compare_exchange_strong(
                expected, NetState::kNetStateStarted)) {
          NET_DEBUG("DoneConnect not starting state {}",
                    ToNetStateStr(derive.GetNetState()));
          ec = asio::error::operation_aborted;
        }
      }

      SetLastError(ec);

      if constexpr (ArgsType::is_session) {  // 网络层会话
        if (!ec) {
          expected = NetState::kNetStateStarted;
          if (derive.GetNetState().compare_exchange_strong(
                  expected, NetState::kNetStateStarted)) {
            NET_DEBUG("DoneConnect started state {}",
                      ToNetStateStr(derive.GetNetState()));
            // 连接成功时才通知
            derive.FireConnect(this_ptr);
          }
        }
      } else {  // 网络层客户端
        expected = NetState::kNetStateStarting;
        if (derive.GetNetState().compare_exchange_strong(
                expected, NetState::kNetStateStarting)) {
          TPN_ASSERT(ec, "DoneConnect error {}", ec);
          NET_DEBUG("DoneConnect starting state {}",
                    ToNetStateStr(derive.GetNetState()));
          derive.FireConnect(this_ptr, ec);
        } else {
          expected = NetState::kNetStateStarted;
          if (derive.GetNetState().compare_exchange_strong(
                  expected, NetState::kNetStateStarted)) {
            TPN_ASSERT(!ec, "DoneConnect error {}", ec);
            NET_DEBUG("DoneConnect started state {}",
                      ToNetStateStr(derive.GetNetState()));
            derive.FireConnect(this_ptr, ec);
          }
        }
      }

      if (!ec) {
        expected = NetState::kNetStateStarted;
        if (!derive.GetNetState().compare_exchange_strong(
                expected, NetState::kNetStateStarted)) {
          NET_DEBUG("DoneConnect should be started state {}",
                    ToNetStateStr(derive.GetNetState()));
          asio::detail::throw_error(asio::error::operation_aborted);
        }
      }
      asio::detail::throw_error(ec);

      // 通知子类开始
      derive.DoStart(std::move(this_ptr));
    } catch (std::system_error &e) {
      NET_ERROR("DoneConnect error {}", e.code());

      SetLastError(e);
      // 异常后断开连接
      derive.DoDisconnect(e.code());
    }
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CONNECT_H_
