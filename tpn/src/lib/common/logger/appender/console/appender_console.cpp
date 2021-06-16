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

#include "appender_console.h"

#include "log_msg.h"
#include "utils.h"
#include "platform.h"

namespace tpn {

namespace log {

AppenderConsoleBase::AppenderConsoleBase(std::FILE *file,
                                         AppenderColorMode mode)
    : file_(file) {
  SetColorMode(mode);
  colors_[EnumToUnderlyType(LogLevel::kLogLevelTrace)] =
      fmt::fg(fmt::color::white);
  colors_[EnumToUnderlyType(LogLevel::kLogLevelDebug)] =
      fmt::fg(fmt::color::cyan);
  colors_[EnumToUnderlyType(LogLevel::kLogLevelInfo)] =
      fmt::fg(fmt::color::green);
  colors_[EnumToUnderlyType(LogLevel::kLogLevelWarn)] =
      fmt::fg(fmt::color::yellow) | fmt::emphasis::bold;
  colors_[EnumToUnderlyType(LogLevel::kLogLevelError)] =
      fmt::fg(fmt::color::red) | fmt::emphasis::bold;
  colors_[EnumToUnderlyType(LogLevel::kLogLevelFatal)] =
      fmt::fg(fmt::color::white) | fmt::bg(fmt::color::red) |
      fmt::emphasis::bold;
}

void AppenderConsoleBase::SetColor(LogLevel level, fmt::text_style color) {
  std::lock_guard<std::mutex> lock(color_mutex_);
  colors_[EnumToUnderlyType(level)] = color;
}

void AppenderConsoleBase::SetColorMode(AppenderColorMode mode) {
  switch (mode) {
    case AppenderColorMode::kAppenderColorModeAlways: {
      should_do_colors_ = true;
    } break;
    case AppenderColorMode::kAppenderColorModeAutomatic: {
      should_do_colors_ = InTerminal(file_) && IsColorTerminal();
    } break;
    case AppenderColorMode::kAppenderColorModeNever: {
      should_do_colors_ = false;
    } break;
    default: {
      should_do_colors_ = false;
    } break;
  }
}

void AppenderConsoleBase::DoLog(const LogMsg &msg) {
  if (should_do_colors_) {
    fmt::print(file_, colors_[EnumToUnderlyType(msg.level)], "{}{}", msg,
               TPN_EOL);
  } else {
    fmt::print(file_, "{}{}", msg, TPN_EOL);
  }
  fflush(file_);
}

void AppenderConsoleBase::DoFlush() { fflush(file_); }

AppenderConsoleStdout::AppenderConsoleStdout(
    AppenderColorMode
        mode /*  = AppenderColorMode::kAppenderColorModeAutomatic */)
    : AppenderConsoleBase(stdout, mode) {}

AppenderConsoleStderr::AppenderConsoleStderr(
    AppenderColorMode
        mode /*  = AppenderColorMode::kAppenderColorModeAutomatic */)
    : AppenderConsoleBase(stderr, mode) {}

}  // namespace log

}  // namespace tpn
