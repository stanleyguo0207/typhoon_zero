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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIME_ALIVE_TIME_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIME_ALIVE_TIME_H_

#include "net_common.h"
#include "chrono_wrap.h"

namespace tpn {

namespace net {

template <typename Derived, typename ArgsType = void>
class AliveTime {
 public:
  AliveTime()  = default;
  ~AliveTime() = default;

  TPN_INLINE SystemClock::time_point GetLastAliveTime() const {
    return this->last_alive_time_;
  }

  TPN_INLINE Derived &UpdateAliveTime() {
    this->last_alive_time_ = SystemClock::now();
    NET_DEBUG("update alive time");
    return (CRTP_CAST(this));
  }

  TPN_INLINE SystemClock::duration GetSilenceDuration() const {
    return std::chrono::duration_cast<SystemClock::duration>(
        SystemClock::now() - this->last_alive_time_);
  }

 protected:
  SystemClock::time_point last_alive_time_{
      SystemClock::now()};  ///<  上次存活时间点
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_TIME_ALIVE_TIME_H_
