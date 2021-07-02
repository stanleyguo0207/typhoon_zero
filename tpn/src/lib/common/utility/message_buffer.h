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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_MESSAGE_BUFFER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_MESSAGE_BUFFER_H_

#include <vector>

#include "define.h"

namespace tpn {

/// 消息缓冲
class MessageBuffer {
  using storage_type = std::vector<uint8_t>;
  using size_type    = storage_type::size_type;
  using pointer      = storage_type::pointer;

 public:
  /// 构造函数
  /// 默认大小为4096
  MessageBuffer();

  /// 构造函数
  ///  @param[in]   initial_size    缓冲区初始化大小
  explicit MessageBuffer(size_type initial_size);

  /// 拷贝构造函数
  MessageBuffer(const MessageBuffer &other);

  /// 移动构造函数
  MessageBuffer(MessageBuffer &&other);

  /// 拷贝赋值函数
  MessageBuffer &operator=(const MessageBuffer &other);

  /// 移动赋值函数
  MessageBuffer &operator=(MessageBuffer &&other);

  /// 重置读写
  /// 只重置读写位置 内存不会变化
  void Rset();

  /// 重置缓冲区
  ///  @param[in]   bytes     重置后的大小
  void Resize(size_type bytes);

  /// 获取缓冲区地址
  ///  @return 缓冲区地址
  pointer GetBasePointer();

  /// 获取读位置地址
  ///  @return 读位置地址
  pointer GetReadPointer();

  /// 获取写位置地址
  ///  @return 写位置地址
  pointer GetWritePointer();

  /// 读完成
  /// 此接口在读取消息缓冲区后调用
  ///  @param[in]   bytes     读取了多少字节
  void ReadCompleted(size_type bytes);

  /// 写完成
  /// 此接口一般不用自己调用,除非你知道怎么用
  ///  @param[in]   bytes     写入了多少字节
  void WriteCompleted(size_type bytes);

  /// 获取缓冲区可用字节大小
  ///  @return 缓冲区可用字节大小
  size_type GetActiveSize() const;

  /// 获取缓冲区剩余字节大小
  ///  @return 缓冲区剩余字节大小
  size_type GetRemainingSpace() const;

  /// 获取缓冲区总字节大小
  ///  @return 缓冲区总字节大小
  size_type GetBufferSize() const;

  /// 缓冲区初始化
  /// 重新使用时需要调用,缓冲区会清理掉已读数据
  void Normalize();

  /// 确保剩余空间
  /// 调用之前 需要调用 @see Normalize
  void EnsureFreeSpace();

  /// 写入数据
  ///  @param[in]   data    数据地址
  ///  @param[in]   size    数据大小
  void Write(const void *data, size_type size);

  /// 移动缓冲区数据
  ///  @return 移动后的缓冲区数据
  storage_type &&Move();

 private:
  size_type wpos_{0};     ///< 写位置
  size_type rpos_{0};     ///< 读位置
  storage_type storage_;  ///< 缓冲区
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_MESSAGE_BUFFER_H_
