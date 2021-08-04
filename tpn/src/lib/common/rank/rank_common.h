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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_COMMON_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_COMMON_H_

#include "rank_define.h"

namespace tpn {

namespace rank {

/// 将排行榜类型转换为底层需要的类型
///  @param[in]   type    排行类型
///  @return 底层类型
TPN_COMMON_API uint16_t TransformRankType(RankType type);

/// 获取排行榜排序依据key
///  @param[in]   type    底层类型
///  @return 排行榜排序依据key
TPN_COMMON_API uint8_t GetRankKeyType(uint16_t type);

/// 获取排行榜对应字段排序规则
///  @param[in]   type    底层类型
///  @return 排行榜对应字段排序规则
TPN_COMMON_API uint8_t GetRankKeyOrderType(uint16_t type);

/// 获取排序依据key类型对应的key大小
///  @param[in]   type    排行依据key类型
///  @return key对应的大小
TPN_COMMON_API size_t GetSizeByRankKeyType(uint8_t type);

}  // namespace rank

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_RANK_COMMON_H_
