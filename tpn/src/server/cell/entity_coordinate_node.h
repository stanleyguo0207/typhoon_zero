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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_COORDINATE_NODE_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_COORDINATE_NODE_H_

#include <vector>

#include "coordinate_node.h"
#include "cell_fwd.h"

namespace tpn {

/// 实体坐标系节点
class EntityCoordinateNode : public CoordinateNode {
 public:
  /// 构造函数
  ///  @param   entity_ptr    实体对象
  EntityCoordinateNode(Entity *entity_ptr);
  virtual ~EntityCoordinateNode();

  /// 获取节点实时坐标系当前x坐标
  ///  @return 节点实时坐标系当前x坐标
  float GetRealX() const override;
  /// 获取节点实时坐标系当前y坐标
  ///  @return 节点实时坐标系当前y坐标
  float GetRealY() const override;
  /// 获取节点实时坐标系当前z坐标
  ///  @return 节点实时坐标系当前z坐标
  float GetRealZ() const override;

  /// 节点移除
  void OnRemove() override;

  /// 当前节点有变化，需要更新它的list中的相关位置等信息
  void Update() override;

  /// 获取节点所属的实体
  ///  @return 节点所属的实体
  Entity *GetEntity() const;

  /// 设置节点所属的实体
  ///  @param[in]   entity_ptr    节点所属的实体
  void SetEntity(Entity *entity_ptr);

  /// 添加观察者节点
  ///  @param[in]   node_ptr      要添加的观察的节点
  ///  @return 添加成功返回true
  bool AddWatcherNode(CoordinateNode *node_ptr);

  void OnAddWatcherNode(CoordinateNode *node_ptr);

  /// 移除观察者节点
  ///  @param[in]   node_ptr      要移除的观察的节点
  ///  @return 移除成功返回true
  bool DelWatcherNode(CoordinateNode *node_ptr);

  /// 获取范围的实体列表
  ///  @param[out]  found_entities    范围的实体列表
  ///  @param[in]   root_node_ptr     根节点
  ///  @param[in]   origin_pos        原始位置
  ///  @param[in]   radius            半径
  static void EntitiesInRange(std::vector<Entity *> &found_entities,
                              CoordinateNode *root_node_ptr,
                              const Position3D &origin_pos, float radius);

 protected:
  /// 清理移除的节点
  void ClearDelWatcherNodes();

 protected:
  using WatcherNodesVec = std::vector<CoordinateNode *>;

  Entity *entity_ptr_{nullptr};     ///< 实体对象
  WatcherNodesVec watcher_nodes_;   ///< 观察节点
  int delete_watcher_node_num_{0};  ///< 移除中的观察节点数量
  int entity_node_updating_{0};     ///< 实体节点更新计数器
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_COORDINATE_NODE_H_
