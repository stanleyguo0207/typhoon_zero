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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SCOKET_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SCOKET_WRAP_H_

#include <string>

#include "net_common.h"

namespace tpn {

namespace net {

/// 套接字封装
///  @tparam  Derived     CRTP类型
///  @tparam  ArgsType    套接字模板参数 需要提供 SocketType类型定义
///
/// @sa asio::ip::tcp::socket
/// @sa asio::ip::udp::socket
template <typename Derived, typename ArgsType>
class Socket {
 public:
  /// 套接字类型
  using SocketType = TPN_RMRFCV(typename ArgsType::SocketType);

  /// 构造函数
  ///  @param[in]   args    @sa asio::ip::tcp::socket @sa asio::ip::udp::socket
  template <typename... Args>
  explicit Socket(Args &&...args) : socket_(std::forward<Args>(args)...) {}

  ~Socket() = default;

  /// 获取socket对象引用
  ///  @return Socket<Derived,Stream>::SocketType
  inline SocketType &GetSocket() { return this->socket_; }

  /// 获取stream对象引用
  ///  @return Socket<Derived,Stream>::SocketType
  inline SocketType &GetStream() { return this->socket_; }

  /// 获取本地地址
  ///  @throw   异常被SetLastError截获 @ref SetLastError
  ///  @return 本地地址
  inline std::string GetLocalAddress() {
    try {
      return this->socket_.lowest_layer()
          .local_endpoint()
          .address()
          .to_string();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return std::string();
  }

  /// 获取本地端口
  ///  @throw   异常被SetLastError截获 @ref SetLastError
  ///  @return 本地端口
  inline unsigned short GetLocalPort() {
    try {
      return this->socket_.lowest_layer().local_endpoint().port();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return static_cast<unsigned short>(0);
  }

  /// 获取远端地址
  ///  @throw   异常被SetLastError截获 @ref SetLastError
  ///  @return 远端地址
  inline std::string GetRemoteAddress() {
    try {
      return this->socket_.lowest_layer()
          .remote_endpoint()
          .address()
          .to_string();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return std::string();
  }

  /// 获取远端端口
  ///  @throw   异常被SetLastError截获 @ref SetLastError
  ///  @return 远端端口
  inline unsigned short GetRemotePort() {
    try {
      return this->socket_.lowest_layer().remote_endpoint().port();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return static_cast<unsigned short>(0);
  }

  /// 设置套接字的发送缓冲区大小
  /// Implements the SOL_SOCKET/SO_SNDBUF socket option.
  ///  @param[in]   val     要设置的缓冲区大小
  ///  @throw   异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  inline Derived &SetSendBufferSize(int val) {
    try {
      this->socket_.lowest_layer().set_option(
          asio::socket_base::send_buffer_size(val));
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return (CRTP_CAST(this));
  }

  /// 获取套接字的发送缓冲区大小
  /// Implements the SOL_SOCKET/SO_SNDBUF socket option.
  ///  @throw   异常被 SetLastError 截获 @ref SetLastError
  ///  @return 发送缓冲区大小
  inline int GetSendBufferSize() const {
    try {
      asio::socket_base::send_buffer_size option;
      this->socket_.lowest_layer().get_option(option);
      return option.value();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return -1;
  }

  /// 设置套接字的接受缓冲区大小
  /// Implements the SOL_SOCKET/SO_RCVBUF socket option.
  ///  @param[in]   val     要设置的缓存区大小
  ///  @throw   异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  inline Derived &SetReceiveBufferSize(int val) {
    try {
      this->socket_.lowest_layer().set_option(
          asio::socket_base::receive_buffer_size(val));
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return (CRTP_CAST(this));
  }

  /// 获取套接字的接受缓冲区大小
  /// Implements the SOL_SOCKET/SO_RCVBUF socket option.
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return 接受缓冲区大小
  inline int GetReceiveBufferSize() const {
    try {
      asio::socket_base::receive_buffer_size option;
      this->socket_.lowest_layer().get_option(option);
      return option.value();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return -1;
  }

  /// 设置套接字是否保持连接
  /// Implements the SOL_SOCKET/SO_KEEPALIVE socket option.
  ///  @param[in]  val     是否保持连接标志
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  inline Derived &SetKeepAlive(int val) {
    try {
      this->socket_.lowest_layer().set_option(
          asio::socket_base::keep_alive(val));
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return (CRTP_CAST(this));
  }

  /// 获取套接字是否保持连接
  /// Implements the SOL_SOCKET/SO_KEEPALIVE socket option.
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return 是否保持连接
  inline bool IsKeepAlive() const {
    try {
      asio::socket_base::keep_alive option;
      this->socket_.lowest_layer().get_option(option);
      return option.value();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return false;
  }

  /// 设置套接字是否端口复用
  /// Implements the SOL_SOCKET/SO_REUSEADDR socket option.
  ///  @param[in]  val     是否复用标志
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  inline Derived &ReuseAddress(bool val) {
    try {
      this->socket_.lowest_layer().set_option(
          asio::socket_base::reuse_address(val));
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return (CRTP_CAST(this));
  }

  /// 获取套接字是否端口复用
  /// Implements the SOL_SOCKET/SO_REUSEADDR socket option.
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  inline bool IsReuseAddress() const {
    try {
      asio::socket_base::reuse_address option;
      this->socket_.lowest_layer().get_option(option);
      return option.value();
    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return false;
  }

  /// 设置套接字是否禁用Nagle算法
  /// Implements the IPPROTO_TCP/TCP_NODELAY socket option.
  ///  @param[in]  val     是否禁用标志
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  /// @note
  ///    只对tcp有效 非tcp不做任何事情
  inline Derived &SetNoDelay(bool val) {
    try {
      if constexpr (std::is_same_v<typename SocketType::protocol_type,
                                   asio::ip::tcp>) {
        this->socket_.lowest_layer().set_option(asio::ip::tcp::no_delay(val));
      } else {
        std::ignore = true;
      }

    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return (CRTP_CAST(this));
  }

  /// 获取套接字是否禁用Nagle算法
  /// Implements the IPPROTO_TCP/TCP_NODELAY socket option.
  ///  @throw  异常被 SetLastError 截获 @ref SetLastError
  ///  @return CRTP对象自身引用 用于链式调用
  /// @note
  ///    只对tcp有效 非tcp不做任何事情
  inline bool IsNodelay() const {
    try {
      if constexpr (std::is_same_v<typename SocketType::protocol_type,
                                   asio::ip::tcp>) {
        asio::ip::tcp::no_delay option;
        this->socket_.lowest_layer().get_option(option);
        return option.value();
      } else {
        std::ignore = true;
      }

    } catch (std::system_error &e) {
      SetLastError(e);
    }
    return false;
  }

 protected:
  typename ArgsType::SocketType socket_;  ///< socket
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_WRAPPER_SCOKET_WRAP_H_
