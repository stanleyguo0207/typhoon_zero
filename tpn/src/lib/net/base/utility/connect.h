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

template <typename Derived, typename ArgsType, bool IsSession>
class ConnectBase;

template <typename Derived, typename ArgsType>
class ConnectBase<Derived, ArgsType, true> {};

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
  std::string host_;          ///<  地址
  std::string port_;          ///<  端口
  endpoints_type endpoints_;  ///<  端点
};

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
  template <bool IsAsync, typename String, typename StrOrInt,
            bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, bool> StartConnect(
      String &&host, StrOrInt &&port, std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("Connect client async {} {}:{} => {}:{}", IsAsync,
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), host, port);

    try {
      if (derive.IsIoPoolStopped()) {
        NET_ERROR("Connect client io pool stopped");
        SetLastError(asio::error::shut_down);
        return false;
      }

      this->host_ = ToString(std::forward<String>(host));
      this->port_ = ToString(std::forward<StrOrInt>(port));

      auto &socket = derive.GetSocket().lowest_layer();

      socket.close(s_ec_ignore);

      // @sa LocalEndpoint
      socket.open(derive.GetLocalEndpoint().protocol());

      // 设置端口复用
      socket.set_option(typename raw_socket_type::reuse_address(true));

      // tcp keeplive处理
      if constexpr (std::is_same_v<typename raw_socket_type::protocol_type,
                                   asio::ip::tcp>) {
        NET_DEBUG("Connect client tcp");
        // @sa TcpKeepAlive
        derive.SetKeepAliveOptions();
      } else {
        NET_DEBUG("Connect client not tcp");
        std::ignore = true;
      }

      // 监听器通知启动事件
      // derive.FireInit();

      // 端口绑定
      socket.bind(derive.GetLocalEndpoint());

      if constexpr (IsAsync) {  // 如果异步连接
        // 开启超时定时器
        derive.Post([&derive, this_ptr]() mutable {
          NET_DEBUG("Connect client {}:{} async post connect timeout timer",
                    derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port());
          derive.PostConnectTimeoutTimer(
              derive.GetConnectTimeout(), this_ptr,
              [&derive, this_ptr](const std::error_code &ec) mutable {
                IgnoreUnused(this_ptr);

                NET_DEBUG("Connect client {}:{} async connect timeout error {}",
                          derive.GetLocalEndpoint().address().to_string(),
                          derive.GetLocalEndpoint().port(), ec);

                // 没有错误表明连接超时
                if (!ec) {
                  // 关闭套接字，async_connect将会返回 operation_aborted
                  derive.GetSocket().lowest_layer().close(s_ec_ignore);
                }
              });
        });

        derive.PostResolve(std::move(this_ptr));

        return true;
      } else {  // 如果同步连接
        std::promise<std::error_code> promise;
        std::future<std::error_code> future = promise.get_future();

        // 开启一个超时定时器
        derive.Post([&derive, this_ptr,
                     promise = std::move(promise)]() mutable {
          NET_DEBUG("Connect client {}:{} sync post connect timeout timer",
                    derive.GetLocalEndpoint().address().to_string(),
                    derive.GetLocalEndpoint().port());
          derive.PostConnectTimeoutTimer(
              derive.GetConnectTimeout(), this_ptr,
              [&derive, this_ptr, promise = std::move(promise)](
                  const std::error_code &ec) mutable {
                IgnoreUnused(this_ptr);

                NET_DEBUG("Connect client {}:{} sync connect timeout error {}",
                          derive.GetLocalEndpoint().address().to_string(),
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

        derive.PostResolve(std::move(this_ptr));

        if (!derive.GetIoHandle().GetStrand().running_in_this_thread()) {
          NET_DEBUG(
              "Connect client {}:{} sync not runing in this thread connect "
              "error {}",
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), derive.GetConnectErrorCode());
          SetLastError(future.get());
        } else {
          TPN_ASSERT(false, "Connect client PostResolve faile");
        }
        return derive.IsStarted();
      }

    } catch (std::system_error &e) {
      NET_ERROR("Connect client {}:{} async {} error {}",
                derive.GetLocalEndpoint().address().to_string(),
                derive.GetLocalEndpoint().port(), IsAsync, e.code());

      SetLastError(e);

      derive.HandleConnect(e.code(), std::move(this_ptr));
    }
    return false;
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> PostResolve(
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

      // 在 async_resolve 执行完成前，我们必须保持 resolver 对象
      // 所以在lambda表达式中捕获 resolver_rptr
      resolver_rptr->async_resolve(
          this->host_, this->port_,
          asio::bind_executor(
              derive.GetIoHandle().GetStrand(),
              [this, &derive, this_ptr = std::move(this_ptr),
               resolver_uptr = std::move(resolver_uptr)](
                  const std::error_code &ec,
                  const endpoints_type &endpoints) mutable {
                NET_DEBUG("PostResolve client {}:{} => {}:{} error {}",
                          derive.GetLocalEndpoint().address().to_string(),
                          derive.GetLocalEndpoint().port(), this->host_,
                          this->port_, ec);

                SetLastError(ec);
                this->endpoints_ = endpoints;
                if (ec) {
                  NET_WARN(
                      "PostResolve client {}:{} => {}:{} error {} "
                      "HandleConnect",
                      derive.GetLocalEndpoint().address().to_string(),
                      derive.GetLocalEndpoint().port(), this->host_,
                      this->port_, ec);
                  derive.HandleConnect(ec, std::move(this_ptr));
                } else {
                  NET_DEBUG(
                      "PostResolve client {}:{} => {}:{} error {} PostConnect",
                      derive.GetLocalEndpoint().address().to_string(),
                      derive.GetLocalEndpoint().port(), this->host_,
                      this->port_, ec);
                  derive.PostConnect(ec, this->endpoints_.begin(),
                                     std::move(this_ptr));
                }
              }));
    });
  }

  template <bool IsSession = ArgsType::is_session>
  TPN_INLINE std::enable_if_t<!IsSession, void> PostConnect(
      std::error_code ec, endpoints_iterator iter,
      std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("PostConnect client {}:{} => {}:{} error {}",
              derive.GetLocalEndpoint().address().to_string(),
              derive.GetLocalEndpoint().port(), this->host_, this->port_, ec);

    try {
      if (iter == this->endpoints_.end()) {
        NET_DEBUG("PostConnect client {}:{} => {}:{} host unreachable",
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
                    SetLastError(ec);
                    if (ec && ec != asio::error::operation_aborted) {
                      NET_WARN("PostConnect client {}:{} error {}",
                               derive.GetLocalEndpoint().address().to_string(),
                               derive.GetLocalEndpoint().port(), ec);
                      // 连接错误 尝试下一个端点
                      derive.PostConnect(ec, ++iter, std::move(this_ptr));
                    } else {
                      NET_DEBUG("PostConnect client {}:{} error {}",
                                derive.GetLocalEndpoint().address().to_string(),
                                derive.GetLocalEndpoint().port(), ec);
                      // 连接成功 处理连接成功
                      derive.HandleConnect(ec, std::move(this_ptr));
                    }
                  })));

    } catch (std::system_error &e) {
      NET_DEBUG("PostConnect client {}:{} error {}",
                derive.GetLocalEndpoint().address().to_string(),
                derive.GetLocalEndpoint().port(), e.code());
      SetLastError(e);

      // 连接失败 处理连接
      derive.HandleConnect(e.code(), std::move(this_ptr));
    }
  }

  TPN_INLINE void HandleConnect(const std::error_code ec,
                                std::shared_ptr<Derived> this_ptr) {
    Derived &derive = CRTP_CAST(this);

    NET_DEBUG("HandleConnect error {}", ec);

    SetLastError(ec);

    derive.DoneConnect(ec, std::move(this_ptr));
  }

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

      if constexpr (ArgsType::is_session) {
        // 如果是会话，仅在连接成功时调用FireConnect通知
        if (!ec) {
          expected = NetState::kNetStateStarted;
          if (derive.GetNetState().compare_exchange_strong(
                  expected, NetState::kNetStateStarted)) {
            NET_DEBUG("DoneConnect started and FireConnect state {}",
                      ToNetStateStr(derive.GetNetState()));
            // derive.FireConnect(this_ptr);
          }
        }
      } else {
        // 如果是客户端，连接成功还是失败，始终调用FireConnect通知
        expected = NetState::kNetStateStarting;
        if (derive.GetNetState().compare_exchange_strong(
                expected, NetState::kNetStateStarting)) {
          TPN_ASSERT(ec, "DoneConnect error {}", ec);
          NET_DEBUG("DoneConnect starting and FireConnect state {}",
                    ToNetStateStr(derive.GetNetState()));
          // derive.FireConnect(this_ptr, ec);
        } else {
          expected = NetState::kNetStateStarted;
          if (derive.GetNetState().compare_exchange_strong(
                  expected, NetState::kNetStateStarted)) {
            TPN_ASSERT(!ec, "DoneConnect error {}", ec);
            NET_DEBUG("DoneConnect started and FireConnect state {}",
                      ToNetStateStr(derive.GetNetState()));
            // derive.FireConnect(this_ptr, ec);
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

      derive.DoStart(std::move(this_ptr));

    } catch (std::system_error &e) {
      NET_ERROR("DoneConnect error {}", e.code());

      SetLastError(e);
      derive.DoDisconnect(e.code());
    }
  }
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_CONNECT_H_
