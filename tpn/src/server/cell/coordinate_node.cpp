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

#include "coordinate_node.h"

#include "debug_hub.h"
#include "fmt_wrap.h"
#include "log.h"
#include "coordinate_system.h"

namespace tpn {

CoordinateNode::CoordinateNode(
    CoordinateSystem *coordinate_system_ptr /*= nullptr*/)
    : coordinate_system_ptr_(coordinate_system_ptr) {}

CoordinateNode::~CoordinateNode() {
  TPN_ASSERT(nullptr == prev_x_ptr_ && nullptr == next_x_ptr_ &&
                 nullptr == prev_y_ptr_ && nullptr == next_y_ptr_ &&
                 nullptr == prev_z_ptr_ && nullptr == next_z_ptr_ &&
                 nullptr == coordinate_system_ptr_,
             "AOINode memory leak);
}

float CoordinateNode::GetX() const { return position_.x; }

void CoordinateNode::SetX(float v) { position_.x = v; }

float CoordinateNode::GetY() const { return position_.y; }

void CoordinateNode::SetY(float v) { position_.y = v; }

float CoordinateNode::GetZ() const { return position_.z; }

void CoordinateNode::SetZ(float v) { position_.z = v; }

Position3D CoordinateNode::GetPosition() const { return position_; }

void CoordinateNode::SetPosition(const Position3D &v) { position_ = v; }

float CoordinateNode::GetRealX() const { return 0.f; }

float CoordinateNode::GetRealY() const { return 0.f; }

float CoordinateNode::GetRealZ() const { return 0.f; }

Position3D CoordinateNode::GetRealPosition() const {
  return Position3D(0.f, 0.f, 0.f);
}

float CoordinateNode::GetOldRealX() const { return old_real_position_.x; }

void CoordinateNode::SetOldRealX(float v) { old_real_position_.x = v; }

float CoordinateNode::GetOldRealY() const { return old_real_position_.y; }

void CoordinateNode::SetOldRealY(float v) { old_real_position_.y = v; }

float CoordinateNode::GetOldRealZ() const { return old_real_position_.z; }

void CoordinateNode::SetOldRealZ(float v) { old_real_position_.z = v; }

Position3D CoordinateNode::GetOldRealPosition() const {
  return old_real_position_;
}

void CoordinateNode::SetOldRealPosition(const Position3D &v) {
  old_real_position_ = v;
}

void CoordinateNode::ResetOldRealXYZ() {
  old_real_position_ = GetRealPosition();
}

uint32_t CoordinateNode::GetFlags() const {}

void CoordinateNode::SetFlag(CoordinateNodeFlag flag) {}

void CoordinateNode::AddFlag(CoordinateNodeFlag flag) {}

void CoordinateNode::RemoveFlag(CoordinateNodeFlag flag) {}

bool CoordinateNode::HasFlag(CoordinateNodeFlag flag) const {}

bool CoordinateNode::HasAllFlag(CoordinateNodeFlag flag) const {}

CoordinateNode *CoordinateNode::GetPrevXPtr() const {}

void CoordinateNode::SetPrevXPtr(CoordinateNode *node_ptr) {}

CoordinateNode *CoordinateNode::GetNextXPtr() const {}

void CoordinateNode::SetNextXPtr(CoordinateNode *node_ptr) {}

CoordinateNode *CoordinateNode::GetPrevYPtr() const {}

void CoordinateNode::SetPrevYPtr(CoordinateNode *node_ptr) {}

CoordinateNode *CoordinateNode::GetNextYPtr() const {}

void CoordinateNode::SetNextYPtr(CoordinateNode *node_ptr) {}

CoordinateNode *CoordinateNode::GetPrevZPtr() const {}

void CoordinateNode::SetPrevZPtr(CoordinateNode *node_ptr) {}

CoordinateNode *CoordinateNode::GetNextZPtr() const {}

void CoordinateNode::SetNextZPtr(CoordinateNode *node_ptr) {}

CoordinateSystem *CoordinateNode::GetCoordinateSystemPtr() const {}

void CoordinateNode::SetCoordinateSystemPtr(
    CoordinateSystem *coordinate_system_ptr) {}

void CoordinateNode::OnNodePassX(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnNodePassY(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnNodePassZ(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnRemove() {}

void CoordinateNode::OnParentRemove(CoordinateNode *parent_node_ptr) {}

void CoordinateNode::Update() {}

std::string CoordinateNode::DebugStr() {}

void CoordinateNode::DebugX() {}

void CoordinateNode::DebugY() {}

void CoordinateNode::DebugZ() {}

const char *CoordinateNode::GetDescCStr() {}

void CoordinateNode::SetDescStr(std::string_view strv) {}

}  // namespace tpn
