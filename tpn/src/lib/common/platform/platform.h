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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_PLATFORM_PLATFORM_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_PLATFORM_PLATFORM_H_

#include <ctime>

#include <thread>

#include "define.h"

namespace tpn {

/// 获取当前线程id
///  @return 当前线程id
TPN_COMMON_API size_t GetCurrentThreadId() noexcept;

/// 获取指定时间当地时间
///  @param[in]		time_tt			指定时间
///  @return 时间信息
TPN_COMMON_API std::tm Localtime(const std::time_t &time_tt) noexcept;

/// 获取当前时间当地时间
///  @return 时间信息
TPN_COMMON_API std::tm Localtime() noexcept;

/// 获取指定时间UTC时间
///  @param[in]		time_tt			指定时间
///  @return 时间信息
TPN_COMMON_API std::tm GmTime(const std::time_t &time_tt) noexcept;

/// 获取当前时间UTC时间
///  @return 时间信息
TPN_COMMON_API std::tm GmTime() noexcept;

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_PLATFORM_PLATFORM_H_
