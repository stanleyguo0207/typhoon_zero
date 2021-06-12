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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_RANDOM_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_RANDOM_HUB_H_

#include <cmath>

#include <limits>
#include <vector>

#include "define.h"
#include "chrono_wrap.h"

namespace tpn {

/// uint32_t范围随机
///  @return [0, UINT32_MAX)
TPN_COMMON_API uint32_t Rand32();

/// int32_t范围随机
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值 默认 0
///  @param[in]		max			随机范围的最大值 默认 INT32_MAX - 1
///  @return [min, max] (max - min < INT32_MAX)
TPN_COMMON_API int32_t
RandI32(int32_t min = 0, int32_t max = std::numeric_limits<int32_t>::max() - 1);

/// uint32_t范围随机
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值 默认 0
///  @param[in]		max			随机范围的最大值 默认 UINT32_MAX - 1
///  @return [min, max] (max - min < UINT32_MAX)
TPN_COMMON_API uint32_t RandU32(
    uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max() - 1);

/// float范围随机
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值 默认 std::numeric_limits<float>::min()
///  @param[in]		max			随机范围的最大值 默认 std::numeric_limits<float>::max()
///  @return [min, max]
TPN_COMMON_API float RandFloat(float min = std::numeric_limits<float>::min(),
                               float max = std::numeric_limits<float>::max());

/// double范围随机
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值 默认 std::numeric_limits<double>::min()
///  @param[in]		max			随机范围的最大值 默认 std::numeric_limits<double>::max()
///  @return [min, max]
TPN_COMMON_API double RandDouble(
    double min = std::numeric_limits<double>::min(),
    double max = std::numeric_limits<double>::max());

/// 毫秒随机 uint32_t
/// 相当与 RandU32(min * IN_MILLISECONDS, max * IN_MILLISECONDS)
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @attention		(std::numeric_limits<uint32_t>::max() / MilliSeconds::period::den) >= max 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值 默认 0
///  @param[in]		max			随机范围的最大值 默认 std::numeric_limits<double>::max() / MilliSeconds::period::den
///  @return [min, max]
TPN_COMMON_API uint32_t
RandMS(uint32_t min = 0, uint32_t max = std::numeric_limits<uint32_t>::max() /
                                        MilliSeconds::period::den);

/// 毫秒精度时间随机
///  @attention		max >= min 需要调用者保证，否则底层将会断言中断
///  @attention		max.count() - min.count() 需要在[0, 0xFFFFFFFF]范围内 需要调用者保证，否则底层将会断言中断
///  @param[in]		min			随机范围的最小值
///  @param[in]		max			随机范围的最大值
///  @return [min, max]
TPN_COMMON_API MilliSeconds RandTime(const MilliSeconds &min,
                                     const MilliSeconds &max);

/// 规范随机
///  @return [0.0, 1.0)
TPN_COMMON_API double RandNorm();

/// 机会随机
///  @return [0.0, 100.0)
TPN_COMMON_API double RandChance();

/// 权重随机
///  @param[in]		weights		权重
///  @param[in]		count			权重个数
///  @return [0, count)
TPN_COMMON_API uint32_t RandU32Weighted(const double *weights, size_t count);

/// 浮点型机会随机
///  @param[in]		chance		roll的概率下限
///  @return roll 中返回true
inline bool RollChanceFloat(float chance) { return chance > RandChance(); }

/// int32_t机会随机
///  @param[in]		chance		roll的概率下限
///  @return roll 中返回true
inline bool RollChanceI32(int32_t chance) { return chance > RandI32(0, 99); }

/// 随机数生成器引擎
struct TPN_COMMON_API RandomEngine {
  using result_type = uint32_t;

  static constexpr result_type min() {
    return std::numeric_limits<result_type>::min();
  }

  static constexpr result_type max() {
    return std::numeric_limits<result_type>::max();
  }

  result_type operator()() const { return Rand32(); }

  TPN_SINGLETON_DECL(RandomEngine)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_RANDOM_HUB_H_
