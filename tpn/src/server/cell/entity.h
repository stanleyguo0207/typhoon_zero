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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_H_

#include "g3d_wrap.h"
#include "cell_fwd.h"

namespace tpn {

/// 实体基类
class Entity {
 public:
  Entity();
  virtual ~Entity();

 protected:
  Position3D position_;       ///< 空间坐标向量
  Position3D last_position_;  ///< 上一个空间坐标向量
  Direction3D direction_;     ///< 空间方向向量
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_H_
