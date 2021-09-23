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

#include "entity_coordinate_node.h"

#include <algorithm>

#include "entity.h"

namespace tpn {

EntityCoordinateNode::EntityCoordinateNode(Entity *entity_ptr)
    : CoordinateNode(nullptr), entity_ptr_(entity_ptr) {
  SetFlag(CoordinateNodeFlag::kCoordinateNodeFlagEntity);
  // TODO desc
}

EntityCoordinateNode::~EntityCoordinateNode() {
  entity_ptr_->OnCoordinateNodeDestory(this);
  watcher_nodes_.clear();
}

float EntityCoordinateNode::GetRealX() const {
  if (nullptr == entity_ptr_ ||
      HasFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoved |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoving)) {
    return std::numeric_limits<float>::lowest();
  }

  return entity_ptr_->GetPosition().x;
}

float EntityCoordinateNode::GetRealY() const {
  if (nullptr == entity_ptr_ /*||
      HasFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoved |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoving)*/) {
    return std::numeric_limits<float>::lowest();
  }

  return entity_ptr_->GetPosition().y;
}

float EntityCoordinateNode::GetRealZ() const {
  if (nullptr == entity_ptr_ /*||
      HasFlag(CoordinateNodeFlag::kCoordinateNodeFlagRemoved |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoving)*/) {
    return std::numeric_limits<float>::lowest();
  }

  return entity_ptr_->GetPosition().z;
}

void EntityCoordinateNode::OnRemove() {
  for (auto &watcher_node_ptr : watcher_nodes_) {
    if (nullptr == watcher_node_ptr) {
      continue;
    }

    CoordinateNode *node_ptr = watcher_node_ptr;
    watcher_node_ptr         = nullptr;
    ++delete_watcher_node_num_;

    node_ptr->OnParentRemove(this);
  }

  CoordinateNode::OnRemove();
}

void EntityCoordinateNode::Update() {
  SetOldRealPosition(GetX(), GetY(), GetZ());

  CoordinateNode::Update();

  AddFlag(CoordinateNodeFlag::kCoordinateNodeFlagUpdating);
  ++entity_node_updating_;

  for (auto i = 0; i < watcher_nodes_.size(); ++i) {
    CoordinateNode *node_ptr = watcher_nodes_[i];
    if (!node_ptr) {
      continue;
    }

    node_ptr->Update();
  }

  --entity_node_updating_;
  if (0 == entity_node_updating_) {
    RemoveFlag(CoordinateNodeFlag::kCoordinateNodeFlagUpdating);
  }

  ClearDelWatcherNodes();
}

Entity *EntityCoordinateNode::GetEntity() const { return entity_ptr_; }

void EntityCoordinateNode::SetEntity(Entity *entity_ptr) {
  entity_ptr_ = entity_ptr;
}

bool EntityCoordinateNode::AddWatcherNode(CoordinateNode *node_ptr) {
  ClearDelWatcherNodes();

  auto iter = std::find(watcher_nodes_.begin(), watcher_nodes_.end(), node_ptr);
  if (watcher_nodes_.end() != iter) {
    return false;
  }

  watcher_nodes_.emplace_back(node_ptr);

  OnAddWatcherNode(node_ptr);
  return true;
}

void EntityCoordinateNode::OnAddWatcherNode(CoordinateNode *node_ptr) {}

bool EntityCoordinateNode::DelWatcherNode(CoordinateNode *node_ptr) {
  auto iter = std::find(watcher_nodes_.begin(), watcher_nodes_.end(), node_ptr);
  if (watcher_nodes_.end() != iter) {
    return false;
  }

  if (HasFlag(CoordinateNodeFlag::kCoordinateNodeFlagUpdating |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoved |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoving)) {
    (*iter) = nullptr;
    ++delete_watcher_node_num_;
  } else {
    watcher_nodes_.erase(iter);
  }

  return true;
}

void EntityCoordinateNode::ClearDelWatcherNodes() {
  if (HasFlag(CoordinateNodeFlag::kCoordinateNodeFlagUpdating |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoved |
              CoordinateNodeFlag::kCoordinateNodeFlagRemoving)) {
    return;
  }

  if (delete_watcher_node_num_ > 0) {
    for (auto iter = watcher_nodes_.begin(); iter != watcher_nodes_.end();) {
      if (!(*iter)) {
        iter = watcher_nodes_.erase(iter);
        --delete_watcher_node_num_;

        if (delete_watcher_node_num_ <= 0) {
          return;
        }
      } else {
        ++iter;
      }
    }
  }
}

void EntityCoordinateNode::EntitiesInRange(
    std::vector<Entity *> &found_entities, CoordinateNode *root_node_ptr,
    const Position3D &origin_pos, float radius) {}

}  // namespace tpn
