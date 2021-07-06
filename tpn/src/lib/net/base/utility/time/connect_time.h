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

#include "net_common.h"
#include "chrono_wrap.h"

namespace tpn {

namespace net {

/// 连接时间
template <typename Derived, typename ArgsType = void>
class ConnectTime {
 public:
  ConnectTime()  = default;
  ~ConnectTime() = default;

  /// 获取连接时间点
  ///  @return 连接时间点
  TPN_INLINE SystemClock::time_point GetConnectTime() const {
    return this->connect_time_;
  }

  /// 重置连接时间点
  TPN_INLINE Derived &ResetConnectTime() {
    this->connect_time_ = SystemClock::now();
    NET_DEBUG("reset connect time");
    return (CRTP_CAST(this));
  }

  /// 获取连接时长
  TPN_INLINE SystemClock::duration GetConnectDuration() const {
    return std::chrono::duration_cast<SystemClock::duration>(
        SystemClock::now() - this->connect_time_);
  }

 protected:
  SystemClock::time_point connect_time_{SystemClock::now()};  ///<  连接时间点
};

}  // namespace net

}  // namespace tpn
