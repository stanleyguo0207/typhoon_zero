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

#ifndef TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_NODE_H_
#define TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_NODE_H_

#include <limits>
#include <string>
#include <string_view>

#include "g3d_wrap.h"
#include "cell_fwd.h"
#include "enum_flag.h"

namespace tpn {

/// 坐标系节点标志
enum class CoordinateNodeFlag : uint16_t {
  kCoordinateNodeFlagNone    = 0x0,         ///< 无状态
  kCoordinateNodeFlagEntity  = 0x1,         ///< 实体节点
  kCoordinateNodeFlagTrigger = (0x1 << 1),  ///< 触发器节点
  kCoordinateNodeFlagHide = (0x1 << 2),  ///< 节点隐藏(其他节点不可见)
  kCoordinateNodeFlagRemoving = (0x1 << 3),  ///< 删除中的节点
  kCoordinateNodeFlagRemoved  = (0x1 << 4),  ///< 删除的节点
  kCoordinateNodeFlagPending  = (0x1 << 5),  ///< 处于update操作中的节点
  kCoordinateNodeFlagUpdating = (0x1 << 6),  ///< 正在update操作中的节点
  kCoordinateNodeFlagInstalling       = (0x1 << 7),  ///< 安装中的节点
  kCoordinateNodeFlagPositiveBoundary = (0x1 << 8),  ///< 处于正边界的节点
  kCoordinateNodeFlagNagativeBoundary = (0x1 << 9),  ///< 处于负边界的节点
  kCoordinateNodeFlagHideOrRemoved    = kCoordinateNodeFlagHide | kCoordinateNodeFlagRemoved,
};

/// 坐标系节点
/// 十字链表实现
/// xyz3个方向个一条链表
class CoordinateNode {
 public:
  /// 构造函数
  ///  @param[in]   coordinate_system_ptr   坐标系系统
  CoordinateNode(CoordinateSystem *coordinate_system_ptr = nullptr);
  virtual ~CoordinateNode();

  /// 获取节点本身坐标系x坐标
  ///  @return 节点本身坐标系x坐标
  virtual float GetX() const;
  /// 设置节点本身坐标系x坐标
  ///  @param[in]   v     坐标值
  virtual void SetX(float v);

  /// 获取节点本身坐标系y坐标
  ///  @return 节点本身坐标系y坐标
  virtual float GetY() const;
  /// 设置节点本身坐标系y坐标
  ///  @param[in]   v     坐标值
  virtual void SetY(float v);

  /// 获取节点本身坐标系z坐标
  ///  @return 节点本身坐标系z坐标
  virtual float GetZ() const;
  /// 设置节点本身坐标系z坐标
  ///  @param[in]   v     坐标值
  virtual void SetZ(float v);

  /// 获取节点本身坐标系向量
  ///  @return 节点本身坐标系向量
  virtual Position3D GetPosition() const;
  /// 设置节点本身坐标系向量
  ///  @param[in]   v     坐标系向量
  virtual void SetPosition(const Position3D &v);

  /// 获取节点实时坐标系当前x坐标
  ///  @return 节点实时坐标系当前x坐标
  virtual float GetRealX() const;
  /// 获取节点实时坐标系当前y坐标
  ///  @return 节点实时坐标系当前y坐标
  virtual float GetRealY() const;
  /// 获取节点实时坐标系当前z坐标
  ///  @return 节点实时坐标系当前z坐标
  virtual float GetRealZ() const;
  /// 获取节点实时坐标系向量
  ///  @return 节点实时坐标系向量
  virtual Position3D GetRealPosition() const;

  /// 获取节点实时坐标系缓存x坐标
  ///  @return 节点实时坐标系缓存x坐标
  virtual float GetOldRealX() const;
  /// 设置节点实时坐标系缓存x坐标
  ///  @param[in]   v     节点实时坐标系缓存x坐标
  virtual void SetOldRealX(float v);

  /// 获取节点实时坐标系缓存y坐标
  ///  @return 节点实时坐标系缓存y坐标
  virtual float GetOldRealY() const;
  /// 设置节点实时坐标系缓存y坐标
  ///  @param[in]   v     节点实时坐标系缓存y坐标
  virtual void SetOldRealY(float v);

  /// 获取节点实时坐标系缓存z坐标
  ///  @return 节点实时坐标系缓存z坐标
  virtual float GetOldRealZ() const;
  /// 设置节点实时坐标系缓存z坐标
  ///  @param[in]   v     节点实时坐标系缓存z坐标
  virtual void SetOldRealZ(float v);

  /// 获取节点缓存实时坐标系向量
  ///  @return 节点缓存实时坐标系向量
  virtual Position3D GetOldRealPosition() const;
  /// 设置节点本身坐标系向量
  ///  @param[in]   v     坐标系向量
  virtual void SetOldRealPosition(const Position3D &v);

  /// 重置缓存的实时坐标系坐标
  virtual void ResetOldRealXYZ();

  /// 获取所有标志
  ///  @return 节点当前的所有标志
  TPN_INLINE uint32_t GetFlags() const;
  /// 设置单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void SetFlag(CoordinateNodeFlag flag);
  /// 添加单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void AddFlag(CoordinateNodeFlag flag);
  /// 移除单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void RemoveFlag(CoordinateNodeFlag flag);
  /// 是否含有指定的单个标志
  ///  @param[in]   flag      要检查的单个标志
  ///  @return 含有传入的标志返回true
  TPN_INLINE bool HasFlag(CoordinateNodeFlag flag) const;
  /// 是否含有指定的多个标志
  ///  @param[in]   flag      要检查的多个标志
  ///  @return 含有所有传入的标志返回true
  TPN_INLINE bool HasAllFlag(CoordinateNodeFlag flag) const;

  /// 获取坐标系x轴方向前置节点
  ///  @return 坐标系x轴方向前置节点
  TPN_INLINE CoordinateNode *GetPrevXPtr() const;
  /// 获取坐标系x轴方向前置节点
  ///  @param[in]   node_ptr    坐标系x轴方向前置节点
  TPN_INLINE void SetPrevXPtr(CoordinateNode *node_ptr);

  /// 获取坐标系x轴方向后置节点
  ///  @return 坐标系x轴方向后置节点
  TPN_INLINE CoordinateNode *GetNextXPtr() const;
  /// 获取坐标系x轴方向后置节点
  ///  @param[in]   node_ptr    坐标系x轴方向前置节点
  TPN_INLINE void SetNextXPtr(CoordinateNode *node_ptr);

  /// 获取坐标系y轴方向前置节点
  ///  @return 坐标系y轴方向前置节点
  TPN_INLINE CoordinateNode *GetPrevYPtr() const;
  /// 获取坐标系y轴方向前置节点
  ///  @param[in]   node_ptr    坐标系y轴方向前置节点
  TPN_INLINE void SetPrevYPtr(CoordinateNode *node_ptr);

  /// 获取坐标系y轴方向后置节点
  ///  @return 坐标系y轴方向后置节点
  TPN_INLINE CoordinateNode *GetNextYPtr() const;
  /// 获取坐标系y轴方向后置节点
  ///  @param[in]   node_ptr    坐标系y轴方向前置节点
  TPN_INLINE void SetNextYPtr(CoordinateNode *node_ptr);

  /// 获取坐标系z轴方向前置节点
  ///  @return 坐标系z轴方向前置节点
  TPN_INLINE CoordinateNode *GetPrevZPtr() const;
  /// 获取坐标系z轴方向前置节点
  ///  @param[in]   node_ptr    坐标系z轴方向前置节点
  TPN_INLINE void SetPrevZPtr(CoordinateNode *node_ptr);

  /// 获取坐标系z轴方向后置节点
  ///  @return 坐标系z轴方向后置节点
  TPN_INLINE CoordinateNode *GetNextZPtr() const;
  /// 获取坐标系z轴方向后置节点
  ///  @param[in]   node_ptr    坐标系z轴方向前置节点
  TPN_INLINE void SetNextZPtr(CoordinateNode *node_ptr);

  /// 获取坐标系节点所在的坐标系系统
  ///  @return 坐标系节点所在的坐标系系统
  TPN_INLINE CoordinateSystem *GetCoordinateSystemPtr() const;
  /// 设置坐标系节点所在的坐标系系统
  ///  @param[in]   coordinate_system_ptr   坐标系系统
  TPN_INLINE void SetCoordinateSystemPtr(
      CoordinateSystem *coordinate_system_ptr);

  /// 某个节点x坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassX(CoordinateNode *node_ptr, bool is_front);
  /// 某个节点y坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassY(CoordinateNode *node_ptr, bool is_front);
  /// 某个节点z坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassZ(CoordinateNode *node_ptr, bool is_front);

  /// 节点移除
  virtual void OnRemove();
  /// 父节点移除
  ///  @param[in]   parent_node_ptr     父节点移除
  virtual void OnParentRemove(CoordinateNode *parent_node_ptr);

  /// 当前节点有变化，需要更新它的list中的相关位置等信息
  virtual void Update();

  /// 生成调试用描述
  ///  @return 调试用描述
  std::string DebugStr();

  /// 输出调试X轴
  void DebugX();
  /// 输出调试Y轴
  void DebugY();
  /// 输出调试Z轴
  void DebugZ();

  /// 获取调试描述符
  ///  @return 调试描述符
  virtual const char *GetDescCStr();
  /// 设置调试描述符
  ///  @param[in]   strv    调试描述符
  void SetDescStr(std::string_view strv);

 protected:
  Position3D position_{
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest()};  ///< 节点本身坐标系向量

  Position3D old_real_position_{
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest(),
      std::numeric_limits<float>::lowest()};  ///< 节点缓存实时坐标系向量

  EnumFlag<CoordinateNodeFlag> flags_{
      CoordinateNodeFlag::kCoordinateNodeFlagNone};  ///< 状态集合

  CoordinateNode *prev_x_ptr_{nullptr};  ///< 坐标系x轴方向前置节点
  CoordinateNode *next_x_ptr_{nullptr};  ///< 坐标系x轴方向后置节点

  CoordinateNode *prev_y_ptr_{nullptr};  ///< 坐标系y轴方向前置节点
  CoordinateNode *next_y_ptr_{nullptr};  ///< 坐标系y轴方向后置节点

  CoordinateNode *prev_z_ptr_{nullptr};  ///< 坐标系z轴方向前置节点
  CoordinateNode *next_z_ptr_{nullptr};  ///< 坐标系z轴方向后置节点

  CoordinateSystem *coordinate_system_ptr_{nullptr};  ///< 节点所在坐标系系统

#if defined(TPN_DEBUG)
  std::string desc_;  ///< 调试描述符
#endif
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_SERVER_CELL_COORDINATE_NODE_H_
