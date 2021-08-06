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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_DEFINE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_DEFINE_H_

#include <memory>

#include "define.h"

namespace tpn {

namespace rank {

/// 排行榜排序依据key类型
/// 与排序规则配合使用 @sa RankKeyOrderType
enum RankKeyType : uint8_t {
  kRankKeyTypeNone   = 0x0,         ///< 0|uid
  kRankKeyTypeS0     = 0x1,         ///< 0|uid 1|score
  kRankKeyTypeS0P1   = (0x1 << 1),  ///< 0|uid 1|score 2|p1
  kRankKeyTypeS0P1P2 = (0x1 << 2),  ///< 0|uid 1|score 2|p1 3|p2
};

/// 排行榜对应字段排序规则
/// 默认0 Ascending， 1 Descending
/// 与排序依据key配合使用 @sa RankKeyType
enum RankKeyOrderType : uint8_t {
  kRankKeyOrderTypeS0Desc = 0x1,         ///< 默认asc 1|score|desc
  kRankKeyOrderTypeP1Desc = (0x1 << 1),  ///< 默认asc 2|p1|desc
  kRankKeyOrderTypeP2Desc = (0x1 << 2),  ///< 默认asc 3|p2|desc
};

/// 排行类型
/// 基本够用了，如果需求不满足可以后面家类型
/// 每种类型需要在 @sa TransformRankType 中处理转换类型
enum RankType : uint16_t {
  kRankTypeTest     = 0,   ///< 测试排行
  kRankTypeSA       = 1,   ///< 0|uid 1|score|asc
  kRankTypeSD       = 2,   ///< 0|uid 1|score|desc
  kRankTypeSAP1A    = 3,   ///< 0|uid 1|score|asc 2|p1|asc
  kRankTypeSAP1D    = 4,   ///< 0|uid 1|score|asc 2|p1|desc
  kRankTypeSDP1A    = 5,   ///< 0|uid 1|score|desc 2|p1|asc
  kRankTypeSDP1D    = 6,   ///< 0|uid 1|score|desc 2|p1|desc
  kRankTypeSAP1AP2A = 7,   ///< 0|uid 1|score|asc 2|p1|asc 3|p2|asc
  kRankTypeSAP1AP2D = 8,   ///< 0|uid 1|score|asc 2|p1|asc 3|p2|desc
  kRankTypeSAP1DP2A = 9,   ///< 0|uid 1|score|asc 2|p1|desc 3|p2|asc
  kRankTypeSAP1DP2D = 10,  ///< 0|uid 1|score|asc 2|p1|desc 3|p2|desc
  kRankTypeSDP1AP2A = 11,  ///< 0|uid 1|score|desc 2|p1|asc 3|p2|asc
  kRankTypeSDP1AP2D = 12,  ///< 0|uid 1|score|desc 2|p1|asc 3|p2|desc
  kRankTypeSDP1DP2A = 13,  ///< 0|uid 1|score|desc 2|p1|desc 3|p2|asc
  kRankTypeSDP1DP2D = 14,  ///< 0|uid 1|score|desc 2|p1|desc 3|p2|desc
  kRankTypeMax             ///< 排行榜总数 不要在这个值后面添加
};

class SkipList;
using RankList     = SkipList;
using RankListUptr = std::unique_ptr<RankList>;

}  // namespace rank

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_DEFINE_H_
