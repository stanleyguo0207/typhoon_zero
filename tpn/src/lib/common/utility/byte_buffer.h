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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_BUFFER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_BUFFER_H_

#include <cstring>

#include <array>
#include <vector>
#include <string>

#include "common.h"

namespace tpn {

class MessageBuffer;

/// 字节流
class TPN_COMMON_API ByteBuffer {
 public:
  using storage_type = std::vector<uint8_t>;
  using size_type    = storage_type::size_type;

  static constexpr size_t kByteBufferDefaultSize = 0x1000;  ///< 默认字节流大小
  static constexpr uint8_t kBoundaryBitPos       = 7;  ///< 边界字节位置
  static constexpr uint8_t kInitialBitPos = 8;  ///< 字节位初始位置

  /// 构造函数预留标志
  struct ReserveFlag {};
  /// 构造函数重置标志
  struct ResizeFlag {};

  /// 字节流
  ByteBuffer();

  /// 字节流
  ///  @param[in]   size          预留大小
  ///  @param[in]   ReserveFlag   预留标志
  ByteBuffer(size_t size, ReserveFlag);

  /// 字节流
  ///  @param[in]   size          重置大小
  ///  @param[in]   ReserveFlag   重置标志
  ByteBuffer(size_t size, ResizeFlag);

  /// 消息缓冲构造字节流
  ///  @param[in]   buffer        消息缓冲
  ByteBuffer(MessageBuffer &&buffer);

  /// 移动字节流
  ///  @return 字节流缓冲区
  storage_type &&Move() noexcept;

  /// 拷贝构造函数
  ByteBuffer(const ByteBuffer &other) = default;

  /// 移动构造函数
  ByteBuffer(ByteBuffer &&other) noexcept;

  /// 拷贝赋值函数
  ByteBuffer &operator=(const ByteBuffer &other);

  /// 移动赋值函数
  ByteBuffer &operator=(ByteBuffer &&other) noexcept;

  /// 析构函数
  virtual ~ByteBuffer() = default;

  /// 获取读位置
  ///  @return 当前读位置
  size_t GetReadPos() const;

  /// 设置读位置
  ///  @param[in]   rpos    要设置的读位置
  ///  @return 当前读位置
  size_t SetReadPos(size_t rpos);

  /// 获取写位置
  ///  @return 当前写位置
  size_t GetWritePos() const;

  /// 设置写位置
  ///  @param[in]   wpos    要设置的写位置
  ///  @return 当前写位置
  size_t SetWritePos(size_t wpos);

  /// 获取最后一次写的bit位置
  ///  @return 最后一次写的bit位置
  size_t GetLastWriteBitPos() const;

  /// 获取最后一次写的bit位
  ///  @param[in]   new_pos     新的写bit位置
  ///  @return 最后一次写的bit位置
  size_t SetLastWriteBitPos(size_t new_pos);

  /// 获取字节流内容
  /// 返回的是底层容器地址
  ///  @return 字节流内容
  uint8_t *GetContents();

  /// 获取字节流内容
  /// 返回的是底层容器地址
  ///  @return 字节流内容
  const uint8_t *GetContents() const;

  /// 获取字节流大小
  ///  @return 字节流大小
  size_t GetSize() const;

  /// 判断字节流是否为空
  ///  @return 字节流为空为true
  bool IsEmpty() const;

  /// 字节流重置大小
  /// 重置后字节流将被清空并且写位置将置位字节流长度
  ///  @param[in]   new_size    新的大小
  void Resize(size_t new_size);

  /// 字节流预留大小
  ///  @param[in]   res_size    预留大小，如果预留大小小于原有长度则无效
  void Reserve(size_t res_size);

  /// 清空字节流
  void Clear();

  /// 判断是否有未刷新的bit包
  bool HasUnfinishedBitPack() const;

  /// 刷新bit流
  void FlushBits();

  /// 读结束
  /// 读位置更新到写位置 内存没有移除
  void ReadFinish();

  /// 重置bit位置
  void ResetBitPos();

  /// 写入bit值
  ///  @param[in]   bit     bit值
  ///  @return 写入的bit值
  bool WriteBit(bool bit);

  /// 读取bit值
  bool ReadBit();

  /// 值以bit方式写入
  ///  @param[in]   value     写入的值
  ///  @param[in]   bits      写入位数
  void WriteBits(size_t value, int32_t bits);

  /// 读取bit值
  ///  @param[in]   bits      读出位数
  ///  @return 长度bits位的数据
  uint32_t ReadBits(int32_t bits);

  /// 指定数量的值放置在数据包中的指定位置
  ///  @param[in]   pos       放置值得位置，以bit为单位
  ///  @param[in]   src       写入值的地址
  ///  @param[in]   cnt       写入值的位数
  void Put(size_t pos, const uint8_t *src, size_t cnt);

  /// 指定数量的值放置在数据包中的指定位置
  ///  @tparam      T         写入数据类型
  ///  @param[in]   pos       放置值得位置，以bit为单位
  ///  @param[in]   value     写入值
  template <typename T>
  void Put(size_t pos, T value);

  /// 将指定数量的值位放置在数据包中的指定位置。 为确保正确写入所有位，
  /// 请仅在执行位刷新后调用此方法
  ///  @param[in]   pos       放置值的位置，以bit为单位。
  ///                         整个值必须适合数据包 建议使用 GetWritePos() 函数获取位置。
  ///  @param[in]   value     要写入的数据
  ///  @param[in]   bit_count 写入值得位数
  void PutBits(size_t pos, size_t value, uint32_t bit_count);

  ByteBuffer &operator<<(uint8_t value);
  ByteBuffer &operator<<(uint16_t value);
  ByteBuffer &operator<<(uint32_t value);
  ByteBuffer &operator<<(uint64_t value);
  ByteBuffer &operator<<(int8_t value);
  ByteBuffer &operator<<(int16_t value);
  ByteBuffer &operator<<(int32_t value);
  ByteBuffer &operator<<(int64_t value);
  ByteBuffer &operator<<(float value);
  ByteBuffer &operator<<(double value);
  ByteBuffer &operator<<(const std::string &value);
  ByteBuffer &operator<<(std::string_view value);
  ByteBuffer &operator<<(const char *str);

  ByteBuffer &operator>>(bool &value);
  ByteBuffer &operator>>(uint8_t &value);
  ByteBuffer &operator>>(uint16_t &value);
  ByteBuffer &operator>>(uint32_t &value);
  ByteBuffer &operator>>(uint64_t &value);
  ByteBuffer &operator>>(int8_t &value);
  ByteBuffer &operator>>(int16_t &value);
  ByteBuffer &operator>>(int32_t &value);
  ByteBuffer &operator>>(int64_t &value);
  ByteBuffer &operator>>(float &value);
  ByteBuffer &operator>>(double &value);
  ByteBuffer &operator>>(std::string &value);

  uint8_t &operator[](const size_t pos);
  const uint8_t &operator[](const size_t pos) const;

  /// 读取数据
  ///  @param[out]  desc    读取数据存放地址
  ///  @param[in]   len     读取的长度
  void Read(uint8_t *dest, size_t len);

  /// 读取数据
  ///  @tparam      T       读取数据的类型
  ///  @param[out]  desc    读取数据存放地址
  ///  @param[in]   count   读取的长度
  template <typename T>
  void Read(T *dest, size_t count);

  /// 读取指定类型的数据
  ///  @tparam      T           读取的数据类型
  ///  @tparam      Underlying  T底层类型
  template <typename T, typename Underlying = T>
  T Read();

  /// 读取指定类型的数据
  ///  @tparam      T           读取的数据类型
  ///  @tparam      Underlying  T底层类型
  ///  @param[in]   pos         从post位置开始读取T类型数据
  template <typename T, typename Underlying = T>
  T Read(size_t pos) const;

  /// 读取跳过指定类型
  ///  @tparam      T           读取的数据类型
  template <typename T>
  void ReadSkip();

  /// 读取跳过指定类型
  ///  @param[in]   skip        跳过的长度
  void ReadSkip(size_t skip);

  /// 读取64位无符号整数
  /// 用作GUID使用
  ///  @param[out]  guid        读取存放的64位数据
  void ReadPackedUInt64(uint64_t &guid);

  /// 读取64位无符号整数
  ///  @param[in]   mask        掩码 字节中bit位为1的位置将被读取 64 / 8 对应字节中的8位
  void ReadPackedUInt64(uint8_t mask, uint64_t &value);

  /// 读取指定长度字符串
  ///  @param[in]   length      指定长度
  ///  @return 读取的结果字符串
  std::string ReadString(uint32_t length);

  /// 从字节流中读取时间
  uint32_t ReadPackedTime();

  /// 写入字符串
  ///  @param[in]   str     要写入的字符串
  void WriteString(const std::string &str);

  /// 写入字符串
  ///  @param[in]   strv    要写入的字符串
  void WriteString(std::string_view strv);

  /// 写入字符串
  ///  @param[in]   str     要写入的字符串地址
  ///  @param[in]   len     要写入的字符串长度
  void WriteString(const char *str, size_t len);

  /// 字节流中添加无符号字节数据
  ///  @param[in]   src     插入数据地址
  ///  @param[in]   cnt     插入数据字节个数
  void Append(const uint8_t *src, size_t cnt);

  /// 字节流中添加有符号字节数据
  ///  @param[in]   src     插入数据地址
  ///  @param[in]   cnt     插入数据字节个数
  void Append(const char *src, size_t cnt);

  /// 字节流中添加T类型数据
  ///  @param[in]   src     插入数据地址
  ///  @param[in]   cnt     插入数据字节个数
  template <typename T>
  void Append(const T *src, size_t cnt);

  /// 字节流中添加T类型数据，要求T类型是可平凡复制对象
  ///  @tparam      T       T类型时可平凡复制对象
  ///  @param[in]   value   T类型数据
  template <typename T>
  void Append(T value);

  /// 字节流中添加字节流
  ///  @param[in]   buf     字节流
  void Append(const ByteBuffer &buf);

  /// 字节流中追加stl array数据
  ///  @param[in]   arr     要插入的字节流数据
  template <size_t Size>
  void Append(const std::array<uint8_t, Size> &arr);

  /// 字节流添加时间
  ///  @param[in]   time    时间数据
  void AppendPackedTime(time_t time);

  /// 字节流中添加 xyz类型坐标数据
  void AppendPackXYZ(float x, float y, float z);

  /// 打包64位数据
  ///  @param[in]   value   64位数据
  ///  @param[out]  mask    掩码
  ///  @param[out]  result  结果
  ///  @return 结果大小
  static size_t PackUInt64(uint64_t value, uint8_t *mask, uint8_t *result);

  /// 字节流中添加64位数据
  void AppendPackedUInt64(uint64_t guid);

  /// 打印字节流数据
  /// 日志模式打开 并且允许TRACE模式本函数有效
  void PrintStorage() const;

  /// 字符形式打印字节流数据
  /// 日志模式打开 并且允许TRACE模式本函数有效
  void Textlike() const;

  /// 十六进制形式打印字节流数据
  /// 日志模式打开 并且允许TRACE模式本函数有效
  void Hexlike() const;

 private:
  size_type rpos_{0};       ///< 读位置
  size_type wpos_{0};       ///< 写位置
  size_type bitpos_{0};     ///< 字节流位置
  uint8_t cur_bit_val_{0};  ///< 当前字节流位置的值
  storage_type storage_;    ///< 字节流存储器
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_BYTE_BUFFER_H_
