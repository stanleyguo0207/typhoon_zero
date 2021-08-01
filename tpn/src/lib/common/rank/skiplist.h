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

#include "define.h"

namespace tpn {

constexpr size_t kSkipListMaxLevel    = 32;    ///< 2 ^ 64 元素
constexpr double kSkipListProbability = 0.25;  ///< 随机层数概率

/// 跳表节点类型
/// 0xFF | 0xFF
/// 高8位为类型的排序顺序 默认0 Descending， 1 Ascending
/// 低8位为类型的个数
/// 高8位的标志是根据低8位的个数相关的，大于低8位的个数，无效
/// @note   0000 0101 0000 0100
///         kSkipListTypeOrderP2Asc | kSkipListTypeOrderS0Asc | kSkipListTypeS0P1P2
///         将被解析为 key为3个参数 长度为4 uid,socre,p1,p2
///         其中score为升序，p1为降序，p2为升序
/// @sa GetKeySizeByType 低8位的长度
enum SkipListType : uint16_t {
  kSkipListTypeNone       = 0x0,          ///< 默认
  kSkipListTypeS0         = 0x1,          ///< 1|score
  kSkipListTypeS0P1       = (0x1 << 1),   ///< 1|score 2|p1
  kSkipListTypeS0P1P2     = (0x1 << 2),   ///< 1|score 2|p1 3|p2
  kSkipListTypeOrderS0Asc = (0x1 << 8),   ///< 默认desc 1|score|asc
  kSkipListTypeOrderP1Asc = (0x1 << 9),   ///< 默认desc 2|p1|asc
  kSkipListTypeOrderP2Asc = (0x1 << 10),  ///< 默认desc 3|p2|asc
};

class SkipListNode;
using SkipListNodeSptr    = std::shared_ptr<SkipListNode>;
using SkipListNodeArrSptr = std::shared_ptr<SkipListNode[]>;

/// 跳表层结构
class SkipListLevel {
 public:
  SkipListLevel();
  ~SkipListLevel();

  SkipListNodeSptr GetForward();
  SkipListNodeSptr GetForward() const;
  void SetForward(SkipListNodeSptr node_sptr);

  size_t &GetSpan();
  const size_t &GetSpan() const;
  void SetSpan(size_t span);

 private:
  SkipListNodeSptr forward_{nullptr};  ///< 前置跳表节点
  size_t span_{0};                     ///< 跨越节点
};

using SkipListLevelArrUptr = std::unique_ptr<SkipListLevel[]>;

using SkipListNodeUakArrUptr = std::unique_ptr<uint64_t[]>;

/// 跳表节点
class SkipListNode {
 public:
  /// 构造函数
  ///  @param[in]   level     当前节点创建层数
  SkipListNode(int level);

  /// 构造函数
  ///  @param[in]   level     当前节点创建层数
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  SkipListNode(int level, SkipListNodeUakArrUptr uaks);

  ~SkipListNode();

  uint64_t *GetUaks();
  uint64_t *GetUaks() const;
  void SetUaks(SkipListNodeUakArrUptr uaks);

  SkipListNodeSptr GetBackward();
  SkipListNodeSptr GetBackward() const;
  void SetBackward(SkipListNodeSptr node_sptr);

  SkipListLevel *GetLevels();
  SkipListLevel *GetLevels() const;
  void SetLevels(SkipListLevelArrUptr levels);

 private:
  SkipListNodeUakArrUptr uaks_{nullptr};  ///< uid and keys
  SkipListNodeSptr backward_{nullptr};    ///< 后置跳表节点
  SkipListLevelArrUptr levels_{nullptr};  ///< 该节点的层数据
};

/// 跳表
class SkipList {
 public:
  /// 构造函数
  ///  @param[in]   type    调表节点类型
  explicit SkipList(uint16_t type);

  ~SkipList();

  /// 插入数据
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  ///  @return 成功返回true
  bool Insert(SkipListNodeUakArrUptr uaks);

  /// 删除数据
  ///  @param[in]   uid       玩家id
  ///  @return 成功返回true
  bool Delete(uint64_t uid);

  /// 更新数据
  ///  @param[in]   uaks      玩家id与跳表key集合，根据节点类型不同，所需要的参数个数不同
  ///  @return 成功返回true
  bool Update(SkipListNodeUakArrUptr uaks);

  /// 根据玩家id获取分数
  ///  @param[in]   uid       玩家id
  ///  @return 返回分数
  uint64_t GetScore(uint64_t uid);

  /// 获取跳表类型
  ///  @return 跳表类型
  uint16_t GetType();

  /// 比较uaks的大小
  ///  @param[in]   left    对象1
  ///  @param[in]   right   对象2
  ///  @return 返回uaks的大小 用于rank中的排序规则 left < right 返回true
  bool CompUaks(uint64_t left[], uint64_t right[]);

  /// 打印数据
  void PrintStorage() const;

  /// 根据类型获取对应的key的大小
  ///  @param[in]   type    调表节点类型
  ///  @return 类型对应的key大小
  static constexpr size_t GetKeySizeByType(uint16_t type);

  /// 随机层数
  ///  @return 节点层数
  static int GetRandomLevel();

 private:
  uint16_t type_{SkipListType::kSkipListTypeNone};      ///< 跳表节点类型
  SkipListNodeSptr header_{nullptr};                    ///< 头结点
  SkipListNodeSptr tail_{nullptr};                      ///< 尾结点
  size_t length_{0};  ///< 跳表中节点总个数
  int level_{1};      ///< 跳表中最高层数
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_SKIPLIST_H_
