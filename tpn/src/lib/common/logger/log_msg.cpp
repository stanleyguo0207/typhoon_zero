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

#include "log_msg.h"

#include "platform.h"

namespace tpn {

namespace log {

LogMsg::LogMsg(std::string_view logger_name_in, LogLevel level_in,
               LogClock::time_point time_in, SrcLocInfo src_loc_in,
               std::string_view content_in)
    : logger_name(logger_name_in),
      level(level_in),
      time(time_in),
      src_loc(src_loc_in),
      content(content_in),
      thread_id(GetCurrentThreadId()) {}

LogMsg::LogMsg(std::string_view logger_name_in, LogLevel level_in,
               SrcLocInfo src_loc_in, std::string_view content_in)
    : LogMsg(logger_name_in, level_in, LogClock::now(), src_loc_in,
             content_in) {}

LogMsg::LogMsg(std::string_view logger_name_in, LogLevel level_in,
               std::string_view content_in)
    : LogMsg(logger_name_in, level_in, LogClock::now(), SrcLocInfo{},
             content_in) {}

AsyncLogMsg::AsyncLogMsg(const LogMsg &msg) : LogMsg{msg} {
  buf_.append(logger_name.begin(), logger_name.end());
  buf_.append(content.begin(), content.end());
  UpdateBuf();
}

AsyncLogMsg::AsyncLogMsg(const AsyncLogMsg &other) : LogMsg{other} {
  buf_.append(logger_name.begin(), logger_name.end());
  buf_.append(content.begin(), content.end());
  UpdateBuf();
}

AsyncLogMsg::AsyncLogMsg(AsyncLogMsg &&other) noexcept
    : LogMsg{other}, buf_{std::move(other.buf_)} {
  UpdateBuf();
}

AsyncLogMsg &AsyncLogMsg::operator=(const AsyncLogMsg &other) {
  LogMsg::operator=(other);
  buf_.clear();
  buf_.append(other.buf_.data(), other.buf_.data() + other.buf_.size());
  UpdateBuf();
  return *this;
}

AsyncLogMsg &AsyncLogMsg::operator=(AsyncLogMsg &&other) noexcept {
  LogMsg::operator=(other);
  buf_            = std::move(other.buf_);
  UpdateBuf();
  return *this;
}

void AsyncLogMsg::UpdateBuf() {
  logger_name = std::string_view{buf_.data(), logger_name.size()};
  content = std::string_view{buf_.data() + logger_name.size(), content.size()};
}

}  // namespace log

}  // namespace tpn
