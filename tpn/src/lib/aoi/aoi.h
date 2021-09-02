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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_H_

#include <list>

#include "aoi_fwd.h"

namespace tpn {

namespace aoi {

/// 视野节点管理器
/// 底层基于xyz轴方向的十字链表组成
class AOIMgr {
 public:
  /// 构造函数
  AOIMgr();

  /// 析构函数
  ~AOIMgr();

  /// 将视野节点插入到视野管理器中
  ///  @param[in]   node_ptr    视野节点
  ///  @return 插入成功返回true
  bool Insert(AOINode *node_ptr);

  /// 将视野节点从管理器中移除
  ///  @param[in]   node_ptr    视野节点
  ///  @return 移除成功返回true
  bool Remove(AOINode *node_ptr);

  /// 将视野节点从管理器中移除
  ///  @param[in]   node_ptr    视野节点
  ///  @return 移除成功返回true
  bool RemoveReal(AOINode *node_ptr);

  void RemoveDelNodes();
  void ReleaseNodes();

  /// 当某个节点有变动时，需要更新它所在的list中的相关位置等信息
  ///  @param[in]   node_ptr    变动节点
  void Update(AOINode *node_ptr);

  /// 移动节点坐标轴X方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   x               x轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeX(AOINode *node_ptr, float x, AOINode *curr_node_ptr);
  /// 移动节点坐标轴y方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   y               y轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeY(AOINode *node_ptr, float y, AOINode *curr_node_ptr);
  /// 移动节点坐标轴z方向
  ///  @param[in]   node_ptr        目标节点
  ///  @param[in]   z               z轴坐标
  ///  @param[in]   curr_node_ptr   当前节点
  void MoveNodeZ(AOINode *node_ptr, float z, AOINode *curr_node_ptr);

  /// 获取管理器中的坐标系x轴第一个节点
  ///  @return 管理器中的坐标系x轴第一个节点
  TPN_INLINE AOINode *GetFirstXNodePtr() const;
  /// 获取管理器中的坐标系y轴第一个节点
  ///  @return 管理器中的坐标系y轴第一个节点
  TPN_INLINE AOINode *GetFirstYNodePtr() const;
  /// 获取管理器中的坐标系z轴第一个节点
  ///  @return 管理器中的坐标系z轴第一个节点
  TPN_INLINE AOINode *GetFirstZNodePtr() const;

  /// 判断管理器是否为空
  ///  @return 为空返回true
  TPN_INLINE bool IsEmpty() const;

  /// 获取管理器管理的节点数量
  ///  @return 管理器管理的节点数量
  TPN_INLINE size_t GetSize() const;

 private:
  size_t size_{0};  ///< 管理器中的视野节点数量

  AOINode *first_x_node_ptr_{nullptr};  ///< 坐标系x轴的首尾节点
  AOINode *first_y_node_ptr_{nullptr};  ///< 坐标系y轴的首尾节点
  AOINode *first_z_node_ptr_{nullptr};  ///< 坐标系y轴的首尾节点

  size_t dels_count_{0};           ///< 移除的节点数量 stl_list优化
  std::list<AOINode *> dels_;      ///< 移除列表
  std::list<AOINode *> releases_;  ///< 释放列表
};

}  // namespace aoi

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_H_
