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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_FWD_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_FWD_H_

#include <memory>
#include <vector>

#include "define.h"
#include "fmt_wrap.h"
#include "chrono_wrap.h"

namespace tpn {

namespace log {

enum class LogLevel : uint8_t;

struct LogMsg;

using LogClock = SystemClock;

class Appender;
using AppenderSptr     = std::shared_ptr<Appender>;
using AppenderInitList = std::initializer_list<AppenderSptr>;
using AppenderSptrVec  = std::vector<AppenderSptr>;

using ErrHandler = std::function<void(std::string_view err_msg)>;

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_FWD_H_
