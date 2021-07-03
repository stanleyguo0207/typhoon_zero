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

#include "byte_buffer.h"

#include <sstream>

#include "exception_hub.h"
#include "byte_converter.h"
#include "platform.h"
#include "log.h"
#include "message_buffer.h"

namespace tpn {

ByteBuffer::ByteBuffer()
    : rpos_{0}, wpos_{0}, bitpos_{kInitialBitPos}, cur_bit_val_{0} {
  storage_.reserve(kByteBufferDefaultSize);
}

ByteBuffer::ByteBuffer(size_t size, ReserveFlag)
    : rpos_{0}, wpos_{0}, bitpos_{kInitialBitPos}, cur_bit_val_{0} {
  storage_.reserve(size);
}

ByteBuffer::ByteBuffer(size_t size, ResizeFlag)
    : rpos_{0}, wpos_{0}, bitpos_{kInitialBitPos}, cur_bit_val_{0} {
  storage_.resize(size);
}

ByteBuffer::ByteBuffer(MessageBuffer &&buffer)
    : rpos_{0},
      wpos_{0},
      bitpos_{kInitialBitPos},
      cur_bit_val_{0},
      storage_(buffer.Move()) {}

ByteBuffer::storage_type &&ByteBuffer::Move() noexcept {
  rpos_        = 0;
  wpos_        = 0;
  bitpos_      = kInitialBitPos;
  cur_bit_val_ = 0;
  return std::move(storage_);
}

ByteBuffer::ByteBuffer(ByteBuffer &&other) noexcept
    : rpos_{other.rpos_},
      wpos_{other.wpos_},
      bitpos_{other.bitpos_},
      cur_bit_val_{other.cur_bit_val_},
      storage_{other.Move()} {}

ByteBuffer &ByteBuffer::operator=(const ByteBuffer &other) {
  if (this != &other) {
    rpos_        = 0;
    wpos_        = 0;
    bitpos_      = kInitialBitPos;
    cur_bit_val_ = 0;
    storage_     = other.storage_;
  }
  return *this;
}

ByteBuffer &ByteBuffer::operator=(ByteBuffer &&other) noexcept {
  if (this != &other) {
    rpos_        = 0;
    wpos_        = 0;
    bitpos_      = kInitialBitPos;
    cur_bit_val_ = 0;
    storage_     = other.Move();
  }
  return *this;
}

size_t ByteBuffer::GetReadPos() const { return rpos_; }

size_t ByteBuffer::SetReadPos(size_t rpos) {
  rpos_ = rpos;
  return rpos_;
}

size_t ByteBuffer::GetWritePos() const { return wpos_; }

size_t ByteBuffer::SetWritePos(size_t wpos) {
  wpos_ = wpos;
  return wpos_;
}

size_t ByteBuffer::GetLastWriteBitPos() const {
  return wpos_ * kInitialBitPos + kInitialBitPos - bitpos_;
}

size_t ByteBuffer::SetLastWriteBitPos(size_t new_pos) {
  wpos_   = new_pos / kInitialBitPos;
  bitpos_ = kInitialBitPos - (new_pos % kInitialBitPos);
  return wpos_ * kInitialBitPos + kInitialBitPos - bitpos_;
}

uint8_t *ByteBuffer::GetContents() {
  if (storage_.empty()) {
    TPN_THROW(TpnException());
  }
  return storage_.data();
}

const uint8_t *ByteBuffer::GetContents() const {
  if (storage_.empty()) {
    TPN_THROW(TpnException());
  }
  return storage_.data();
}

size_t ByteBuffer::GetSize() const { return storage_.size(); }

bool ByteBuffer::IsEmpty() const { return storage_.empty(); }

void ByteBuffer::Resize(size_t new_size) {
  storage_.resize(new_size, 0);
  rpos_ = 0;
  wpos_ = GetSize();
}

void ByteBuffer::Reserve(size_t res_size) {
  if (res_size > storage_.size()) {
    storage_.reserve(res_size);
  }
}

void ByteBuffer::Clear() {
  rpos_        = 0;
  wpos_        = 0;
  bitpos_      = kInitialBitPos;
  cur_bit_val_ = 0;
  storage_.clear();
}

bool ByteBuffer::HasUnfinishedBitPack() const {
  return bitpos_ != kInitialBitPos;
}

void ByteBuffer::FlushBits() {
  if (kInitialBitPos == bitpos_) {
    return;
  }
  bitpos_ = kInitialBitPos;
  Append(reinterpret_cast<uint8_t *>(&cur_bit_val_), sizeof(uint8_t));
  cur_bit_val_ = 0;
}

void ByteBuffer::ReadFinish() { rpos_ = GetWritePos(); }

void ByteBuffer::ResetBitPos() {
  if (bitpos_ > kBoundaryBitPos) {
    return;
  }
  bitpos_      = kInitialBitPos;
  cur_bit_val_ = 0;
}

bool ByteBuffer::WriteBit(bool bit) {
  --bitpos_;
  if (bit) {
    cur_bit_val_ |= (1 << (bitpos_));
  }
  if (0 == bitpos_) {
    bitpos_ = kInitialBitPos;
    Append(reinterpret_cast<uint8_t *>(&cur_bit_val_), sizeof(uint8_t));
    cur_bit_val_ = 0;
  }
  return bit;
}

bool ByteBuffer::ReadBit() {
  ++bitpos_;
  if (bitpos_ > kBoundaryBitPos) {
    cur_bit_val_ = Read<uint8_t>();
    bitpos_      = 0;
  }
  return 0 != ((cur_bit_val_ >> (kBoundaryBitPos - bitpos_)) & 0x1);
}

void ByteBuffer::WriteBits(size_t value, int32_t bits) {
  for (int32_t i = bits - 1; i >= 0; --i) {
    WriteBit((value >> i) & 0x1);
  }
}

uint32_t ByteBuffer::ReadBits(int32_t bits) {
  uint32_t value = 0;

  for (int32_t i = bits - 1; i >= 0; --i) {
    if (ReadBit()) {
      value |= (0x1 << (i));
    }
  }

  return value;
}

void ByteBuffer::Put(size_t pos, const uint8_t *src, size_t cnt) {
  TPN_ASSERT(pos + cnt <= GetSize(),
             fmt::format("Attempted to put value with size: {} "
                         "in ByteBuffer (pos: {} size: {})",
                         cnt, pos, GetSize())
                 .c_str());
  TPN_ASSERT(src,
             "Attempted to put a NULL-pointer in ByteBuffer (pos: {} size: {})",
             pos, GetSize());
  TPN_ASSERT(cnt,
             "Attempted to put a zero-sized value in "
             "ByteBuffer (pos: {} size: {})",
             pos, GetSize());
  std::memcpy(&storage_[pos], src, cnt);
}

template <typename T>
void ByteBuffer::Put(size_t pos, T value) {
  static_assert(std::is_trivially_copyable<T>::value,
                "Put(size_t, T) must be used with trivially copyable types");
  tpn::EndianRefMakeLittle(value);
  Put(pos, reinterpret_cast<uint8_t *>(&value), sizeof(value));
}

void ByteBuffer::PutBits(size_t pos, size_t value, uint32_t bit_count) {
  TPN_ASSERT(pos + bit_count <= GetSize() * kInitialBitPos,
             "Attempted to put {} bits in ByteBuffer (bitpos: {} size: {})",
             bit_count, pos, GetSize());
  TPN_ASSERT(bit_count, "Attempted to put a zero bits in ByteBuffer");

  for (uint32_t i = 0; i < bit_count; ++i) {
    size_t wpos = (pos + i) / kInitialBitPos;
    size_t bit  = (pos + i) % kInitialBitPos;
    if ((value >> (bit_count - i - 1)) & 0x1) {
      storage_[wpos] |= 1 << (kBoundaryBitPos - bit);
    } else {
      storage_[wpos] &= ~(1 << (kBoundaryBitPos - bit));
    }
  }
}

ByteBuffer &ByteBuffer::operator<<(uint8_t value) {
  Append<uint8_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(uint16_t value) {
  Append<uint16_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(uint32_t value) {
  Append<uint32_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(uint64_t value) {
  Append<uint64_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(int8_t value) {
  Append<int8_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(int16_t value) {
  Append<int16_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(int32_t value) {
  Append<int32_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(int64_t value) {
  Append<int64_t>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(float value) {
  Append<float>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(double value) {
  Append<double>(value);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(const std::string &value) {
  if (size_t len = value.length()) {
    Append(reinterpret_cast<const uint8_t *>(value.c_str()), len);
  }
  Append<uint8_t>(0);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(std::string_view value) {
  if (size_t len = value.length()) {
    Append(reinterpret_cast<const uint8_t *>(value.data()), len);
  }
  Append<uint8_t>(0);
  return *this;
}

ByteBuffer &ByteBuffer::operator<<(const char *str) {
  if (size_t len = (str ? strlen(str) : 0)) {
    Append(reinterpret_cast<const uint8_t *>(str), len);
  }
  Append<uint8_t>(0);
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(bool &value) {
  value = Read<char>() > 0 ? true : false;
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(uint8_t &value) {
  value = Read<uint8_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(uint16_t &value) {
  value = Read<uint16_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(uint32_t &value) {
  value = Read<uint32_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(uint64_t &value) {
  value = Read<uint64_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(int8_t &value) {
  value = Read<uint8_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(int16_t &value) {
  value = Read<int16_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(int32_t &value) {
  value = Read<int32_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(int64_t &value) {
  value = Read<int64_t>();
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(float &value) {
  value = Read<float>();
  if (!std::isfinite(value)) {
    TPN_THROW(TpnException());
  }
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(double &value) {
  value = Read<double>();
  if (!std::isfinite(value)) {
    TPN_THROW(TpnException());
  }
  return *this;
}

ByteBuffer &ByteBuffer::operator>>(std::string &value) {
  value.clear();
  while (GetReadPos() < GetSize()) {
    char c = Read<char>();
    if (0 == c) {
      break;
    }
    value += c;
  }
  return *this;
}

uint8_t &ByteBuffer::operator[](size_t pos) {
  if (pos >= GetSize()) {
    TPN_THROW(ByteBufferException(pos, 1, GetSize()));
  }
  return storage_[pos];
}

const uint8_t &ByteBuffer::operator[](size_t pos) const {
  if (pos >= GetSize()) {
    TPN_THROW(ByteBufferException(pos, 1, GetSize()));
  }
  return storage_[pos];
}

void ByteBuffer::Read(uint8_t *dest, size_t len) {
  if (rpos_ + len > GetSize()) {
    TPN_THROW(ByteBufferException(rpos_, len, GetSize()));
  }

  ResetBitPos();
  std::memcpy(dest, &storage_[rpos_], len);
  rpos_ += len;
}

template <typename T>
void ByteBuffer::Read(T *dest, size_t count) {
  static_assert(std::is_trivially_copyable<T>::value,
                "Read(T*, size_t) must be used with trivially copyable types");
  return Read(reinterpret_cast<uint8_t *>(dest), count * sizeof(T));
}

template <typename T, typename Underlying /* = T */>
T ByteBuffer::Read() {
  ResetBitPos();
  T r = Read<T, Underlying>(rpos_);
  rpos_ += sizeof(Underlying);
  return r;
}

template <typename T, typename Underlying /* = T */>
T ByteBuffer::Read(size_t pos) const {
  if (pos + sizeof(Underlying) > GetSize()) {
    TPN_THROW(ByteBufferException(pos, sizeof(Underlying), GetSize()));
  }
  Underlying val;
  std::memcpy(&val, &storage_[pos], sizeof(Underlying));
  tpn::EndianRefMakeLittle(val);
  return static_cast<T>(val);
}

template <typename T>
void ByteBuffer::ReadSkip() {
  ReadSkip(sizeof(T));
}

template <>
void ByteBuffer::ReadSkip<char *>() {
  std::string temp;
  *this >> temp;
}

template <>
void ByteBuffer::ReadSkip<const char *>() {
  ReadSkip<char *>();
}

template <>
void ByteBuffer::ReadSkip<std::string>() {
  ReadSkip<char *>();
}

template <>
void ByteBuffer::ReadSkip<std::string_view>() {
  ReadSkip<char *>();
}

void ByteBuffer::ReadSkip(size_t skip) {
  if (rpos_ + skip > GetSize()) {
    TPN_THROW(ByteBufferException(rpos_, skip, GetSize()));
  }
  ResetBitPos();
  rpos_ += skip;
}

void ByteBuffer::ReadPackedUInt64(uint64_t &guid) {
  guid = 0;
  ReadPackedUInt64(Read<uint8_t>(), guid);
}

void ByteBuffer::ReadPackedUInt64(uint8_t mask, uint64_t &value) {
  for (uint32_t i = 0; i < kInitialBitPos; ++i) {
    if (mask & (uint8_t(1) << i)) {
      value |= (uint64_t(Read<uint8_t>()) << (i * kInitialBitPos));
    }
  }
}

std::string ByteBuffer::ReadString(uint32_t length) {
  if (rpos_ + length > GetSize()) {
    TPN_THROW(ByteBufferException(rpos_, length, GetSize()));
  }

  ResetBitPos();
  if (!length) {
    return std::string();
  }

  std::string str(reinterpret_cast<const char *>(&storage_[rpos_]), length);
  rpos_ += length;
  return str;
}

uint32_t ByteBuffer::ReadPackedTime() {
  uint32_t packed_date = Read<uint32_t>();

  auto lt    = std::tm();
  lt.tm_min  = packed_date & 0x3F;
  lt.tm_hour = (packed_date >> 6) & 0x1F;
  // lt.tm_wday = (packed_date >> 11) & 7;
  lt.tm_mday = ((packed_date >> 14) & 0x3F) + 1;
  lt.tm_mon  = (packed_date >> 20) & 0xF;
  lt.tm_year = ((packed_date >> 24) & 0x1F) + 100;

  return static_cast<uint32_t>(mktime(&lt));
}

void ByteBuffer::WriteString(const std::string &str) {
  if (size_t len = str.length()) {
    Append(str.c_str(), len);
  }
}

void ByteBuffer::WriteString(std::string_view strv) {
  if (size_t len = strv.length()) {
    Append(strv.data(), len);
  }
}

void ByteBuffer::WriteString(const char *str, size_t len) {
  if (len) {
    Append(str, len);
  }
}

void ByteBuffer::Append(const uint8_t *src, size_t cnt) {
  TPN_ASSERT(src,
             "Attempted to put a NULL-pointer in ByteBuffer (pos: {} size: {})",
             wpos_, GetSize());
  TPN_ASSERT(
      cnt,
      "Attempted to put a zero-sized value in ByteBuffer (pos: {} size: {})",
      wpos_, GetSize());
  TPN_ASSERT(GetSize() < 10000000,
             "Attempted to put larger value in ByteBuffer (pos: {} size: {})",
             wpos_, GetSize());

  FlushBits();

  size_t const newSize = wpos_ + cnt;
  if (storage_.capacity() < newSize) {
    if (newSize < 100) {
      storage_.reserve(300);
    } else if (newSize < 750) {
      storage_.reserve(2500);
    } else if (newSize < 6000) {
      storage_.reserve(10000);
    } else {
      storage_.reserve(400000);
    }
  }

  if (storage_.size() < newSize) {
    storage_.resize(newSize);
  }
  std::memcpy(&storage_[wpos_], src, cnt);
  wpos_ = newSize;
}

void ByteBuffer::Append(const char *src, size_t cnt) {
  return Append(reinterpret_cast<const uint8_t *>(src), cnt);
}

template <typename T>
void ByteBuffer::Append(const T *src, size_t cnt) {
  return Append(reinterpret_cast<const uint8_t *>(src), cnt * sizeof(T));
}

template <typename T>
void ByteBuffer::Append(T value) {
  static_assert(std::is_trivially_copyable<T>::value,
                "Append(T) must be used with trivially copyable types");
  tpn::EndianRefMakeLittle(value);
  Append(reinterpret_cast<uint8_t *>(&value), sizeof(value));
}

void ByteBuffer::Append(const ByteBuffer &buf) {
  if (!buf.IsEmpty()) {
    Append(buf.GetContents(), buf.GetSize());
  }
}

template <size_t Size>
void ByteBuffer::Append(const std::array<uint8_t, Size> &arr) {
  Append(arr.data(), Size);
}

void ByteBuffer::AppendPackedTime(time_t time) {
  auto lt = Localtime();
  Append<uint32_t>((lt.tm_year - 100) << 24 | lt.tm_mon << 20 |
                   (lt.tm_mday - 1) << 14 | lt.tm_wday << 11 | lt.tm_hour << 6 |
                   lt.tm_min);
}

void ByteBuffer::AppendPackXYZ(float x, float y, float z) {
  uint32_t packed = 0;
  packed |= (static_cast<int>(x / 0.25f) & 0x7FF);
  packed |= (static_cast<int>(y / 0.25f) & 0x7FF) << 11;
  packed |= (static_cast<int>(z / 0.25f) & 0x3FF) << 22;
  *this << packed;
}

size_t ByteBuffer::PackUInt64(uint64_t value, uint8_t *mask, uint8_t *result) {
  size_t result_size = 0;
  *mask              = 0;
  memset(result, 0, kInitialBitPos);

  for (uint8_t i = 0; value != 0; ++i) {
    if (value & 0xFF) {
      *mask |= uint8_t(1 << i);
      result[result_size++] = uint8_t(value & 0xFF);
    }

    value >>= 8;
  }

  return result_size;
}

void ByteBuffer::AppendPackedUInt64(uint64_t guid) {
  uint8_t mask = 0;
  size_t pos   = GetWritePos();
  *this << uint8_t(mask);

  uint8_t packed[8];
  if (size_t packed_size = PackUInt64(guid, &mask, packed)) {
    Append(packed, packed_size);
  }
  Put<uint8_t>(pos, mask);
}

void ByteBuffer::PrintStorage() const {
  // 这里默认判断的是默认日志记录器 没有特化去处理网络模块的
  if (!g_log_hub->GetDefaultLoggerRaw() ||
      !g_log_hub->GetDefaultLoggerRaw()->ShouldLog(
          log::LogLevel::kLogLevelTrace)) {
    return;
  }
  std::ostringstream os;
  fmt::print(os, "STORAGE SIZE: {}\n", GetSize());
  for (uint32_t i = 0; i < GetSize(); ++i) {
    fmt::print(os, "{} - ", Read<uint8_t>(i));
  }
  fmt::print(os, " ");
  LOG_TRACE("BYTEBUFFER PRINT\n{}\n", os.str().c_str());
}

void ByteBuffer::Textlike() const {
  // 这里默认判断的是默认日志记录器 没有特化去处理网络模块的
  if (!g_log_hub->GetDefaultLoggerRaw() ||
      !g_log_hub->GetDefaultLoggerRaw()->ShouldLog(
          log::LogLevel::kLogLevelTrace)) {
    return;
  }
  std::ostringstream os;
  fmt::print(os, "STORAGE SIZE: {}\n", GetSize());
  for (uint32_t i = 0; i < GetSize(); ++i) {
    fmt::print(os, "{}", Read<uint8_t>(i));
  }
  fmt::print(os, " ");
  LOG_TRACE("BYTEBUFFER PRINT\n{}\n", os.str().c_str());
}

void ByteBuffer::Hexlike() const {
  // 这里默认判断的是默认日志记录器 没有特化去处理网络模块的
  if (!g_log_hub->GetDefaultLoggerRaw() ||
      !g_log_hub->GetDefaultLoggerRaw()->ShouldLog(
          log::LogLevel::kLogLevelTrace)) {
    return;
  }
  std::ostringstream os;
  fmt::print(os, "STORAGE SIZE: {}\n", GetSize());
  uint32_t j = 1;
  uint32_t k = 1;
  for (uint32_t i = 0; i < GetSize(); ++i) {
    char buf[4];
    snprintf(buf, 4, "%2X ", Read<uint8_t>(i));
    if ((i == (j * 8)) && ((i != (k * 16)))) {
      fmt::print(os, "| ");
      ++j;

    } else if (i == (k * 16)) {
      fmt::print("\n");
      ++k;
      ++j;
    }
    fmt::print(os, "{}", buf);
  }
  fmt::print(os, " ");
  LOG_TRACE("BYTEBUFFER PRINT\n{}\n", os.str().c_str());
}

template TPN_COMMON_API ByteBuffer::ByteBuffer(BufferWrap<LinearBuffer> &);

template TPN_COMMON_API ByteBuffer::ByteBuffer(BufferWrap<asio::streambuf> &);

}  // namespace tpn
