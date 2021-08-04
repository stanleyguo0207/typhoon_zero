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

#include "rank_common.h"

#include "debug_hub.h"

namespace tpn {

namespace rank {

/// 将排行榜类型转换为底层需要的类型
///  @return 底层类型
uint16_t TransformRankType(RankType type) {
  switch (type) {
    case RankType::kRankTypeTest: {
      return MAKE_UINT16_T(kRankKeyOrderTypeS0Asc | kRankKeyOrderTypeP1Asc,
                           kRankKeyTypeS0P1);
    } break;
    default:
      break;
  }

  TPN_ASSERT(false, "Rank type unkown, {}", type);
  return 0;
}

/// 获取排行榜排序依据key
///  @return 排行榜排序依据key
uint8_t GetRankKeyType(uint16_t type) { return GET_UINT16_T_L(type); }

/// 获取排行榜对应字段排序规则
///  @return 排行榜对应字段排序规则
uint8_t GetRankKeyOrderType(uint16_t type) { return GET_UINT16_T_H(type); }

/// 获取排序依据key类型对应的key大小
size_t GetSizeByRankKeyType(uint8_t type) {
  switch (type) {
    case kRankKeyTypeS0: {  // uid + score
      return 2;
    } break;
    case kRankKeyTypeS0P1: {  // uid + score + p1
      return 3;
    } break;
    case kRankKeyTypeS0P1P2: {  // uid + score + p1 + p2
      return 4;
    } break;
    default:
      break;
  }

  TPN_ASSERT(false, "RankKey type unkown, {}", type);
  return 0;
}

}  // namespace rank

}  // namespace tpn
