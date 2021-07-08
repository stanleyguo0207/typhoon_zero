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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_PCH_PCH_NET_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_PCH_PCH_NET_H_

#include <set>
#include <mutex>
#include <queue>
#include <chrono>
#include <thread>
#include <atomic>
#include <limits>
#include <memory>
#include <vector>
#include <string>
#include <future>
#include <utility>
#include <algorithm>
#include <functional>
#include <type_traits>
#include <system_error>
#include <shared_mutex>
#include <unordered_map>

#include "define.h"
#include "asio_wrap.h"
#include "chrono_wrap.h"
#include "traits_hub.h"
#include "crtp_object.h"
#include "log.h"
#include "utils.h"
#include "message_buffer.h"
#include "rpc_type.pb.h"
#include "byte_converter.h"


#include "net_common.h"
#include "net_error.h"
#include "io_pool.h"

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_PCH_PCH_NET_H_
