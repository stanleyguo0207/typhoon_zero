
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

#include "aoi_node.h"

#include "debug_hub.h"
#include "aoi.h"
#include "fmt_wrap.h"
#include "log.h"

namespace tpn {

namespace aoi {

AOINode::AOINode(AOIMgr *aoi_mgr /* = nullptr */) : aoi_mgr_(aoi_mgr) {}

AOINode::~AOINode() {
  TPN_ASSERT(nullptr == prev_x_ptr_ && nullptr == next_x_ptr_ &&
                 nullptr == prev_y_ptr_ && nullptr == next_y_ptr_ &&
                 nullptr == prev_z_ptr_ && nullptr == next_z_ptr_ &&
                 nullptr == aoi_mgr_,
             "AOINode memory leak");
}

float AOINode::GetX() const { return x_; }

float AOINode::GetY() const { return y_; }

float AOINode::GetZ() const { return z_; }

void AOINode::SetX(float v) { x_ = v; }

void AOINode::SetY(float v) { y_ = v; }

void AOINode::SetZ(float v) { z_ = v; }

float AOINode::GetExtX() const { return 0.f; }

float AOINode::GetExtY() const { return 0.f; }

float AOINode::GetExtZ() const { return 0.f; }

float AOINode::GetOldExtX() const { return old_ext_x_; }

float AOINode::GetOldExtY() const { return old_ext_y_; }

float AOINode::GetOldExtZ() const { return old_ext_z_; }

void AOINode::SetOldExtX(float v) { old_ext_x_ = v; }

void AOINode::SetOldExtY(float v) { old_ext_y_ = v; }

void AOINode::SetOldExtZ(float v) { old_ext_z_ = v; }

void AOINode::ResetOldExtXYZ() {
  old_ext_x_ = GetExtX();
  old_ext_y_ = GetExtY();
  old_ext_z_ = GetExtZ();
}

uint32_t AOINode::GetFlags() const { return flags_.AsUnderlyingType(); }

void AOINode::SetFlag(AOINodeFlag flag) { flags_.SetFlag(flag); }

void AOINode::AddFlag(AOINodeFlag flag) { flags_.AddFlag(flag); }

void AOINode::RemoveFlag(AOINodeFlag flag) { flags_.RemoveFlag(flag); }

bool AOINode::HasFlag(AOINodeFlag flag) const { return flags_.HasFlag(flag); }

bool AOINode::HasAllFlag(AOINodeFlag flag) const {
  return flags_.HasAllFlag(flag);
}

AOINode *AOINode::GetPrevXPtr() const { return prev_x_ptr_; }

AOINode *AOINode::GetNextXPtr() const { return next_x_ptr_; }

AOINode *AOINode::GetPrevYPtr() const { return prev_y_ptr_; }

AOINode *AOINode::GetNextYPtr() const { return next_y_ptr_; }

AOINode *AOINode::GetPrevZPtr() const { return prev_z_ptr_; }

AOINode *AOINode::GetNextZPtr() const { return next_z_ptr_; }

void AOINode::SetPrevXPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    prev_x_ptr_ = node_ptr;
  }
}

void AOINode::SetNextXPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    next_x_ptr_ = node_ptr;
  }
}

void AOINode::SetPrevYPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    prev_y_ptr_ = node_ptr;
  }
}

void AOINode::SetNextYPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    next_y_ptr_ = node_ptr;
  }
}

void AOINode::SetPrevZPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    prev_z_ptr_ = node_ptr;
  }
}

void AOINode::SetNextZPtr(AOINode *node_ptr) {
  if (node_ptr != this) {
    next_z_ptr_ = node_ptr;
  }
}

void AOINode::SetAOIMgr(AOIMgr *aoi_mgr) { aoi_mgr_ = aoi_mgr; }

AOIMgr *AOINode::GetAOIMgr() const { return aoi_mgr_; }

void AOINode::OnNodePassX(AOINode *node_ptr, bool is_front) {}

void AOINode::OnNodePassY(AOINode *node_ptr, bool is_front) {}

void AOINode::OnNodePassZ(AOINode *node_ptr, bool is_front) {}

void AOINode::OnRemove() {
  SetOldExtX(x_);
  SetOldExtY(y_);
  SetOldExtZ(z_);

  x_ = std::numeric_limits<float>::lowest();
  // y_ = std::numeric_limits<float>::lowest();
  // z_ = std::numeric_limits<float>::lowest();
}

void AOINode::OnParentRemove(AOINode *parent_node_ptr) {}

void AOINode::Update() {
  if (aoi_mgr_) {
    aoi_mgr_->Update(this);
  }
}

std::string AOINode::DebugStr() {
  return fmt::format(
      "AOINode::DebugStr(): {:p} curr({}, {}, {}), {}, prev_x={:p} next_x={:p} "
      "prev_y={:p} next_y={:p} prev_z={:p} next_z={:p} flags={} desc={}\n",
      (void *)this, GetX(), GetY(), GetZ(),
      fmt::format("ext xyz({}, {}, {}), old_ext xyz({}, {}, {})", GetExtX(),
                  GetExtY(), GetExtZ(), GetOldExtX(), GetOldExtY(),
                  GetOldExtZ()),
      (void *)prev_x_ptr_, (void *)next_x_ptr_, (void *)prev_y_ptr_,
      (void *)next_y_ptr_, (void *)prev_z_ptr_, (void *)next_z_ptr_,
      flags_.AsUnderlyingType(), GetDescCStr());
}

void AOINode::DebugX() {
#if defined(TPN_AOIDEBUG)
  AOI_DEBUG("{}", DebugStr());
  if (next_x_ptr_) {
    this->next_x_ptr_->DebugX();
    if (this->next_x_ptr_->GetX() < GetX()) {
      AOI_ERROR("{:p} > {:p}", (void *)this, (void *)next_x_ptr_);
    }
  }
#endif
}

void AOINode::DebugY() {
#if defined(TPN_AOIDEBUG)
  AOI_DEBUG("{}", DebugStr());
  if (next_y_ptr_) {
    this->next_y_ptr_->DebugY();
    if (this->next_y_ptr_->GetY() < GetY()) {
      AOI_ERROR("{:p} > {:p}", (void *)this, (void *)next_y_ptr_);
    }
  }
#endif
}

void AOINode::DebugZ() {
#if defined(TPN_AOIDEBUG)
  AOI_DEBUG("{}", DebugStr());
  if (next_z_ptr_) {
    this->next_z_ptr_->DebugZ();
    if (this->next_z_ptr_->GetZ() < GetZ()) {
      AOI_ERROR("{:p} > {:p}", (void *)this, (void *)next_z_ptr_);
    }
  }
#endif
}

void AOINode::SetDescStr(std::string_view strv) {
#if defined(TPN_AOIDEBUG)
  desc_.assign(strv.data(), strv.size());
#endif
}

const char *AOINode::GetDescCStr() {
#if defined(TPN_AOIDEBUG)
  return desc_.c_str();
#else
  return "";
#endif
}

}  // namespace aoi

}  // namespace tpn
