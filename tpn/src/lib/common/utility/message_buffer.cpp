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
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include <cstring>

#include "message_buffer.h"

namespace tpn {

MessageBuffer::MessageBuffer() : wpos_(0), rpos_(0), storage_() {
  storage_.resize(4096);
}

MessageBuffer::MessageBuffer(size_type initial_size)
    : wpos_(0), rpos_(0), storage_() {
  storage_.resize(initial_size);
}

MessageBuffer::MessageBuffer(const MessageBuffer &other)
    : wpos_(other.wpos_), rpos_(other.rpos_), storage_(other.storage_) {}

MessageBuffer::MessageBuffer(MessageBuffer &&other)
    : wpos_(other.wpos_), rpos_(other.rpos_), storage_(other.Move()) {}

MessageBuffer &MessageBuffer::operator=(const MessageBuffer &other) {
  if (this != &other) {
    wpos_    = other.wpos_;
    rpos_    = other.rpos_;
    storage_ = other.storage_;
  }

  return *this;
}

MessageBuffer &MessageBuffer::operator=(MessageBuffer &&other) {
  if (this != &other) {
    wpos_    = other.wpos_;
    rpos_    = other.rpos_;
    storage_ = other.Move();
  }

  return *this;
}

void MessageBuffer::Rest() {
  wpos_ = 0;
  rpos_ = 0;
}

void MessageBuffer::Resize(size_type bytes) { storage_.resize(bytes); }

MessageBuffer::pointer MessageBuffer::GetBasePointer() {
  return storage_.data();
}

MessageBuffer::pointer MessageBuffer::GetReadPointer() {
  return storage_.data() + rpos_;
}

MessageBuffer::pointer MessageBuffer::GetWritePointer() {
  return storage_.data() + wpos_;
}

void MessageBuffer::ReadCompleted(size_type bytes) { rpos_ += bytes; }

void MessageBuffer::WriteCompleted(size_type bytes) { wpos_ += bytes; }

MessageBuffer::size_type MessageBuffer::GetActiveSize() const {
  return wpos_ - rpos_;
}

MessageBuffer::size_type MessageBuffer::GetRemainingSpace() const {
  return storage_.size() - wpos_;
}

MessageBuffer::size_type MessageBuffer::GetBufferSize() const {
  return storage_.size();
}

void MessageBuffer::Normalize() {
  if (rpos_) {
    if (rpos_ != wpos_) {
      memmove(GetBasePointer(), GetReadPointer(), GetActiveSize());
    }
    wpos_ = rpos_;
    rpos_ = 0;
  }
}

void MessageBuffer::EnsureFreeSpace() {
  if (0 == GetRemainingSpace()) {
    storage_.resize(storage_.size() * 1.5);
  }
}

void MessageBuffer::Write(const void *data, size_type size) {
  if (size) {
    memcpy(GetWritePointer(), data, size);
    WriteCompleted(size);
  }
}

MessageBuffer::storage_type &&MessageBuffer::Move() {
  wpos_ = 0;
  rpos_ = 0;
  return std::move(storage_);
}

}  // namespace tpn
