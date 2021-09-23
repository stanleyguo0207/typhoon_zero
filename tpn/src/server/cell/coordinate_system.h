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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_SYSTEM_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_SYSTEM_H_

#include <list>

#include "g3d_wrap.h"
#include "cell_fwd.h"

namespace tpn {

/// 坐标系系统
class CoordinateSystem {
 public:
  CoordinateSystem();
  ~CoordinateSystem();

  /// 向坐标系系统中插入一个坐标节点
  ///  @param[out]  node_ptr    坐标节点
  ///  @return 插入成功返回true
  bool Insert(CoordinateNode *node_ptr);

  /// 从坐标系系统中移除一个坐标节点
  /// 放到待断链的列表中
  ///  @param[out]  node_ptr    坐标节点
  ///  @return 删除成功返回true
  bool Remove(CoordinateNode *node_ptr);

  /// 移除断链节点
  ///  @param[out]  node_ptr    坐标节点
  void RemoveImmediately(CoordinateNode *node_ptr);

  /// 移除所有断链节点
  void RemoveDelNodes();

  /// 更新坐标系中的坐标节点
  ///  @param[out]  node_ptr    要更新的节点
  void Update(CoordinateNode *node_ptr);

  /// 移动节点坐标轴X方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   x               x轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeX(CoordinateNode *node_ptr, float x,
                 CoordinateNode *curr_node_ptr);
  /// 移动节点坐标轴y方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   y               y轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeY(CoordinateNode *node_ptr, float y,
                 CoordinateNode *curr_node_ptr);
  /// 移动节点坐标轴z方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   z               z轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeZ(CoordinateNode *node_ptr, float z,
                 CoordinateNode *curr_node_ptr);

  /// 获取坐标系中x轴的第一个节点
  ///  @return 坐标系中x轴的第一个节点
  TPN_INLINE CoordinateNode *GetFirstXNodePtr() const;

  /// 获取坐标系中y轴的第一个节点
  ///  @return 坐标系中y轴的第一个节点
  TPN_INLINE CoordinateNode *GetFirstYNodePtr() const;

  /// 获取坐标系中z轴的第一个节点
  ///  @return 坐标系中z轴的第一个节点
  TPN_INLINE CoordinateNode *GetFirstZNodePtr() const;

  /// 坐标系是否为空
  /// @return 坐标系中的没有节点返回true
  TPN_INLINE bool IsEmpty() const;

  /// 获取坐标系中的坐标数量
  /// @return 坐标系中的坐标数量
  TPN_INLINE uint32_t GetSize() const;

 private:
  /// 从坐标系系统中移除一个坐标节点
  /// 真是的断链操作
  ///  @param[out]  node_ptr    坐标节点
  ///  @return 删除成功返回true
  bool RemoveReal(CoordinateNode *node_ptr);

 private:
  uint32_t size_{0};  ///< 坐标系系统中节点数量

  CoordinateNode *x_first_node_ptr_{nullptr};  ///< 坐标系x轴
  CoordinateNode *y_first_node_ptr_{nullptr};  ///< 坐标系y轴
  CoordinateNode *z_first_node_ptr_{nullptr};  ///< 坐标系z轴

  uint32_t dels_count_{0};            ///< 要移除的节点个数
  std::list<CoordinateNode *> dels_;  ///< 要移除的节点
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_SYSTEM_H_
