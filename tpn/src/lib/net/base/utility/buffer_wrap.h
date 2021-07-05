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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_BUFFER_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_BUFFER_WRAP_H_

#include <limits>
#include <memory>

#include "common.h"
#include "traits_hub.h"
#include "asio_wrap.h"

namespace tpn {

namespace net {

/// 空缓冲区
/// 接口参考 @see asio::basic_streambuf
struct EmptyBuffer {
  using size_type = size_t;

  /// 获取输入序列的大小 @see asio::basic_streambuf
  ///  @return 输入序列的大小
  TPN_INLINE size_type size() const { return 0; }

  /// 获取缓冲区最大长度 @see asio::basic_streambuf
  ///  @return 缓冲区最大长度
  TPN_INLINE size_type max_size() const { return 0; }

  /// 获取缓冲区的内存实际申请最大长度 @see asio::basic_streambuf
  ///  @return 缓冲区的内存实际申请最大长度
  TPN_INLINE size_type capacity() const { return 0; }

  /// 获取表示输入序列的缓冲区列表 @see asio::basic_streambuf
  ///  @return 表示输入序列的缓冲区列表
  TPN_INLINE auto data() const { return asio::buffer(asio::const_buffer()); }

  /// 获取表示输出序列的缓冲区列表具有给定的大小 @see asio::basic_streambuf
  ///  @param[in]   n     缓冲大小
  ///  @return 表示输出序列的缓冲区列表具有给定的大小
  TPN_INLINE auto prepare(size_type /* n */) {
    return asio::buffer(asio::mutable_buffer());
  }

  /// 将数据从输出序列移动到输入序列 @see asio::basic_streambuf
  ///  @param[in]   n     数据大小
  TPN_INLINE void commit(size_type /* n */) {}

  /// 从输入序列中删除数据
  ///  @param[in]   n     数据大小
  TPN_INLINE void consume(size_type /* n */) {}
};

/// 缓冲区包裹
/// CRTP
template <typename BufferType,
          bool has_construct_with_size =
              is_buffer_has_construct_with_size_v<BufferType>>
class BufferWrap;

/// 缓冲区包裹 有size_t构造函数缓冲区类特化
/// CRTP
///  @tparam  BufferType    缓冲区类型,需要满足指定接口,@see EmptyBuffer
template <typename BufferType>
class BufferWrap<BufferType, true> : public BufferType {
 public:
  using size_type   = size_t;
  using buffer_type = BufferType;
  using BufferType::BufferType;

  BufferWrap()  = default;
  ~BufferWrap() = default;

  /// 构造函数
  ///  @param[in]   max       缓冲区最大长度
  BufferWrap(size_type max) : BufferType(max) {}

  /// 构造函数
  ///  @param[in]   max       缓冲区最大长度
  ///  @param[in]   prepare   缓冲区准备长度
  BufferWrap(size_type max, size_type prepare)
      : BufferType(max), prepare_{prepare} {
    BufferType::prepare(this->prepare_);
  }

  /// 获取基类
  ///  @return 获取CRTP基类
  TPN_INLINE BufferType &GetBase() { return (*this); }

  /// 获取基类
  ///  @return 获取CRTP基类
  TPN_INLINE const BufferType &GetBase() const { return (*this); }

  /// 设置缓冲区准备长度
  ///  @param[in]   prepare   缓冲区准备长度
  ///  @return buffer包裹类
  TPN_INLINE BufferWrap &SetPrepareSize(size_type prepare) {
    this->prepare_ = prepare;
    return (*this);
  }

  /// 获取缓冲区准备长度
  ///  @return 缓冲区准备长度
  TPN_INLINE size_type GetPrepareSize() const { return this->prepare_; }

  /// 设置缓冲区准备长度
  /// 本接口无效
  ///  @param[in]   max       缓冲区最大长度
  ///  @return buffer包裹类
  TPN_INLINE BufferWrap &SetMaxSize(size_type /* max */) { return (*this); }

  /// 获取缓冲区最大长度
  ///  @return 缓冲区最大长度
  TPN_INLINE size_type GetMaxSize() const {
    if constexpr (is_buffer_has_max_size_v<BufferType>) {
      return BufferType::max_size();
    } else {
      return (std::numeric_limits<size_type>::max)();
    }
  }

 private:
  size_type prepare_{kBufferDefaultSize};  ///< 缓冲区准备长度

  TPN_DEFAULT_COPY(BufferWrap)
  TPN_DEFAULT_MOVE(BufferWrap)
};

/// 缓冲区包裹 无size_t构造函数缓冲区类特化
/// CRTP
///  @tparam  BufferType    缓冲区类型,需要满足指定接口,@see EmptyBuffer
template <typename BufferType>
class BufferWrap<BufferType, false> : public BufferType {
 public:
  using size_type   = size_t;
  using buffer_type = BufferType;
  using BufferType::BufferType;

  BufferWrap()  = default;
  ~BufferWrap() = default;

  /// 构造函数
  ///  @param[in]   max       缓冲区最大长度
  BufferWrap(size_type max) : BufferType(), max_{max} {}

  /// 构造函数
  ///  @param[in]   max       缓冲区最大长度
  ///  @param[in]   prepare   缓冲区准备长度
  BufferWrap(size_type max, size_type prepare)
      : BufferType(), max_{max}, prepare_{prepare} {
    BufferType::prepare(this->prepare_);
  }

  /// 获取基类
  ///  @return 获取CRTP基类
  TPN_INLINE BufferType &GetBase() { return (*this); }

  /// 获取基类
  ///  @return 获取CRTP基类
  TPN_INLINE const BufferType &GetBase() const { return (*this); }

  /// 设置缓冲区准备长度
  ///  @param[in]   prepare   缓冲区准备长度
  ///  @return buffer包裹类
  TPN_INLINE BufferWrap &SetPrepareSize(size_type prepare) {
    this->prepare_ = prepare;
    return (*this);
  }

  /// 获取缓冲区准备长度
  ///  @return 缓冲区准备长度
  TPN_INLINE size_type GetPrepareSize() const { return this->prepare_; }

  /// 设置缓冲区准备长度
  ///  @param[in]   max       缓冲区最大长度
  ///  @return buffer包裹类
  TPN_INLINE BufferWrap &SetMaxSize(size_type max) {
    this->max_ = max;
    return (*this);
  }

  /// 获取缓冲区最大长度
  ///  @return 缓冲区最大长度
  TPN_INLINE size_type GetMaxSize() const {
    if constexpr (is_buffer_has_max_size_v<BufferType>) {
      return BufferType::max_size();
    } else {
      return this->max_;
    }
  }

 private:
  size_type max_{(std::numeric_limits<size_type>::max)()};  ///< 缓冲区最大长度
  size_type prepare_{kBufferDefaultSize};  ///< 缓冲区准备长度

  TPN_DEFAULT_COPY(BufferWrap)
  TPN_DEFAULT_MOVE(BufferWrap)
};

/// 缓冲区包裹 空buffer特化
/// CRTP
///  @tparam  BufferType    缓冲区类型,需要满足指定接口,@see EmptyBuffer
template <>
class BufferWrap<EmptyBuffer, true> : public EmptyBuffer {
 public:
  using size_type   = size_t;
  using buffer_type = EmptyBuffer;

  BufferWrap()  = default;
  ~BufferWrap() = default;

  /// 构造函数
  BufferWrap(size_type /* max */) {}

  /// 构造函数
  BufferWrap(size_type /* max */, size_type /* prepare */) {}

  /// 获取基类
  TPN_INLINE EmptyBuffer &GetBase() { return (*this); }

  /// 获取基类
  TPN_INLINE const EmptyBuffer &GetBase() const { return (*this); }

  /// 设置缓冲区准备长度
  TPN_INLINE BufferWrap &SetPrepareSize(size_type /* prepare */) {
    return (*this);
  }

  /// 获取缓冲区准备长度
  TPN_INLINE size_type GetPrepareSize() const { return 0; }

  /// 设置缓冲区准备长度
  TPN_INLINE BufferWrap &SetMaxSize(size_type /* max */) { return (*this); }

  /// 获取缓冲区最大长度
  TPN_INLINE size_type GetMaxSize() const { return 0; }

  TPN_DEFAULT_COPY(BufferWrap)
  TPN_DEFAULT_MOVE(BufferWrap)
};

/// 缓冲区包裹 空buffer特化
/// CRTP
///  @tparam  BufferType    缓冲区类型,需要满足指定接口,@see EmptyBuffer
template <>
class BufferWrap<EmptyBuffer, false> : public EmptyBuffer {
 public:
  using size_type   = size_t;
  using buffer_type = EmptyBuffer;

  BufferWrap()  = default;
  ~BufferWrap() = default;

  /// 构造函数
  BufferWrap(size_type /* max */) {}

  /// 构造函数
  BufferWrap(size_type /* max */, size_type /* prepare */) {}

  /// 获取基类
  TPN_INLINE EmptyBuffer &GetBase() { return (*this); }

  /// 获取基类
  TPN_INLINE const EmptyBuffer &GetBase() const { return (*this); }

  /// 设置缓冲区准备长度
  TPN_INLINE BufferWrap &SetPrepareSize(size_type /* prepare */) {
    return (*this);
  }

  /// 获取缓冲区准备长度
  TPN_INLINE size_type GetPrepareSize() const { return 0; }

  /// 设置缓冲区准备长度
  TPN_INLINE BufferWrap &SetMaxSize(size_type /* max */) { return (*this); }

  /// 获取缓冲区最大长度
  TPN_INLINE size_type GetMaxSize() const { return 0; }

  TPN_DEFAULT_COPY(BufferWrap)
  TPN_DEFAULT_MOVE(BufferWrap)
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_BUFFER_WRAP_H_
