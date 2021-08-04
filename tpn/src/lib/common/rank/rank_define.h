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
/// 默认0 Descending， 1 Ascending
/// 与排序依据key配合使用 @sa RankKeyType
enum RankKeyOrderType : uint8_t {
  kRankKeyOrderTypeS0Asc = 0x1,         ///< 默认desc 1|score|asc
  kRankKeyOrderTypeP1Asc = (0x1 << 1),  ///< 默认desc 2|p1|asc
  kRankKeyOrderTypeP2Asc = (0x1 << 2),  ///< 默认desc 3|p2|asc
};

/// 排行类型
enum RankType : uint16_t {
  kRankTypeTest = 0,  ///< 测试排行
  kRankTypeMax        ///< 排行榜总数 不要在这个值后面添加
};

class SkipList;
using RankList     = SkipList;
using RankListUptr = std::unique_ptr<RankList>;

}  // namespace rank

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_DEFINE_H_
