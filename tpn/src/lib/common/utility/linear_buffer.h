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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_LINEAR_BUFFER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_LINEAR_BUFFER_H_

#include <limits>
#include <memory>
#include <vector>

#include "common.h"
#include "asio_wrap.h"

namespace tpn {

/// 线性缓冲区基类
/// 可用于BufferWrap的缓冲区都将实现规定的接口
/// @see BufferWrap
template <typename Container>
class BasicLinearBuffer : protected Container {
 public:
  using size_type            = typename Container::size_type;
  using allocator_type       = typename Container::allocator_type;
  using const_buffers_type   = asio::const_buffer;
  using mutable_buffers_type = asio::mutable_buffer;

  BasicLinearBuffer()  = default;
  ~BasicLinearBuffer() = default;

  /// 构造函数
  ///  @param[in]   max     缓冲区的最大长度
  explicit BasicLinearBuffer(size_t max) : Container(), max_(max) {}

  /// 获取输入序列的大小 @see asio::basic_streambuf
  ///  @return 输入序列的大小
  TPN_INLINE size_type size() const { return wpos_ - rpos_; }

  /// 获取缓冲区最大长度 @see asio::basic_streambuf
  ///  @return 缓冲区最大长度
  TPN_INLINE size_type max_size() const { return max_; }

  /// 获取缓冲区的内存实际申请最大长度 @see asio::basic_streambuf
  ///  @return 缓冲区的内存实际申请最大长度
  TPN_INLINE size_type capacity() const { return Container::capacity(); }

  /// 获取表示输入序列的缓冲区列表 @see asio::basic_streambuf
  ///  @return 表示输入序列的缓冲区列表
  TPN_INLINE const_buffers_type data() const {
    return {Container::data() + rpos_, wpos_ - rpos_};
  }

  /// 获取表示输出序列的缓冲区列表具有给定的大小 @see asio::basic_streambuf
  ///  @param[in]   n     缓冲大小
  ///  @return 表示输出序列的缓冲区列表具有给定的大小
  TPN_INLINE mutable_buffers_type prepare(size_type n) {
    const size_type cap = Container::capacity();
    if (n <= cap - wpos_) {
      Container::resize(wpos_ + n);
      return {Container::data() + wpos_, n};
    }

    const size_type size = this->size();
    if (n <= cap - size) {
      if (size > 0) {
        std::memmove(Container::data(), Container::data() + rpos_, size);
      }
      rpos_ = 0;
      wpos_ = size;
      Container::resize(wpos_ + n);
      return {Container::data() + wpos_, n};
    }

    if (n > max_ - size) {
      asio::detail::throw_exception(
          std::length_error{" BasicLinearBuffer overflow"});
    }
    const size_type new_size =
        (std::max<
            size_type>)((std::min<size_type>)(max_,
                                              (std::max<size_type>)(2 * cap,
                                                                    wpos_ + n)),
                        kBufferDefaultSize);
    Container::resize(new_size);
    Container::resize(wpos_ + n);
    return {Container::data() + rpos_, n};
  }

  /// 将数据从输出序列移动到输入序列 @see asio::basic_streambuf
  ///  @param[in]   n     数据大小
  TPN_INLINE void commit(size_type n) {
    wpos_ += (std::min<size_type>)(n, Container::size() - wpos_);
  }

  /// 从输入序列中删除数据
  ///  @param[in]   n     数据大小
  TPN_INLINE void consume(size_type n) {
    if (n > wpos_ - rpos_) {
      wpos_ = 0;
      rpos_ = 0;
      Container::resize(0);
      return;
    }

    rpos_ += n;
  }

 private:
  size_type rpos_{0};                                       ///< 读位置
  size_type wpos_{0};                                       ///< 写位置
  size_type max_{(std::numeric_limits<size_type>::max)()};  ///< 缓冲区最大长度
};

/// 线性缓冲区
using LinearBuffer = BasicLinearBuffer<std::vector<uint8_t>>;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_LINEAR_BUFFER_H_
