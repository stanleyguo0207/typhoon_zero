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

#include "utils.h"

namespace tpn {

namespace net {

static constexpr std::string_view s_net_state_names[]{"STOPPED", "STOPPING",
                                                      "STARTING", "STARTED"};

static constexpr std::string_view s_listener_event_names[]{
    "RECV",    "CONNECT", "DISCONNECT", "ACCEPT", "HANDSHAKE",
    "UPGRADE", "INIT",    "START",      "STOP",   "UNKNOWN"};

std::string_view ToNetStateStr(NetState state) noexcept {
  return s_net_state_names[EnumToUnderlyType(state)];
}

std::string_view ToListenerEventStr(ListenerEvent event) noexcept {
  return s_listener_event_names[EnumToUnderlyType(event)];
}

}  // namespace net

}  // namespace tpn
