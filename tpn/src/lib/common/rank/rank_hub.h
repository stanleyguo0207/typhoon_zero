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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_HUB_H_

#include <array>

#include "rank_common.h"

namespace tpn {

namespace rank {

/// 排行榜中枢
class TPN_COMMON_API RankHub {
 public:
  /// 初始化
  void Init();

  /// 更新排行
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   uid     玩家id
  ///  @param[in]   score   排行榜分数
  ///  @param[in]   p1      p1参数
  ///  @param[in]   p2      p2参数
  ///  @return 成功返回true
  bool UpdateRank(RankType type, uint64_t uid, uint64_t score, uint64_t p1 = 0,
                  uint64_t p2 = 0);

  /// 移除排行
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   uid     玩家id
  ///  @return 成功返回true
  bool RemoveRank(RankType type, uint64_t uid);

  /// 根据uid获取分数
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   uid     玩家id
  ///  @return 分数
  uint64_t GetScore(RankType type, uint64_t uid);

  /// 根据uid获取排行
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   uid     玩家id
  ///  @return 排行 从1开始，等于0说明该uid不存在
  size_t GetRank(RankType type, uint64_t uid);

  /// 根据uid获取反向排行
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   uid     玩家id
  ///  @return 反向排行 从1开始，等于0说明该uid不存在
  size_t GetRevRank(RankType type, uint64_t uid);

  /// 根据排行获取uid
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   rank    排行
  ///  @return uid 等于0说明不存在
  uint64_t GetUidByRank(RankType type, size_t rank);

  /// 根据反向排行获取uid
  ///  @param[in]   type    排行榜类型
  ///  @param[in]   rank    反向排行
  ///  @return uid 等于0说明不存在
  uint64_t GetUidByRevRank(RankType type, size_t rank);

  /// 获取排名范围
  ///  @param[in]   type          排行榜类型
  ///  @param[in]   rank_start    起始排名
  ///  @param[in]   rank_end      结束排名 默认0 从起始位置到整个排名范围
  ///  @return 排名范围内的玩家id集合
  std::vector<uint64_t> GetRange(RankType type, size_t rank_start = 0,
                                 size_t rank_end = 0);

  /// 获取反向排名范围
  ///  @param[in]   type          排行榜类型
  ///  @param[in]   rank_start    反向起始排名
  ///  @param[in]   rank_end      反向结束排名 默认0 从起始位置到整个排名范围
  ///  @return 排名范围内的玩家id集合
  std::vector<uint64_t> GetRevRange(RankType type, size_t rank_start = 0,
                                    size_t rank_end = 0);

  /// 打印数据
  ///  @param[in]   type    排行榜类型
  /// @note 测试用
  void PrintStorage(RankType type) const;

 private:
  std::array<RankListUptr, RankType::kRankTypeMax> ranks_;  ///< 所有排行

  TPN_SINGLETON_DECL(RankHub)
};

}  // namespace rank

}  // namespace tpn

/// global rank hub instance
#define g_rank_hub tpn::rank::RankHub::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_HUB_H_
