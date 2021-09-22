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

#include "entity.h"

namespace tpn {

Entity::Entity() {}

Entity::~Entity() {}

Position3D &Entity::GetPosition() { return position_; }

void Entity::SetPosition(const Position3D &pos) {
  // TODO Check
  position_ = pos;
  OnPositionChanged();
}

Direction3D &Entity::GetDirection() { return direction_; }

void Entity::SetDirection(const Direction3D &dir) {
  // TODO Check
  direction_ = dir;
  OnDirectionChanged();
}

void Entity::SetPositionAndDirection(const Position3D &pos,
                                     const Direction3D &dir) {
  // TODO Check
  SetPosition(pos);
  SetDirection(dir);
}

void Entity::OnPositionChanged() {
  // TODO
  UpdateLastPosition();
}

void Entity::OnDirectionChanged() {
  // TODO
}

void Entity::UpdateLastPosition() { last_position_ = this->GetPosition(); }

}  // namespace tpn
