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

#include "exception_hub.h"

namespace tpn {

FileException::FileException(std::string_view msg) {
  Message() = {msg.data(), msg.size()};
}

LogException::LogException(std::string_view msg) {
  Message() = {msg.data(), msg.size()};
}

ByteBufferException::ByteBufferException(size_t pos, size_t size,
                                         size_t val_size) {
  Message().assign(
      "Attempted to get value with size: {} in ByteBuffer (pos: {} size: {})"_format(
          val_size, pos, size));
}

}  // namespace tpn
