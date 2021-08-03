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

#include "define.h"
#include "debug_hub.h"

namespace tpn {

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
};

/// 将排行榜类型转换为底层需要的类型
///  @return 底层类型
TPN_COMMON_API constexpr uint16_t TransformRankType(RankType type) {
  switch (type) {
    case RankType::kRankTypeTest: {
      return MAKE_UINT16_T(kRankKeyOrderTypeS0Asc | kRankKeyOrderTypeP1Asc,
                           kRankKeyTypeS0P1);
    } break;
    default:
      TPN_ASSERT(false, "Rank type unkown, {}", type);
      break;
  }

  return 0;
};

/// 获取排行榜排序依据key
///  @return 排行榜排序依据key
TPN_COMMON_API constexpr uint8_t GetRankKeyType(uint16_t type) {
  return GET_UINT16_T_L(type);
}

/// 获取排行榜对应字段排序规则
///  @return 排行榜对应字段排序规则
TPN_COMMON_API constexpr uint8_t GetRankKeyOrderType(uint16_t type) {
  return GET_UINT16_T_H(type);
}

/// 获取排序依据key类型对应的key大小
TPN_COMMON_API constexpr size_t GetSizeByRankKeyType(uint8_t type) {
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

  return 0;  // !危险 外面使用注意0的情况
}

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_DEFINE_H_
