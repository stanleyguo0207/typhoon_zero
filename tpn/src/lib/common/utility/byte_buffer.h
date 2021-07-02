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

template <typename, bool>
class BufferWrap;

/// 字节流
class TPN_COMMON_API ByteBuffer {
  using storage_type = std::vector<uint8_t>;
  using size_type    = storage_type::size_type;

 public:
  static constexpr size_t kByteBufferDefaultSize = 0x1000;  ///< 默认字节流大小
  static constexpr uint8_t kBoundaryBitPos       = 7;  ///< 边界字节位置
  static constexpr uint8_t kInitialBitPos = 8;  ///< 字节位初始位置

  /// 构造函数预留标志
  struct ReserveFlag {};
  /// 构造函数重置标志
  struct ResizeFlag {};

  ByteBuffer();

  ByteBuffer(size_t size, ReserveFlag);

  ByteBuffer(size_t size, ResizeFlag);

  template <typename BufferType, bool b>
  ByteBuffer(BufferWrap<BufferType, b> &buffer);

  storage_type &&Move() noexcept;

  ByteBuffer(ByteBuffer &&other) noexcept;

  ByteBuffer(const ByteBuffer &other) = default;

  ByteBuffer &operator=(const ByteBuffer &other);

  ByteBuffer &operator=(ByteBuffer &&other) noexcept;

  virtual ~ByteBuffer() = default;

  size_t GetReadPos() const;
  size_t SetReadPos(size_t rpos);

  size_t GetWritePos() const;
  size_t SetWritePos(size_t wpos);

  size_t GetLastWriteBitPos() const;
  size_t SetLastWriteBitPos(size_t new_pos);

  uint8_t *GetContents();

  const uint8_t *GetContents() const;

  uint8_t *GetBasePointer();

  uint8_t *GetReadPointer();

  uint8_t *GetWritePointer();

  void ReadCompleted(size_t bytes);

  void WriteCompleted(size_t bytes);

  size_t GetSize() const;

  size_t GetRemainingSpace() const;

  bool IsEmpty() const;

  void Resize(size_t new_size);

  void Reserve(size_t res_size);

  void Clear();

  bool HasUnfinishedBitPack() const;

  void FlushBits();

  void ReadFinish();

  void ResetBitPos();

  bool WriteBit(bool bit);

  bool ReadBit();

  void WriteBits(size_t value, int32_t bits);

  uint32_t ReadBits(int32_t bits);

  void Put(size_t pos, const uint8_t *src, size_t cnt);

  template <typename T>
  void Put(size_t pos, T value);

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
  ByteBuffer &operator<<(std::string_view &value);
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

  void Read(uint8_t *dest, size_t len);

  template <typename T>
  void Read(T *dest, size_t count);

  template <typename T, typename Underlying = T>
  T Read();

  template <typename T, typename Underlying = T>
  T Read(size_t pos) const;

  template <typename T>
  void ReadSkip();

  void ReadSkip(size_t skip);

  void ReadPackedUInt64(uint64_t &guid);

  void ReadPackedUInt64(uint8_t mask, uint64_t &value);

  std::string ReadString(uint32_t length);

  uint32_t ReadPackedTime();

  void WriteString(const std::string &str);

  void WriteString(std::string_view strv);

  void WriteString(const char *str, size_t len);

  void Append(const uint8_t *src, size_t cnt);

  void Append(const char *src, size_t cnt);

  template <typename T>
  void Append(const T *src, size_t cnt);

  template <typename T>
  void Append(T value);

  void Append(const ByteBuffer &buf);

  template <size_t Size>
  void Append(const std::array<uint8_t, Size> &arr);

  void AppendPackedTime(time_t time);

  void AppendPackXYZ(float x, float y, float z);

  static size_t PackUInt64(uint64_t value, uint8_t *mask, uint8_t *result);

  void AppendPackedUInt64(uint64_t guid);

  void PrintStorage() const;

  void Textlike() const;

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
