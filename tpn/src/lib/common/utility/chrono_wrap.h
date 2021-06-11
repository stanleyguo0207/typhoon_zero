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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CHRONO_WRAP_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CHRONO_WRAP_H_

#include <chrono>

namespace tpn {

using NanoSeconds  = std::chrono::nanoseconds;
using MicroSeconds = std::chrono::microseconds;
using MilliSeconds = std::chrono::milliseconds;
using Seconds      = std::chrono::seconds;
using Minutes      = std::chrono::minutes;
using Hours        = std::chrono::hours;

using SystemClock = std::chrono::system_clock;
using SteadyClock = std::chrono::steady_clock;

using namespace std::chrono_literals;

/// 获取给定时间点的秒数除外的部分
///  @tparam	ToDuration	返还的时间单位
///  @tparam	TimePoint		时间点类型 默认系统时钟
///  @param[in]		tp			给定的时间点
///  @return 给定时间点的秒数除外的部分
template <typename ToDuration,
          typename TimePoint = std::chrono::system_clock::time_point>
inline auto TimeFraction(TimePoint tp) {
  using std::chrono::duration_cast;
  using std::chrono::seconds;
  auto duration = tp.time_since_epoch();
  auto secs     = duration_cast<seconds>(duration);
  return duration_cast<ToDuration>(duration) - duration_cast<ToDuration>(secs);
}

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CHRONO_WRAP_H_
