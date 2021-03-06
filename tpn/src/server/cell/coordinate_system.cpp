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

#include "coordinate_system.h"

#include <algorithm>

#include "debug_hub.h"
#include "coordinate_node.h"

namespace tpn {

CoordinateSystem::CoordinateSystem() {}

CoordinateSystem::~CoordinateSystem() {
  dels_.clear();
  dels_count_ = 0;

  if (x_first_node_ptr_) {
    CoordinateNode *node_ptr = x_first_node_ptr_;
    while (nullptr != node_ptr) {
      CoordinateNode *next_node_ptr = node_ptr->GetNextXPtr();

      if (next_node_ptr) {
        next_node_ptr->SetPrevXPtr(nullptr);
      }

      node_ptr->ResetXYZPtr();
      node_ptr->SetCoordinateSystemPtr(nullptr);

      // 这里只处理断链操作，析构放到节点实体的地方析构的时候处理
      //delete node_ptr;

      node_ptr = next_node_ptr;
    }

    x_first_node_ptr_ = nullptr;
    y_first_node_ptr_ = nullptr;
    z_first_node_ptr_ = nullptr;
  }

  RemoveDelNodes();
}

bool CoordinateSystem::Insert(CoordinateNode *node_ptr) {
  if (IsEmpty()) {
    x_first_node_ptr_ = node_ptr;
    y_first_node_ptr_ = node_ptr;
    z_first_node_ptr_ = node_ptr;

    node_ptr->ResetXYZPtr();
    node_ptr->SetCoordinateSystemPtr(this);

    node_ptr->SetX(node_ptr->GetRealX());
    node_ptr->SetY(node_ptr->GetRealY());
    node_ptr->SetZ(node_ptr->GetRealZ());
    node_ptr->ResetOldRealXYZ();

    size_ = 1;
  } else {  // 将节点放到首节点前，然后使用底层更新处理
    node_ptr->SetOldRealX(std::numeric_limits<float>::lowest());
    node_ptr->SetOldRealY(std::numeric_limits<float>::lowest());
    node_ptr->SetOldRealZ(std::numeric_limits<float>::lowest());

    // x轴放到第一个
    node_ptr->SetX(x_first_node_ptr_->GetX());
    x_first_node_ptr_->SetPrevXPtr(node_ptr);
    node_ptr->SetNextXPtr(x_first_node_ptr_);
    x_first_node_ptr_ = node_ptr;

    // y轴放到第一个
    node_ptr->SetY(y_first_node_ptr_->GetY());
    y_first_node_ptr_->SetPrevYPtr(node_ptr);
    node_ptr->SetNextYPtr(y_first_node_ptr_);
    y_first_node_ptr_ = node_ptr;

    // z轴放到第一个
    node_ptr->SetZ(z_first_node_ptr_->GetZ());
    z_first_node_ptr_->SetPrevZPtr(node_ptr);
    node_ptr->SetNextZPtr(z_first_node_ptr_);
    z_first_node_ptr_ = node_ptr;

    node_ptr->SetCoordinateSystemPtr(this);

    ++size_;

    Update(node_ptr);
  }

  return true;
}

bool CoordinateSystem::Remove(CoordinateNode *node_ptr) {
  node_ptr->AddFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoving);
  node_ptr->OnRemove();
  Update(node_ptr);
  node_ptr->AddFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoved);

  auto iter = std::find(dels_.begin(), dels_.end(), node_ptr);
  if (dels_.end() == iter) {
    dels_.emplace_back(node_ptr);
    ++dels_count_;
  }

  return true;
}

void CoordinateSystem::RemoveImmediately(CoordinateNode *node_ptr) {
  node_ptr->AddFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoving);
  node_ptr->OnRemove();
  Update(node_ptr);
  node_ptr->AddFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoved);
  RemoveReal(node_ptr);
}

void CoordinateSystem::RemoveDelNodes() {
  if (0 == dels_count_) {
    return;
  }

  for (auto &node_ptr : dels_) {
    RemoveReal(node_ptr);
  }

  dels_.clear();
  dels_count_ = 0;
}

void CoordinateSystem::Update(CoordinateNode *node_ptr) {
#if defined(TPN_DEBUG)
  LOG_DEBUG("Enter:[{:p}]: ({}, {}, {})", (void *)node_ptr,
            node_ptr->GetRealX(), node_ptr->GetRealY(), node_ptr->GetRealZ());
#endif

  // 先检查负方向 再检查正方向 再更正坐标点坐标
  if (node_ptr->GetRealX() != node_ptr->GetOldRealX()) {
    CoordinateNode *curr_node_ptr = node_ptr->GetPrevXPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetX() > node_ptr->GetRealX()) ||
            (curr_node_ptr->GetX() == node_ptr->GetRealX() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagNagativeBoundary)))) {
      MoveNodeX(node_ptr, node_ptr->GetRealX(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetPrevXPtr();
    }

    curr_node_ptr = node_ptr->GetNextXPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetX() < node_ptr->GetRealX()) ||
            (curr_node_ptr->GetX() == node_ptr->GetRealX() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagPositiveBoundary)))) {
      MoveNodeX(node_ptr, node_ptr->GetRealX(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetNextXPtr();
    }

    node_ptr->SetX(node_ptr->GetRealX());
  }

  if (node_ptr->GetRealY() != node_ptr->GetOldRealY()) {
    CoordinateNode *curr_node_ptr = node_ptr->GetPrevYPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetY() > node_ptr->GetRealY()) ||
            (curr_node_ptr->GetY() == node_ptr->GetRealY() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagNagativeBoundary)))) {
      MoveNodeY(node_ptr, node_ptr->GetRealY(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetPrevYPtr();
    }

    curr_node_ptr = node_ptr->GetNextYPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetY() < node_ptr->GetRealY()) ||
            (curr_node_ptr->GetY() == node_ptr->GetRealY() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagPositiveBoundary)))) {
      MoveNodeY(node_ptr, node_ptr->GetRealY(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetNextYPtr();
    }

    node_ptr->SetY(node_ptr->GetRealY());
  }

  if (node_ptr->GetRealZ() != node_ptr->GetOldRealZ()) {
    CoordinateNode *curr_node_ptr = node_ptr->GetPrevZPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetZ() > node_ptr->GetRealZ()) ||
            (curr_node_ptr->GetZ() == node_ptr->GetRealZ() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagNagativeBoundary)))) {
      MoveNodeZ(node_ptr, node_ptr->GetRealZ(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetPrevZPtr();
    }

    curr_node_ptr = node_ptr->GetNextZPtr();
    while (curr_node_ptr && curr_node_ptr != node_ptr &&
           ((curr_node_ptr->GetZ() < node_ptr->GetRealZ()) ||
            (curr_node_ptr->GetZ() == node_ptr->GetRealZ() &&
             !curr_node_ptr->HasFlag(
                 CoordinateNodeFlag::kCoordinateNodeFlagPositiveBoundary)))) {
      MoveNodeZ(node_ptr, node_ptr->GetRealZ(), curr_node_ptr);
      curr_node_ptr = node_ptr->GetNextZPtr();
    }

    node_ptr->SetZ(node_ptr->GetRealZ());
  }

  // 更新后 重置旧的xyz坐标
  node_ptr->ResetOldRealXYZ();

#if defined(TPN_DEBUG)
  LOG_DEBUG("Debug X:[{:p}]", (void *)node_ptr);
  x_first_node_ptr_->DebugX();
  LOG_DEBUG("Debug Y:[{:p}]", (void *)node_ptr);
  x_first_node_ptr_->DebugY();
  LOG_DEBUG("Debug Z:[{:p}]", (void *)node_ptr);
  x_first_node_ptr_->DebugZ();
#endif
}

void CoordinateSystem::MoveNodeX(CoordinateNode *node_ptr, float x,
                                 CoordinateNode *curr_node_ptr) {
  if (nullptr != node_ptr) {
    node_ptr->SetX(curr_node_ptr->GetX());

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update start: [{}X] ({}), curr=>({})",
              curr_node_ptr == node_ptr->GetPrevXPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif

    if (curr_node_ptr == node_ptr->GetPrevXPtr()) {  // -方向移动
      TPN_ASSERT(curr_node_ptr->GetX() >= x, "{} >= {} error",
                 curr_node_ptr->GetX(), x);

      CoordinateNode *prev_node_ptr = curr_node_ptr->GetPrevXPtr();
      curr_node_ptr->SetPrevXPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextXPtr(node_ptr);
        if (node_ptr == x_first_node_ptr_ && node_ptr->GetNextXPtr()) {
          x_first_node_ptr_ = node_ptr->GetNextXPtr();
        }
      } else {
        x_first_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevXPtr()) {
        node_ptr->GetPrevXPtr()->SetNextXPtr(node_ptr->GetNextXPtr());
      }

      if (node_ptr->GetNextXPtr()) {
        node_ptr->GetNextXPtr()->SetPrevXPtr(node_ptr->GetPrevXPtr());
      }

      node_ptr->SetPrevXPtr(prev_node_ptr);
      node_ptr->SetNextXPtr(curr_node_ptr);
    } else {  // +方向移动
      TPN_ASSERT(curr_node_ptr->GetX() <= x, "{} >= {} error",
                 curr_node_ptr->GetX(), x);

      CoordinateNode *next_node_ptr = curr_node_ptr->GetNextXPtr();
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

          if (node_ptr == x_first_node_ptr_) {
            x_first_node_ptr_ = node_ptr->GetNextXPtr();
          }
        }

        node_ptr->SetPrevXPtr(curr_node_ptr);
        node_ptr->SetNextXPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 1: [{}X] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevXPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      curr_node_ptr->OnNodePassX(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 2: [{}X] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevXPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      node_ptr->OnNodePassX(curr_node_ptr, true);
    }

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update end: [{}X] ({}), pass=>({})",
              curr_node_ptr == node_ptr->GetPrevXPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
  }
}

void CoordinateSystem::MoveNodeY(CoordinateNode *node_ptr, float y,
                                 CoordinateNode *curr_node_ptr) {
  if (nullptr != node_ptr) {
    node_ptr->SetY(curr_node_ptr->GetY());

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update start: [{}Y] ({}), curr=>({})",
              curr_node_ptr == node_ptr->GetPrevYPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif

    if (curr_node_ptr == node_ptr->GetPrevYPtr()) {  // -方向移动
      TPN_ASSERT(curr_node_ptr->GetY() >= y, "{} >= {} error",
                 curr_node_ptr->GetY(), y);

      CoordinateNode *prev_node_ptr = curr_node_ptr->GetPrevYPtr();
      curr_node_ptr->SetPrevYPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextYPtr(node_ptr);
        if (node_ptr == y_first_node_ptr_ && node_ptr->GetNextYPtr()) {
          y_first_node_ptr_ = node_ptr->GetNextYPtr();
        }
      } else {
        y_first_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevYPtr()) {
        node_ptr->GetPrevYPtr()->SetNextYPtr(node_ptr->GetNextYPtr());
      }

      if (node_ptr->GetNextYPtr()) {
        node_ptr->GetNextYPtr()->SetPrevYPtr(node_ptr->GetPrevYPtr());
      }

      node_ptr->SetPrevYPtr(prev_node_ptr);
      node_ptr->SetNextYPtr(curr_node_ptr);
    } else {  // +方向移动
      TPN_ASSERT(curr_node_ptr->GetY() <= y, "{} >= {} error",
                 curr_node_ptr->GetY(), y);

      CoordinateNode *next_node_ptr = curr_node_ptr->GetNextYPtr();
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

          if (node_ptr == y_first_node_ptr_) {
            y_first_node_ptr_ = node_ptr->GetNextYPtr();
          }
        }

        node_ptr->SetPrevYPtr(curr_node_ptr);
        node_ptr->SetNextYPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 1: [{}Y] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevYPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      curr_node_ptr->OnNodePassY(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 2: [{}Y] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevYPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      node_ptr->OnNodePassY(curr_node_ptr, true);
    }

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update end: [{}Y] ({}), pass=>({})",
              curr_node_ptr == node_ptr->GetPrevYPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
  }
}

void CoordinateSystem::MoveNodeZ(CoordinateNode *node_ptr, float z,
                                 CoordinateNode *curr_node_ptr) {
  if (nullptr != node_ptr) {
    node_ptr->SetZ(curr_node_ptr->GetZ());

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update start: [{}Z] ({}), curr=>({})",
              curr_node_ptr == node_ptr->GetPrevZPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif

    if (curr_node_ptr == node_ptr->GetPrevZPtr()) {  // -方向移动
      TPN_ASSERT(curr_node_ptr->GetZ() >= z, "{} >= {} error",
                 curr_node_ptr->GetZ(), z);

      CoordinateNode *prev_node_ptr = curr_node_ptr->GetPrevZPtr();
      curr_node_ptr->SetPrevZPtr(node_ptr);
      if (prev_node_ptr) {
        prev_node_ptr->SetNextZPtr(node_ptr);
        if (node_ptr == z_first_node_ptr_ && node_ptr->GetNextZPtr()) {
          z_first_node_ptr_ = node_ptr->GetNextZPtr();
        }
      } else {
        z_first_node_ptr_ = node_ptr;
      }

      if (node_ptr->GetPrevZPtr()) {
        node_ptr->GetPrevZPtr()->SetNextZPtr(node_ptr->GetNextZPtr());
      }

      if (node_ptr->GetNextZPtr()) {
        node_ptr->GetNextZPtr()->SetPrevZPtr(node_ptr->GetPrevZPtr());
      }

      node_ptr->SetPrevZPtr(prev_node_ptr);
      node_ptr->SetNextZPtr(curr_node_ptr);
    } else {  // +方向移动
      TPN_ASSERT(curr_node_ptr->GetZ() <= z, "{} >= {} error",
                 curr_node_ptr->GetZ(), z);

      CoordinateNode *next_node_ptr = curr_node_ptr->GetNextZPtr();
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

          if (node_ptr == z_first_node_ptr_) {
            z_first_node_ptr_ = node_ptr->GetNextZPtr();
          }
        }

        node_ptr->SetPrevZPtr(curr_node_ptr);
        node_ptr->SetNextZPtr(next_node_ptr);
      }
    }

    if (!node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 1: [{}Z] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevZPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      curr_node_ptr->OnNodePassZ(node_ptr, true);
    }

    if (!curr_node_ptr->HasFlag(
            CoordinateNodeFlag::kCoordinateNodeFlagHideOrRemoved)) {
#if defined(TPN_DEBUG)
      LOG_DEBUG("Update 2: [{}Z] ({}), pass=>({})",
                curr_node_ptr == node_ptr->GetPrevZPtr() ? "-" : "+",
                node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
      node_ptr->OnNodePassZ(curr_node_ptr, true);
    }

#if defined(TPN_DEBUG)
    LOG_DEBUG("Update end: [{}Z] ({}), pass=>({})",
              curr_node_ptr == node_ptr->GetPrevZPtr() ? "-" : "+",
              node_ptr->DebugStr(), curr_node_ptr->DebugStr());
#endif
  }
}

CoordinateNode *CoordinateSystem::GetFirstXNodePtr() const {
  return x_first_node_ptr_;
}

CoordinateNode *CoordinateSystem::GetFirstYNodePtr() const {
  return y_first_node_ptr_;
}

CoordinateNode *CoordinateSystem::GetFirstZNodePtr() const {
  return z_first_node_ptr_;
}

bool CoordinateSystem::IsEmpty() const {
  return nullptr == x_first_node_ptr_ && nullptr == y_first_node_ptr_ &&
         nullptr == z_first_node_ptr_;
}

uint32_t CoordinateSystem::GetSize() const { return size_; }

bool CoordinateSystem::RemoveReal(CoordinateNode *node_ptr) {
  if (nullptr == node_ptr->GetCoordinateSystemPtr()) {
    return true;
  }

  if (node_ptr == x_first_node_ptr_) {
    x_first_node_ptr_ = x_first_node_ptr_->GetNextXPtr();
    if (x_first_node_ptr_) {
      x_first_node_ptr_->SetPrevXPtr(nullptr);
    }
  } else {
    node_ptr->GetPrevXPtr()->SetNextXPtr(node_ptr->GetNextXPtr());

    if (node_ptr->GetNextXPtr()) {
      node_ptr->GetNextXPtr()->SetPrevXPtr(node_ptr->GetPrevXPtr());
    }
  }

  if (node_ptr == y_first_node_ptr_) {
    y_first_node_ptr_ = y_first_node_ptr_->GetNextYPtr();
    if (y_first_node_ptr_) {
      y_first_node_ptr_->SetPrevYPtr(nullptr);
    }
  } else {
    node_ptr->GetPrevYPtr()->SetNextYPtr(node_ptr->GetNextYPtr());

    if (node_ptr->GetNextYPtr()) {
      node_ptr->GetNextYPtr()->SetPrevYPtr(node_ptr->GetPrevYPtr());
    }
  }

  if (node_ptr == z_first_node_ptr_) {
    z_first_node_ptr_ = z_first_node_ptr_->GetNextZPtr();
    if (z_first_node_ptr_) {
      z_first_node_ptr_->SetPrevZPtr(nullptr);
    }
  } else {
    node_ptr->GetPrevZPtr()->SetNextZPtr(node_ptr->GetNextZPtr());

    if (node_ptr->GetNextZPtr()) {
      node_ptr->GetNextZPtr()->SetPrevZPtr(node_ptr->GetPrevZPtr());
    }
  }

  node_ptr->ResetXYZPtr();
  node_ptr->SetCoordinateSystemPtr(nullptr);

  --size_;

  return true;
}

}  // namespace tpn
