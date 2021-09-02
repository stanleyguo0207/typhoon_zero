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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_NODE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_NODE_H_

#include <limits>
#include <string>
#include <string_view>

#include "aoi_fwd.h"
#include "enum_flag.h"

namespace tpn {

namespace aoi {

/// 视野节点标志
/// 当前视野节点的状态
enum class AOINodeFlag : uint32_t {
  kAOINodeFlagNone       = 0x0,         ///< 无状态
  kAOINodeFlagEntity     = 0x1,         ///< entity节点
  kAOINodeFlagTrigger    = (0x1 << 1),  ///< trigger节点
  kAOINodeFlagHide       = (0x1 << 2),  ///< 节点隐藏(其他节点不可见)
  kAOINodeFlagRemoving   = (0x1 << 3),  ///< 删除中的节点
  kAOINodeFlagRemoved    = (0x1 << 4),  ///< 删除的节点
  kAOINodeFlagPending    = (0x1 << 5),  ///< 处于update操作中的节点
  kAOINodeFlagUpdating   = (0x1 << 6),  ///< 正在update操作中的节点
  kAOINodeFlagInstalling = (0x1 << 7),  ///< 安装中的节点
  kAOINodeFlagPositiveBoundary = (0x1 << 8),  ///< 处于正边界的节点
  kAOINodeFlagNagativeBoundary = (0x1 << 9),  ///< 处于负边界的节点
  kAOINodeFlagHideOrRemoved    = kAOINodeFlagHide | kAOINodeFlagRemoved,
};

/// 视野节点
/// 底层使用xyz方向的双向链表组成
class AOINode {
 public:
  /// 构造函数
  ///  @param[in]   aoi_mgr     视野管理器
  AOINode(AOIMgr *aoi_mgr = nullptr);
  /// 析构函数
  virtual ~AOINode();

  /// 获取节点本身坐标系x坐标
  ///  @return 节点本身坐标系x坐标
  virtual float GetX() const;
  /// 获取节点本身坐标系y坐标
  ///  @return 节点本身坐标系y坐标
  virtual float GetY() const;
  /// 获取节点本身坐标系z坐标
  ///  @return 节点本身坐标系z坐标
  virtual float GetZ() const;

  /// 设置节点本身坐标系x坐标
  ///  @param[in]   v     坐标值
  virtual void SetX(float v);
  /// 设置节点本身坐标系y坐标
  ///  @param[in]   v     坐标值
  virtual void SetY(float v);
  /// 设置节点本身坐标系z坐标
  ///  @param[in]   v     坐标值
  virtual void SetZ(float v);

  /// 获取节点扩展坐标系当前x坐标
  ///  @return 节点扩展坐标系当前x坐标
  virtual float GetExtX() const;
  /// 获取节点扩展坐标系当前y坐标
  ///  @return 节点扩展坐标系当前y坐标
  virtual float GetExtY() const;
  /// 获取节点扩展坐标系当前z坐标
  ///  @return 节点扩展坐标系当前z坐标
  virtual float GetExtZ() const;

  /// 获取节点扩展坐标系缓存x坐标
  ///  @return 节点扩展坐标系缓存x坐标
  virtual float GetOldExtX() const;
  /// 获取节点扩展坐标系缓存y坐标
  ///  @return 节点扩展坐标系缓存y坐标
  virtual float GetOldExtY() const;
  /// 获取节点扩展坐标系缓存z坐标
  ///  @return 节点扩展坐标系缓存z坐标
  virtual float GetOldExtZ() const;

  /// 设置节点扩展坐标系缓存x坐标
  ///  @param[in]   v     节点扩展坐标系缓存x坐标
  virtual void SetOldExtX(float v);
  /// 设置节点扩展坐标系缓存y坐标
  ///  @param[in]   v     节点扩展坐标系缓存y坐标
  virtual void SetOldExtY(float v);
  /// 设置节点扩展坐标系缓存z坐标
  ///  @param[in]   v     节点扩展坐标系缓存z坐标
  virtual void SetOldExtZ(float v);

  /// 重置缓存的扩展坐标系坐标
  virtual void ResetOldExtXYZ();

  /// 获取所有标志
  ///  @return 节点当前的所有标志
  TPN_INLINE uint32_t GetFlags() const;
  /// 设置单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void SetFlag(AOINodeFlag flag);
  /// 添加单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void AddFlag(AOINodeFlag flag);
  /// 移除单个标志
  ///  @param[in]   flag      单个标志
  TPN_INLINE void RemoveFlag(AOINodeFlag flag);
  /// 是否含有指定的单个标志
  ///  @param[in]   flag      要检查的单个标志
  ///  @return 含有传入的标志返回true
  TPN_INLINE bool HasFlag(AOINodeFlag flag) const;
  /// 是否含有指定的多个标志
  ///  @param[in]   flag      要检查的多个标志
  ///  @return 含有所有传入的标志返回true
  TPN_INLINE bool HasAllFlag(AOINodeFlag flag) const;

  /// 获取坐标系x轴方向前置节点
  ///  @return 坐标系x轴方向前置节点
  TPN_INLINE AOINode *GetPrevXPtr() const;
  /// 获取坐标系x轴方向后置节点
  ///  @return 坐标系x轴方向后置节点
  TPN_INLINE AOINode *GetNextXPtr() const;
  /// 获取坐标系y轴方向前置节点
  ///  @return 坐标系y轴方向前置节点
  TPN_INLINE AOINode *GetPrevYPtr() const;
  /// 获取坐标系y轴方向后置节点
  ///  @return 坐标系y轴方向后置节点
  TPN_INLINE AOINode *GetNextYPtr() const;
  /// 获取坐标系z轴方向前置节点
  ///  @return 坐标系z轴方向前置节点
  TPN_INLINE AOINode *GetPrevZPtr() const;
  /// 获取坐标系z轴方向后置节点
  ///  @return 坐标系z轴方向后置节点
  TPN_INLINE AOINode *GetNextZPtr() const;

  /// 获取坐标系x轴方向前置节点
  ///  @param[in]   node_ptr    坐标系x轴方向前置节点
  TPN_INLINE void SetPrevXPtr(AOINode *node_ptr);
  /// 获取坐标系x轴方向后置节点
  ///  @param[in]   node_ptr    坐标系x轴方向前置节点
  TPN_INLINE void SetNextXPtr(AOINode *node_ptr);
  /// 获取坐标系y轴方向前置节点
  ///  @param[in]   node_ptr    坐标系y轴方向前置节点
  TPN_INLINE void SetPrevYPtr(AOINode *node_ptr);
  /// 获取坐标系y轴方向后置节点
  ///  @param[in]   node_ptr    坐标系y轴方向前置节点
  TPN_INLINE void SetNextYPtr(AOINode *node_ptr);
  /// 获取坐标系z轴方向前置节点
  ///  @param[in]   node_ptr    坐标系z轴方向前置节点
  TPN_INLINE void SetPrevZPtr(AOINode *node_ptr);
  /// 获取坐标系z轴方向后置节点
  ///  @param[in]   node_ptr    坐标系z轴方向前置节点
  TPN_INLINE void SetNextZPtr(AOINode *node_ptr);

  /// 设置节点的视野管理器
  ///  @param[in]   aoi_mgr     事业管理器
  TPN_INLINE void SetAOIMgr(AOIMgr *aoi_mgr);
  /// 设置节点的视野管理器
  ///  @return 当前节点的视野管理器
  TPN_INLINE AOIMgr *GetAOIMgr() const;

  /// 某个节点x坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassX(AOINode *node_ptr, bool is_front);
  /// 某个节点y坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassY(AOINode *node_ptr, bool is_front);
  /// 某个节点z坐标变动经过本节点
  ///  @param[in]   node_ptr    变动的节点
  ///  @param[in]   is_front    向前移动为true
  virtual void OnNodePassZ(AOINode *node_ptr, bool is_front);

  /// 节点移除
  virtual void OnRemove();
  /// 父节点移除
  ///  @param[in]   parent_node_ptr     父节点移除
  virtual void OnParentRemove(AOINode *parent_node_ptr);

  /// 当前节点有变化，需要更新它的list中的相关位置等信息
  virtual void Update();

  std::string DebugStr();
  void DebugX();
  void DebugY();
  void DebugZ();

  void SetDescStr(std::string_view strv);
  virtual const char *GetDescCStr();

 private:
  float x_{std::numeric_limits<double>::lowest()};  ///< 节点本身坐标系x轴坐标
  float y_{std::numeric_limits<double>::lowest()};  ///< 节点本身坐标系y轴坐标
  float z_{std::numeric_limits<double>::lowest()};  ///< 节点本身坐标系z轴坐标

  float old_ext_x_{
      std::numeric_limits<double>::lowest()};  ///< 旧的扩展坐标系x轴坐标
  float old_ext_y_{
      std::numeric_limits<double>::lowest()};  ///< 旧的扩展坐标系y轴坐标
  float old_ext_z_{
      std::numeric_limits<double>::lowest()};  ///< 旧的扩展坐标系z轴坐标

  EnumFlag<AOINodeFlag> flags_{AOINodeFlag::kAOINodeFlagNone};  ///< 标志

  AOINode *prev_x_ptr_{nullptr};  ///< 坐标系x轴方向前置节点
  AOINode *next_x_ptr_{nullptr};  ///< 坐标系x轴方向后置节点
  AOINode *prev_y_ptr_{nullptr};  ///< 坐标系y轴方向前置节点
  AOINode *next_y_ptr_{nullptr};  ///< 坐标系y轴方向后置节点
  AOINode *prev_z_ptr_{nullptr};  ///< 坐标系z轴方向前置节点
  AOINode *next_z_ptr_{nullptr};  ///< 坐标系z轴方向后置节点

  AOIMgr *aoi_mgr_{nullptr};  ///< 节点所在的视野管理器

#if defined(TPN_AOIDEBUG)
 private:
  std::string desc_;  ///< 调试描述符
#endif
};

}  // namespace aoi

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_AOI_AOI_NODE_H_
