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

#include "appender.h"

namespace tpn {

namespace log {

void Appender::SetLevel(LogLevel level) {
  level_.store(level, std::memory_order_relaxed);
}

LogLevel Appender::GetLevel() const {
  return static_cast<LogLevel>(level_.load(std::memory_order_relaxed));
}

bool Appender::ShouldLog(LogLevel level) const {
  auto level_need = level_.load(std::memory_order_relaxed);
  return (LogLevel::kLogLevelOff != level_need) && (level >= level_need);
}

}  // namespace log

}  // namespace tpn
