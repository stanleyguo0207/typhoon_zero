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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_CELL_CELL_FWD_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_CELL_CELL_FWD_H_

#include <memory>

#include "define.h"

namespace tpn {

class CoordinateNode;
class CoordinateSystem;
class Entity;
class EntityCoordinateNode;

DECL_SHARED_AND_WEAK_PTR(CoordinateNode)
DECL_SHARED_AND_WEAK_PTR(CoordinateSystem)
DECL_SHARED_AND_WEAK_PTR(Entity)
DECL_SHARED_AND_WEAK_PTR(EntityCoordinateNode)

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_CELL_FWD_H_
