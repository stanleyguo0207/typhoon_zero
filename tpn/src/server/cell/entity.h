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

  /// 获取实体的实时位置
  ///  @return 实体的实时位置
  TPN_INLINE Position3D &GetPosition();
  /// 设置实体的实时位置
  ///  @param[in]   pos     坐标向量
  TPN_INLINE void SetPosition(const Position3D &pos);

  /// 获取实体的实时方向
  ///  @return 实体的实时方向
  TPN_INLINE Direction3D &GetDirection();
  /// 设置实体的实时位置
  ///  @param[in]   dir     方向向量
  TPN_INLINE void SetDirection(const Direction3D &dir);

  /// 获取实体的坐标系节点
  ///  @return 实体的坐标系节点
  TPN_INLINE EntityCoordinateNode *GetEntityCoordinateNodePtr() const;

  /// 设置实体的实时位置和方向
  ///  @param[in]   pos     坐标向量
  ///  @param[in]   dir     方向向量
  void SetPositionAndDirection(const Position3D &pos, const Direction3D &dir);

  /// 监听位置变化
  void OnPositionChanged();
  /// 监听方向变化
  void OnDirectionChanged();

  /// 更新缓存位置
  void UpdateLastPosition();

  /// 坐标系中移除坐标几点
  void RemoveCoordinateNodeFromCoordinateSystem();
  /// 监听坐标节点析构
  void OnCoordinateNodeDestory(
      EntityCoordinateNode *entity_coordinate_node_ptr);

 protected:
  Position3D position_;       ///< 空间坐标向量
  Position3D last_position_;  ///< 上一个空间坐标向量
  Direction3D direction_;     ///< 空间方向向量
  EntityCoordinateNodeUptr entity_coordinate_node_uptr_;  ///< 坐标系entity节点
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_ENTITY_H_
