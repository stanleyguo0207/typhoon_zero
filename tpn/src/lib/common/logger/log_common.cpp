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

#include "log_common.h"
#include "utils.h"

namespace tpn {

namespace log {

static constexpr std::string_view s_log_level_names[]{
    "OFF", "[TRACE]", "[DEBUG]", " [INFO]", " [WARN]", "[ERROR]", "[FATAL]"};

static constexpr std::string_view s_log_level_short_names[]{
    "[O]", "[T]", "[D]", "[I]", "[W]", "[E]", "[F]"};

std::string_view ToLogLevelStr(LogLevel level) noexcept {
  return s_log_level_names[EnumToUnderlyType(level)];
}

std::string_view ToLogLevelShortStr(LogLevel level) noexcept {
  return s_log_level_short_names[EnumToUnderlyType(level)];
}

LogLevel ToLogLevelEnum(std::string_view name) noexcept {
  uint8_t level = 0;
  for (auto &&level_str : s_log_level_names) {
    if (level_str == name) {
      return static_cast<LogLevel>(level);
    }
    ++level;
  }

  return LogLevel::kLogLevelOff;
}

}  // namespace log

}  // namespace tpn
