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

#include "aoi.h"

#include <limits>

#include "aoi_node.h"
#include "debug_hub.h"
#include "log.h"

namespace tpn {

namespace aoi {

AOIMgr::AOIMgr() {}

AOIMgr::~AOIMgr() {
  dels_count_ = 0;
  dels_.clear();

  if (first_x_node_ptr_) {
    AOINode *node_ptr = first_x_node_ptr_;
    while (nullptr != node_ptr) {
      AOINode *next_node_ptr = node_ptr->GetNextXPtr();
      if (next_node_ptr) {
        next_node_ptr->SetPrevXPtr(nullptr);
      }

      node_ptr->SetAOIMgr(nullptr);
      node_ptr->SetPrevXPtr(nullptr);
      node_ptr->SetNextXPtr(nullptr);
      node_ptr->SetPrevYPtr(nullptr);
      node_ptr->SetNextYPtr(nullptr);
      node_ptr->SetPrevZPtr(nullptr);
      node_ptr->SetNextZPtr(nullptr);

      delete node_ptr;

      node_ptr = next_node_ptr;
    }

    first_x_node_ptr_ = nullptr;
    first_y_node_ptr_ = nullptr;
    first_z_node_ptr_ = nullptr;
  }

  ReleaseNodes();
}

bool AOIMgr::Insert(AOINode *node_ptr) {
  if (IsEmpty()) {
    first_x_node_ptr_ = node_ptr;
    first_y_node_ptr_ = node_ptr;
    {  // TODO:2d优化
      first_z_node_ptr_ = node_ptr;
    }
    node_ptr->SetPrevXPtr(nullptr);
    node_ptr->SetNextXPtr(nullptr);
    node_ptr->SetPrevYPtr(nullptr);
    node_ptr->SetNextYPtr(nullptr);
    node_ptr->SetPrevZPtr(nullptr);
    node_ptr->SetNextZPtr(nullptr);

    node_ptr->SetX(node_ptr->GetExtX());
    node_ptr->SetY(node_ptr->GetExtY());
    node_ptr->SetZ(node_ptr->GetExtZ());
    node_ptr->SetAOIMgr(this);

    size_ = 1;

    node_ptr->ResetOldExtXYZ();
    return true;
  }

  node_ptr->SetOldExtX(std::numeric_limits<float>::lowest());
  node_ptr->SetOldExtY(std::numeric_limits<float>::lowest());
  node_ptr->SetOldExtZ(std::numeric_limits<float>::lowest());

  node_ptr->SetX(first_x_node_ptr_->GetX());
  first_x_node_ptr_->SetPrevXPtr(node_ptr);
  node_ptr->SetNextXPtr(first_x_node_ptr_);
  first_x_node_ptr_ = node_ptr;

  node_ptr->SetY(first_y_node_ptr_->GetY());
  first_y_node_ptr_->SetPrevYPtr(node_ptr);
  node_ptr->SetNextYPtr(first_y_node_ptr_);
  first_y_node_ptr_ = node_ptr;

  {  // TODO:2d优化
    node_ptr->SetZ(first_z_node_ptr_->GetZ());
    first_z_node_ptr_->SetPrevZPtr(node_ptr);
    node_ptr->SetNextZPtr(first_z_node_ptr_);
    first_z_node_ptr_ = node_ptr;
  }

  node_ptr->SetAOIMgr(this);
  ++size_;

  Update(node_ptr);
  return true;
}

bool AOIMgr::Remove(AOINode *node_ptr) {
  node_ptr->AddFlag(AOINodeFlag::kAOINodeFlagRemoving);
  node_ptr->OnRemove();
  Update(node_ptr);
  node_ptr->AddFlag(AOINodeFlag::kAOINodeFlagRemoved);

  auto iter = std::find(dels_.begin(), dels_.end(), node_ptr);
  if (dels_.end() == iter) {
    dels_.emplace_back(node_ptr);
    ++dels_count_;
  }

  return true;
}

bool AOIMgr::RemoveReal(AOINode *node_ptr) {
  if (nullptr == node_ptr->GetAOIMgr()) {
    return true;
  }

  if (node_ptr == first_x_node_ptr_) {
    first_x_node_ptr_ = first_x_node_ptr_->GetNextXPtr();
    if (first_x_node_ptr_) {
      first_x_node_ptr_->SetPrevXPtr(nullptr);
    }
  } else {
    node_ptr->GetPrevXPtr()->SetNextXPtr(node_ptr->GetNextXPtr());
    if (node_ptr->GetNextXPtr()) {
      node_ptr->GetNextXPtr()->SetPrevXPtr(node_ptr->GetPrevXPtr());
    }
  }

  if (node_ptr == first_y_node_ptr_) {
    first_y_node_ptr_ = first_y_node_ptr_->GetNextYPtr();
    if (first_y_node_ptr_) {
      first_y_node_ptr_->SetPrevYPtr(nullptr);
    }
  } else {
    node_ptr->GetPrevYPtr()->SetNextYPtr(node_ptr->GetNextYPtr());
    if (node_ptr->GetNextYPtr()) {
      node_ptr->GetNextYPtr()->SetPrevYPtr(node_ptr->GetPrevYPtr());
    }
  }

  {  // TODO:2d优化
    if (node_ptr == first_z_node_ptr_) {
      first_z_node_ptr_ = first_z_node_ptr_->GetNextZPtr();
      if (first_z_node_ptr_) {
        first_z_node_ptr_->SetPrevZPtr(nullptr);
      }
    } else {
      node_ptr->GetPrevZPtr()->SetNextZPtr(node_ptr->GetNextZPtr());
      if (node_ptr->GetNextZPtr()) {
        node_ptr->GetNextZPtr()->SetPrevZPtr(node_ptr->GetPrevZPtr());
      }
    }
  }

  node_ptr->SetPrevXPtr(nullptr);
  node_ptr->SetNextXPtr(nullptr);
  node_ptr->SetPrevYPtr(nullptr);
  node_ptr->SetNextYPtr(nullptr);
  node_ptr->SetPrevZPtr(nullptr);
  node_ptr->SetNextZPtr(nullptr);
  node_ptr->SetAOIMgr(nullptr);

  releases_.emplace_back(node_ptr);
  --size_;
  return true;
}

void AOIMgr::RemoveDelNodes() {
  if (0 == dels_count_) {
    return;
  }

  for (auto iter = dels_.begin(); iter != dels_.end(); ++iter) {
    RemoveReal((*iter));
  }

  dels_.clear();
  dels_count_ = 0;
}

void AOIMgr::ReleaseNodes() {
  RemoveDelNodes();

  for (auto iter = releases_.begin(); iter != releases_.end(); ++iter) {
    delete (*iter);
  }

  releases_.clear();
}

void AOIMgr::Update(AOINode *node_ptr) {
  AOI_DEBUG("Start Update:[{:p}]:  ({}, {}, {})", (void *)node_ptr,
            node_ptr->GetExtX(), node_ptr->GetExtY(), node_ptr->GetExtZ());

  if (node_ptr->GetExtX() != node_ptr->GetOldExtX()) {
    // 大于向负方向移动
    AOINode *curr_node_ptr = node_ptr->GetPrevXPtr();
    while (curr_node_ptr && node_ptr != curr_node_ptr &&
           ((curr_node_ptr->GetX() > node_ptr->GetExtX()) ||
            (curr_node_ptr->GetX() == node_ptr->GetExtX() &&
             !curr_node_ptr->HasFlag(
                 AOINodeFlag::kAOINodeFlagNagativeBoundary)))) {
      MoveNodeX(node_ptr, node_ptr->GetExtX(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetPrevXPtr();
    }

    // 小于向正方向移动
    curr_node_ptr = node_ptr->GetNextXPtr();
    while (curr_node_ptr && node_ptr != curr_node_ptr &&
           ((curr_node_ptr->GetX() < node_ptr->GetExtX()) ||
            (curr_node_ptr->GetX() == node_ptr->GetExtX() &&
             !curr_node_ptr->HasFlag(
                 AOINodeFlag::kAOINodeFlagPositiveBoundary)))) {
      MoveNodeX(node_ptr, node_ptr->GetExtX(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetNextXPtr();
    }

    node_ptr->SetX(node_ptr->GetExtX());
  }

  if (node_ptr->GetExtY() != node_ptr->GetOldExtY()) {
    // 大于向负方向移动
    AOINode *curr_node_ptr = node_ptr->GetPrevYPtr();
    while (curr_node_ptr && node_ptr != curr_node_ptr &&
           ((curr_node_ptr->GetY() > node_ptr->GetExtY()) ||
            (curr_node_ptr->GetY() == node_ptr->GetExtY() &&
             !curr_node_ptr->HasFlag(
                 AOINodeFlag::kAOINodeFlagNagativeBoundary)))) {
      MoveNodeY(node_ptr, node_ptr->GetExtY(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetPrevYPtr();
    }

    // 小于向正方向移动
    curr_node_ptr = node_ptr->GetNextYPtr();
    while (curr_node_ptr && node_ptr != curr_node_ptr &&
           ((curr_node_ptr->GetY() < node_ptr->GetExtY()) ||
            (curr_node_ptr->GetY() == node_ptr->GetExtY() &&
             !curr_node_ptr->HasFlag(
                 AOINodeFlag::kAOINodeFlagPositiveBoundary)))) {
      MoveNodeY(node_ptr, node_ptr->GetExtY(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetNextYPtr();
    }

    node_ptr->SetY(node_ptr->GetExtY());
  }

  {  // TODO:2d优化
    if (node_ptr->GetExtZ() != node_ptr->GetOldExtZ()) {
      // 大于向负方向移动
      AOINode *curr_node_ptr = node_ptr->GetPrevZPtr();
      while (curr_node_ptr && node_ptr != curr_node_ptr &&
             ((curr_node_ptr->GetZ() > node_ptr->GetExtZ()) ||
              (curr_node_ptr->GetZ() == node_ptr->GetExtZ() &&
               !curr_node_ptr->HasFlag(
                   AOINodeFlag::kAOINodeFlagNagativeBoundary)))) {
        MoveNodeZ(node_ptr, node_ptr->GetExtZ(), curr_node_ptr);
        curr_node_ptr = node_ptr->GetPrevZPtr();
      }

      // 小于向正方向移动
      curr_node_ptr = node_ptr->GetNextZPtr();
      while (curr_node_ptr && node_ptr != curr_node_ptr &&
             ((curr_node_ptr->GetZ() < node_ptr->GetExtZ()) ||
              (curr_node_ptr->GetZ() == node_ptr->GetExtZ() &&
               !curr_node_ptr->HasFlag(
                   AOINodeFlag::kAOINodeFlagPositiveBoundary)))) {
        MoveNodeZ(node_ptr, node_ptr->GetExtZ(), curr_node_ptr);
        curr_node_ptr = node_ptr->GetNextZPtr();
      }

      node_ptr->SetZ(node_ptr->GetExtZ());
    }
  }

  node_ptr->ResetOldExtXYZ();

  AOI_DEBUG("Start DebugX:[{:p}]", (void *)node_ptr);
  first_x_node_ptr_->DebugX();
  AOI_DEBUG("Start DebugY:[{:p}]", (void *)node_ptr);
  first_x_node_ptr_->DebugX();
  AOI_DEBUG("Start DebugZ:[{:p}]", (void *)node_ptr);
  first_x_node_ptr_->DebugX();
}

void AOIMgr::MoveNodeX(AOINode *node_ptr, float x, AOINode *curr_node_ptr) {
  if (nullptr != curr_node_ptr) {
    node_ptr->SetX(curr_node_ptr->GetX());

    AOI_DEBUG("Move Start: [{}X] ({}), currnode=>({})",
              node_ptr->GetPrevXPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

    if (node_ptr->GetPrevXPtr() == curr_node_ptr) {
      TPN_ASSERT(curr_node_ptr->GetX() >= x, "move x error, {}", x);

      AOINode *prev_node_ptr = curr_node_ptr->GetPrevXPtr();
      curr_node_ptr->SetPrevXPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextXPtr(node_ptr);
        if (node_ptr == first_x_node_ptr_ && node_ptr->GetNextXPtr()) {
          first_x_node_ptr_ = node_ptr->GetNextXPtr();
        }
      } else {
        first_x_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevXPtr()) {
        node_ptr->GetPrevXPtr()->SetNextXPtr(node_ptr->GetNextXPtr());
      }

      if (node_ptr->GetNextXPtr()) {
        node_ptr->GetNextXPtr()->SetPrevXPtr(node_ptr->GetPrevXPtr());
      }

      node_ptr->SetPrevXPtr(prev_node_ptr);
      node_ptr->SetNextXPtr(curr_node_ptr);
    } else {
      TPN_ASSERT(curr_node_ptr->GetX() <= x, "move x error, {}", x);

      AOINode *next_node_ptr = curr_node_ptr->GetNextXPtr();
      if (next_node_ptr != node_ptr) {
        curr_node_ptr->SetNextXPtr(node_ptr);
        if (next_node_ptr) {
          next_node_ptr->SetPrevXPtr(node_ptr);
        }

        if (node_ptr->GetPrevXPtr()) {
          node_ptr->GetPrevXPtr()->SetNextXPtr(node_ptr->GetNextXPtr());
        }

        if (node_ptr->GetNextXPtr()) {
          node_ptr->GetNextXPtr()->SetPrevXPtr(node_ptr->GetPrevXPtr());
          if (node_ptr == first_x_node_ptr_) {
            first_x_node_ptr_ = node_ptr->GetNextXPtr();
          }
        }

        node_ptr->SetPrevXPtr(curr_node_ptr);
        node_ptr->SetNextXPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass1: [{}X] ({}), passnode=>({})",
                node_ptr->GetPrevXPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      curr_node_ptr->OnNodePassX(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass2: [{}X] ({}), passnode=>({})",
                node_ptr->GetPrevXPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      node_ptr->OnNodePassX(curr_node_ptr, false);
    }

    AOI_DEBUG("Move end: [{}X] ({}), currnode=>({})",
              node_ptr->GetPrevXPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());
  }
}

void AOIMgr::MoveNodeY(AOINode *node_ptr, float y, AOINode *curr_node_ptr) {
  if (nullptr != curr_node_ptr) {
    node_ptr->SetY(curr_node_ptr->GetY());

    AOI_DEBUG("Move Start: [{}Y] ({}), currnode=>({})",
              node_ptr->GetPrevYPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

    if (node_ptr->GetPrevYPtr() == curr_node_ptr) {
      TPN_ASSERT(curr_node_ptr->GetY() >= y, "move y error, {}", y);

      AOINode *prev_node_ptr = curr_node_ptr->GetPrevYPtr();
      curr_node_ptr->SetPrevYPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextYPtr(node_ptr);
        if (node_ptr == first_y_node_ptr_ && node_ptr->GetNextYPtr()) {
          first_y_node_ptr_ = node_ptr->GetNextYPtr();
        }
      } else {
        first_y_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevYPtr()) {
        node_ptr->GetPrevYPtr()->SetNextYPtr(node_ptr->GetNextYPtr());
      }

      if (node_ptr->GetNextYPtr()) {
        node_ptr->GetNextYPtr()->SetPrevYPtr(node_ptr->GetPrevYPtr());
      }

      node_ptr->SetPrevYPtr(prev_node_ptr);
      node_ptr->SetNextYPtr(curr_node_ptr);
    } else {
      TPN_ASSERT(curr_node_ptr->GetY() <= y, "move y error, {}", y);

      AOINode *next_node_ptr = curr_node_ptr->GetNextYPtr();
      if (next_node_ptr != node_ptr) {
        curr_node_ptr->SetNextYPtr(node_ptr);
        if (next_node_ptr) {
          next_node_ptr->SetPrevYPtr(node_ptr);
        }

        if (node_ptr->GetPrevYPtr()) {
          node_ptr->GetPrevYPtr()->SetNextYPtr(node_ptr->GetNextYPtr());
        }

        if (node_ptr->GetNextYPtr()) {
          node_ptr->GetNextYPtr()->SetPrevYPtr(node_ptr->GetPrevYPtr());
          if (node_ptr == first_y_node_ptr_) {
            first_y_node_ptr_ = node_ptr->GetNextYPtr();
          }
        }

        node_ptr->SetPrevYPtr(curr_node_ptr);
        node_ptr->SetNextYPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass1: [{}Y] ({}), passnode=>({})",
                node_ptr->GetPrevYPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      curr_node_ptr->OnNodePassY(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass2: [{}Y] ({}), passnode=>({})",
                node_ptr->GetPrevYPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      node_ptr->OnNodePassY(curr_node_ptr, false);
    }

    AOI_DEBUG("Move end: [{}Y] ({}), currnode=>({})",
              node_ptr->GetPrevYPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());
  }
}

void AOIMgr::MoveNodeZ(AOINode *node_ptr, float z, AOINode *curr_node_ptr) {
  if (nullptr != curr_node_ptr) {
    node_ptr->SetZ(curr_node_ptr->GetZ());

    AOI_DEBUG("Move Start: [{}Z] ({}), currnode=>({})",
              node_ptr->GetPrevZPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

    if (node_ptr->GetPrevZPtr() == curr_node_ptr) {
      TPN_ASSERT(curr_node_ptr->GetZ() >= z, "move z error, {}", z);

      AOINode *prev_node_ptr = curr_node_ptr->GetPrevZPtr();
      curr_node_ptr->SetPrevZPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextZPtr(node_ptr);
        if (node_ptr == first_z_node_ptr_ && node_ptr->GetNextZPtr()) {
          first_z_node_ptr_ = node_ptr->GetNextZPtr();
        }
      } else {
        first_z_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevZPtr()) {
        node_ptr->GetPrevZPtr()->SetNextZPtr(node_ptr->GetNextZPtr());
      }

      if (node_ptr->GetNextZPtr()) {
        node_ptr->GetNextZPtr()->SetPrevZPtr(node_ptr->GetPrevZPtr());
      }

      node_ptr->SetPrevZPtr(prev_node_ptr);
      node_ptr->SetNextZPtr(curr_node_ptr);
    } else {
      TPN_ASSERT(curr_node_ptr->GetZ() <= z, "move z error, {}", z);

      AOINode *next_node_ptr = curr_node_ptr->GetNextZPtr();
      if (next_node_ptr != node_ptr) {
        curr_node_ptr->SetNextZPtr(node_ptr);
        if (next_node_ptr) {
          next_node_ptr->SetPrevZPtr(node_ptr);
        }

        if (node_ptr->GetPrevZPtr()) {
          node_ptr->GetPrevZPtr()->SetNextZPtr(node_ptr->GetNextZPtr());
        }

        if (node_ptr->GetNextZPtr()) {
          node_ptr->GetNextZPtr()->SetPrevZPtr(node_ptr->GetPrevZPtr());
          if (node_ptr == first_z_node_ptr_) {
            first_z_node_ptr_ = node_ptr->GetNextZPtr();
          }
        }

        node_ptr->SetPrevZPtr(curr_node_ptr);
        node_ptr->SetNextZPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass1: [{}Z] ({}), passnode=>({})",
                node_ptr->GetPrevZPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      curr_node_ptr->OnNodePassZ(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(AOINodeFlag::kAOINodeFlagHideOrRemoved)) {
      AOI_DEBUG("Move pass2: [{}Z] ({}), passnode=>({})",
                node_ptr->GetPrevZPtr() == curr_node_ptr ? "-" : "+",
                node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());

      node_ptr->OnNodePassZ(curr_node_ptr, false);
    }

    AOI_DEBUG("Move end: [{}Z] ({}), currnode=>({})",
              node_ptr->GetPrevZPtr() == curr_node_ptr ? "-" : "+",
              node_ptr->GetDescCStr(), curr_node_ptr->GetDescCStr());
  }
}

AOINode *AOIMgr::GetFirstXNodePtr() const { return first_x_node_ptr_; }

AOINode *AOIMgr::GetFirstYNodePtr() const { return first_y_node_ptr_; }

AOINode *AOIMgr::GetFirstZNodePtr() const { return first_z_node_ptr_; }

bool AOIMgr::IsEmpty() const {
  return nullptr == first_x_node_ptr_ && nullptr == first_y_node_ptr_ &&
         nullptr == first_z_node_ptr_;
}

size_t AOIMgr::GetSize() const { return size_; }

}  // namespace aoi

}  // namespace tpn
