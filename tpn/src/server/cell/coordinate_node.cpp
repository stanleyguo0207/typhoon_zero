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
             "AOINode memory leak");
}

float CoordinateNode::GetX() const { return position_.x; }

void CoordinateNode::SetX(float v) { position_.x = v; }

float CoordinateNode::GetY() const { return position_.y; }

void CoordinateNode::SetY(float v) { position_.y = v; }

float CoordinateNode::GetZ() const { return position_.z; }

void CoordinateNode::SetZ(float v) { position_.z = v; }

Position3D &CoordinateNode::GetPosition() { return position_; }

void CoordinateNode::SetPosition(const Position3D &v) { position_ = v; }

float CoordinateNode::GetRealX() const { return 0.f; }

float CoordinateNode::GetRealY() const { return 0.f; }

float CoordinateNode::GetRealZ() const { return 0.f; }

float CoordinateNode::GetOldRealX() const { return old_real_position_.x; }

void CoordinateNode::SetOldRealX(float v) { old_real_position_.x = v; }

float CoordinateNode::GetOldRealY() const { return old_real_position_.y; }

void CoordinateNode::SetOldRealY(float v) { old_real_position_.y = v; }

float CoordinateNode::GetOldRealZ() const { return old_real_position_.z; }

void CoordinateNode::SetOldRealZ(float v) { old_real_position_.z = v; }

Position3D &CoordinateNode::GetOldRealPosition() { return old_real_position_; }

void CoordinateNode::SetOldRealPosition(const Position3D &v) {
  old_real_position_ = v;
}

void CoordinateNode::SetOldRealPosition(float x, float y, float z) {
  old_real_position_.x = x;
  old_real_position_.y = y;
  old_real_position_.z = z;
}

void CoordinateNode::ResetOldRealXYZ() {
  old_real_position_.x = GetOldRealX();
  old_real_position_.y = GetOldRealY();
  old_real_position_.z = GetOldRealZ();
}

uint32_t CoordinateNode::GetFlags() const { return flags_.AsUnderlyingType(); }

void CoordinateNode::SetFlag(CoordinateNodeFlag flag) { flags_.SetFlag(flag); }

void CoordinateNode::AddFlag(CoordinateNodeFlag flag) { flags_.AddFlag(flag); }

void CoordinateNode::RemoveFlag(CoordinateNodeFlag flag) {
  flags_.RemoveFlag(flag);
}

bool CoordinateNode::HasFlag(CoordinateNodeFlag flag) const {
  return flags_.HasFlag(flag);
}

bool CoordinateNode::HasAllFlag(CoordinateNodeFlag flag) const {
  return flags_.HasAllFlag(flag);
}

CoordinateNode *CoordinateNode::GetPrevXPtr() const { return prev_x_ptr_; }

void CoordinateNode::SetPrevXPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    prev_x_ptr_ = node_ptr;
  }
}

CoordinateNode *CoordinateNode::GetNextXPtr() const { return next_x_ptr_; }

void CoordinateNode::SetNextXPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    next_x_ptr_ = node_ptr;
  }
}

CoordinateNode *CoordinateNode::GetPrevYPtr() const { return prev_y_ptr_; }

void CoordinateNode::SetPrevYPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    prev_y_ptr_ = node_ptr;
  }
}

CoordinateNode *CoordinateNode::GetNextYPtr() const { return next_y_ptr_; }

void CoordinateNode::SetNextYPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    next_y_ptr_ = node_ptr;
  }
}

CoordinateNode *CoordinateNode::GetPrevZPtr() const { return prev_z_ptr_; }

void CoordinateNode::SetPrevZPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    prev_z_ptr_ = node_ptr;
  }
}

CoordinateNode *CoordinateNode::GetNextZPtr() const { return next_z_ptr_; }

void CoordinateNode::SetNextZPtr(CoordinateNode *node_ptr) {
  if (this != node_ptr) {
    next_z_ptr_ = node_ptr;
  }
}

CoordinateSystem *CoordinateNode::GetCoordinateSystemPtr() const {
  return coordinate_system_ptr_;
}

void CoordinateNode::SetCoordinateSystemPtr(
    CoordinateSystem *coordinate_system_ptr) {
  coordinate_system_ptr_ = coordinate_system_ptr;
}

void CoordinateNode::OnNodePassX(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnNodePassY(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnNodePassZ(CoordinateNode *node_ptr, bool is_front) {}

void CoordinateNode::OnRemove() { SetOldRealPosition(x_, y_, z_); }

void CoordinateNode::OnParentRemove(CoordinateNode *parent_node_ptr) {}

void CoordinateNode::Update() {
  if (coordinate_system_ptr_) {
    coordinate_system_ptr_->Update(this);
  }
}

std::string CoordinateNode::DebugStr() {
  return fmt::format(
      "CoordinateNode::DebugStr(): {:p} curr({}, {}, {}), {}, X(prev:{:p}, "
      "next:{:p}) Y(prev:{:p}, next:{:p}) Z(prev:{:p}, next:{:p}) flags:{} "
      "desc:{}\n",
      (void *)this, GetX(), GetY(), GetZ(),
      fmt::format("Real({}, {}, {}) OldReal({}, {}, {})\n", GetRealX(),
                  GetRealY(), GetRealZ(), GetOldRealX(), GetOldRealY(),
                  GetOldRealZ()),
      (void *)GetPrevXPtr(), (void *)GetNextXPtr(), (void *)GetPrevYPtr(),
      (void *)GetNextYPtr(), (void *)GetPrevZPtr(), (void *)GetNextZPtr(),
      GetFlags(), GetDescCStr());
}

void CoordinateNode::DebugX() {
#if defined(TPN_DEBUG)
  LOG_DEBUG("{}", DebugStr());

  if (next_x_ptr_) {
    this->next_x_ptr_->DebugX();

    if (this->next_x_ptr_->GetX() < GetX()) {
      LOG_ERROR("{:p} > {:p}", (void *)this, (void *)next_x_ptr_);
    }
  }
#endif
}

void CoordinateNode::DebugY() {
#if defined(TPN_DEBUG)
  LOG_DEBUG("{}", DebugStr());

  if (next_y_ptr_) {
    this->next_y_ptr_->DebugY();

    if (this->next_y_ptr_->GetY() < GetY()) {
      LOG_ERROR("{:p} > {:p}", (void *)this, (void *)next_y_ptr_);
    }
  }
#endif
}

void CoordinateNode::DebugZ() {
#if defined(TPN_DEBUG)
  LOG_DEBUG("{}", DebugStr());

  if (next_z_ptr_) {
    this->next_z_ptr_->DebugZ();

    if (this->next_z_ptr_->GetZ() < GetZ()) {
      LOG_ERROR("{:p} > {:p}", (void *)this, (void *)next_z_ptr_);
    }
  }
#endif
}

const char *CoordinateNode::GetDescCStr() {
#if defined(TPN_DEBUG)
  return desc_.c_str();
#else
  return "";
#endif
}

void CoordinateNode::SetDescStr(std::string_view strv) {
#if defined(TPN_DEBUG)
  desc_.assign(strv.data(), strv.size());
#endif
}

}  // namespace tpn
