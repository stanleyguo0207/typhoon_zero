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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_ERROR_NET_ERROR_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_ERROR_NET_ERROR_H_

#include <string>
#include <system_error>

namespace tpn {

namespace net {

/// 设置错误码
///  @param[in]  ec    错误码
void SetLastError(int ec);

/// 设置错误码
///  @param[in]   ec    错误码
///  @param[in]   ecat  错误码类型 std::system_category 、 std::iostream_category 等
/// @sa std::system_category
void SetLastError(int ec, const std::error_category &ecat);

/// 设置错误码
///  @param[in]   ec    错误码
void SetLastError(const std::error_code &ec);

/// 设置错误码
///  @param[in]   e     系统错误
void SetLastError(const std::system_error &e);

/// 清空错误码
void ClearLastError();

/// 获取最新的错误码
///  @return 最新的错误码
std::error_code &GetLastError();

/// 获取最新的错误码值
///  @return  最新的错误码值
int GetLastErrorVal();

/// 获取最新的错误码信息
///  @return  最新的错误码信息
std::string GetLastErrorMsg();

thread_local static std::error_code s_ec_ignore;  ///< 全局错误码占位用

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_UTILITY_ERROR_NET_ERROR_H_
