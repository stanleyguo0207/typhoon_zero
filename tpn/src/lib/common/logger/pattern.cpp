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

#include "pattern.h"

#include "platform.h"

namespace tpn {

namespace log {

void Pattern::Init() {}

std::tm Pattern::GetTime(const LogMsg &msg) const {
  if (PatternTimeType::kPatternTimeTypeLocal == pattern_time_type_) {
    return Localtime(LogClock::to_time_t(msg.time));
  } else {
    return GmTime(LogClock::to_time_t(msg.time));
  }
}

TPN_SINGLETON_IMPL(Pattern)

}  // namespace log

}  // namespace tpn
