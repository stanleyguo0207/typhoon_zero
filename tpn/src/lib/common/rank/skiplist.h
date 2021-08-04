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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_SKIPLIST_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_SKIPLIST_H_

#include <memory>
#include <unordered_map>

#include "rank_common.h"

namespace tpn {

namespace rank {

class SkipListNode;
using SkipListNodeSptr    = std::shared_ptr<SkipListNode>;
using SkipListNodeArrSptr = std::shared_ptr<SkipListNode[]>;

/// 跳表层结构
class TPN_COMMON_API SkipListLevel {
 public:
  /// 构造函数
  SkipListLevel();

  /// 析构函数
  ~SkipListLevel();

  /// 获取前置跳表节点
  ///  @return 前置跳表节点
  SkipListNodeSptr GetForward();

  /// 获取前置跳表节点
  ///  @return 前置跳表节点
  SkipListNodeSptr GetForward() const;

  /// 设置前置跳表节点
  ///  @param[in]   node_sptr     跳表节点
  void SetForward(SkipListNodeSptr node_sptr);

  /// 获取与后置节点的距离
  ///  @return 跨越的长度
  size_t GetSpan();

  /// 获取与后置节点的距离
  ///  @return 跨越的长度
  const size_t GetSpan() const;

  /// 设置与后置节点的距离
  ///  @param[in]   span      与后置节点的距离
  void SetSpan(size_t span);

 private:
  SkipListNodeSptr forward_{nullptr};  ///< 前置跳表节点
  size_t span_{0};                     ///< 跨越节点
};

using SkipListLevelArrUptr = std::unique_ptr<SkipListLevel[]>;

using SkipListNodeUakArrUptr = std::unique_ptr<uint64_t[]>;

/// 跳表节点
class TPN_COMMON_API SkipListNode {
 public:
  /// 构造函数
  ///  @param[in]   level     当前节点创建层数
  SkipListNode(int level);

  /// 构造函数
  ///  @param[in]   level     当前节点创建层数
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  SkipListNode(int level, SkipListNodeUakArrUptr uaks);

  ~SkipListNode();

  /// 获取玩家id
  /// @note uaks的第一个数据为uid
  /// @attention 使用时需要先判断需要通过list判断类型否则越界
  ///            不要直接使用使用这个接口，除非你了解
  ///  @return 玩家id
  const uint64_t GetUid() const;

  /// 获取分数
  /// @note uaks的第二个数据为score
  /// @attention 使用时需要先判断需要通过list判断类型否则越界
  ///            不要直接使用使用这个接口，除非你了解
  ///  @return 分数
  const uint64_t GetScore() const;

  /// 获取p1参数
  /// @note uaks的第三个数据为p1
  /// @attention 使用时需要先判断需要通过list判断类型否则越界
  ///            不要直接使用使用这个接口，除非你了解
  ///  @return p1
  const uint64_t GetP1() const;

  /// 获取p2参数
  /// @note uaks的第四个数据为p2
  /// @attention 使用时需要先判断需要通过list判断类型否则越界
  ///            不要直接使用使用这个接口，除非你了解
  ///  @return p2
  const uint64_t GetP2() const;

  /// 获取uak数据
  ///  @return 返回uak数据
  uint64_t *GetUaks();

  /// 获取uak数据
  ///  @return 返回uak数据
  const uint64_t *GetUaks() const;

  /// 设置uak数据
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  void SetUaks(SkipListNodeUakArrUptr uaks);

  /// 获取本节点的后置节点
  ///  @return 后置节点
  SkipListNodeSptr GetBackward();

  /// 获取本节点的后置节点
  ///  @return 后置节点
  const SkipListNodeSptr GetBackward() const;

  /// 设置本节点的后置节点
  ///  @param[in]   node_sptr   后置节点
  void SetBackward(SkipListNodeSptr node_sptr);

  /// 获取本节点的层数据
  ///  @return 层数据
  SkipListLevel *GetLevels();

  /// 获取本节点的层数据
  ///  @return 层数据
  const SkipListLevel *GetLevels() const;

  /// 设置本节点的层数据
  ///  @param[in]   levels      层数据
  void SetLevels(SkipListLevelArrUptr levels);

 private:
  SkipListNodeUakArrUptr uaks_{nullptr};  ///< uid and keys
  SkipListNodeSptr backward_{nullptr};    ///< 后置跳表节点
  SkipListLevelArrUptr levels_{nullptr};  ///< 该节点的层数据
};

using SkipListUidNodeUMap = std::unordered_map<uint64_t, SkipListNodeSptr>;

/// 跳表
class TPN_COMMON_API SkipList {
 public:
  /// 构造函数
  ///  @param[in]   type      跳表节点类型
  explicit SkipList(uint16_t type);

  ~SkipList();

  /// 插入数据
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  ///  @return 成功返回true
  bool Insert(SkipListNodeUakArrUptr uaks);

  /// 更新数据
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  ///  @return 成功返回true
  bool Update(SkipListNodeUakArrUptr uaks);

  /// 删除数据
  ///  @param[in]   uid       玩家id
  ///  @return 成功返回true
  bool Delete(uint64_t uid);

  /// 根据玩家id获取分数
  ///  @param[in]   uid       玩家id
  ///  @return 返回分数
  uint64_t GetScore(uint64_t uid);

  /// 根据玩家id获取排名
  ///  @param[in]   uid       玩家id
  ///  @return 返回排名 0 代表未找到 排名从1开始
  size_t GetRank(uint64_t uid);

  /// 根据玩家id获取反向排名
  ///  @param[in]   uid       玩家id
  ///  @return 返回排名 0 代表未找到 排名从1开始
  size_t GetRevRank(uint64_t uid);

  /// 根据排名获取玩家id
  ///  @param[in]   rank      排名 需要大于0
  ///  @return 玩家id
  uint64_t GetUidByRank(size_t rank);

  /// 根据反向排名获取玩家id
  ///  @param[in]   rank      排名 需要大于0
  ///  @return 玩家id
  uint64_t GetUidByRevRank(size_t rank);

  /// 获取排名范围
  ///  @param[in]   rank_start    起始排名
  ///  @param[in]   rank_end      结束排名 默认0 从起始位置到整个排名范围
  ///  @return 排名范围内的玩家id集合
  std::vector<uint64_t> GetRange(size_t rank_start = 0, size_t rank_end = 0);

  /// 获取反向排名范围
  ///  @param[in]   rank_start    反向起始排名
  ///  @param[in]   rank_end      反向结束排名 默认0 从起始位置到整个排名范围
  ///  @return 排名范围内的玩家id集合
  std::vector<uint64_t> GetRevRange(size_t rank_start = 0, size_t rank_end = 0);

  /// 获取跳表类型
  ///  @return 跳表类型
  uint16_t GetType();

  /// 打印数据
  /// @note 测试用
  void PrintStorage() const;

 private:
  /// 获取跳表节点
  ///  @param[in]   uid       玩家id
  ///  @return 存在返回节点，不存在返回nullptr
  SkipListNodeSptr GetNodeByUid(uint64_t uid);

  /// 获取跳表节点
  ///  @param[in]   rank      排名 排名需要大于0
  ///  @return 存在返回节点，不存在返回nullptr
  SkipListNodeSptr GetNodeByRank(size_t rank);

  /// 删除数据
  ///  @param[in]   node_sptr 节点数据
  ///  @return 成功返回true
  bool Delete(SkipListNodeSptr node_sptr);

  /// 删除数据
  ///  @param[in]   node_sptr 节点数据
  ///  @param[in]   update    需要更新的查找路径节点
  ///  @return 成功返回true
  bool Delete(SkipListNodeSptr node_sptr, std::vector<SkipListNodeSptr> update);

  /// 获取排名范围
  ///  @param[in]   rank_start    反向起始排名
  ///  @param[in]   rank_end      反向结束排名 默认0 从起始位置到整个排名范围
  ///  @param[in]   reverse       是否反向
  ///  @return 排名范围内的玩家id集合
  std::vector<uint64_t> GetRangeWithFlag(size_t rank_start = 0,
                                         size_t rank_end   = 0,
                                         bool reverse      = false);

  /// 获取玩家id
  ///  @param[in]   node_sptr 节点数据
  ///  @return 玩家id
  const uint64_t GetUid(SkipListNodeSptr node_sptr) const;

  /// 获取分数
  ///  @param[in]   node_sptr 节点数据
  ///  @return 分数
  const uint64_t GetScore(SkipListNodeSptr node_sptr) const;

  /// 获取p1参数
  ///  @param[in]   node_sptr 节点数据
  ///  @return p1
  const uint64_t GetP1(SkipListNodeSptr node_sptr) const;

  /// 获取p2参数
  ///  @param[in]   node_sptr 节点数据
  ///  @return p2
  const uint64_t GetP2(SkipListNodeSptr node_sptr) const;

  /// 比较uaks的大小
  ///  @param[in]   left    对象1
  ///  @param[in]   right   对象2
  ///  @return 返回uaks的大小 用于rank中的排序规则 left < right 返回true
  bool CompUaks(uint64_t left[], uint64_t right[]);

  /// 测试使用
  void CheckLength();

 private:
  uint16_t type_{0};                  ///< 跳表节点类型
  size_t uaks_size_{0};               ///< 跳表uaks对应的长度
  SkipListNodeSptr header_{nullptr};  ///< 头结点
  SkipListNodeSptr tail_{nullptr};    ///< 尾结点
  size_t length_{0};                  ///< 跳表中节点总个数
  int level_{1};                      ///< 跳表中最高层数
  SkipListUidNodeUMap uid_umap_;      ///< uid关联node
};

}  // namespace rank

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_SKIPLIST_H_
