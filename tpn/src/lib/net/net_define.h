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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_DEFINE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_DEFINE_H_

#include "define.h"

namespace tpn {

namespace net {

// template <typename>
#define TPN_NET_TEMPLATE_DECL_1 template <typename>

// template <typename, typename>
#define TPN_NET_TEMPLATE_DECL_2 template <typename, typename>

// template <typename, bool>
#define TPN_NET_TEMPLATE_DECL_2_BOOL template <typename, bool>

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_NET_DEFINE_H_
