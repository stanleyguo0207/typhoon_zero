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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CONTAINERS_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CONTAINERS_H_

#include <algorithm>
#include <vector>

#include "random_hub.h"

namespace tpn {

namespace containers {

/// 容器随机洗牌
///  @tparam			C						容器类型
///  @param[out]	container		容器
template <typename C>
TPN_INLINE void RandomShuffe(C &container) {
  std::shuffle(std::begin(container), std::end(container),
               *RandomEngine::Instance());
}

/// 随机选取容器中的元素
///  @attention		容器不可以为空
///  @tparam			C						容器类型
///  @param[out]	container		容器
///  @return 容器中的元素
template <typename C>
TPN_INLINE decltype(auto) SelectRandomContainerElement(const C &container) {
  auto iter = std::begin(container);
  std::advance(iter,
               RandU32(0, static_cast<uint32_t>(std::size(container)) - 1));
  return (*iter);
}

/// 随机选取容器中的元素
///  @attention		容器不可以为空
///  @tparam			C						容器类型
///  @param[out]	container		容器
///  @param[int]	weights			容器对应的权重，权重与容器元素是对应的顺序关系，
///														使用者需要保证权重总和是大于0
///  @return 容器中的元素迭代器
template <typename C>
TPN_INLINE decltype(auto) SelectRandomWeightContainerElementIterator(
    const C &container, std::vector<double> &weights) {
  auto iter = std::begin(container);
  std::advance(iter, RandU32Weighted(weights.data(), weights.size()));
  return iter;
}

/// 随机选取容器中的元素
///  @attention		容器不可以为空
///  @tparam			C									容器类型
///  @tparam			Fn								权重提取器类型 参数为容器的元素 返回double类型的权重
///  @param[out]	container					容器
///  @param[in]		weight_extractor	权重提取器，生成每个容器每个对应元素的权重
///																	使用者需要保证权重总和是大于0
///  @return 容器中的元素迭代器
template <typename C, typename Fn>
TPN_INLINE decltype(auto) SelectRandomWeightContainerElementIterator(
    const C &container, Fn weight_extractor) {
  std::vector<double> weights;
  weights.reserve(std::size(container));
  double weight_total = 0.0f;
  for (auto &&val : container) {
    double weight = weight_extractor(val);
    weights.emplace_back(weight);
    weight_total += weight;
  }

  if (weight_total <= std::numeric_limits<double>::epsilon()) {
    weights.assign(std::size(container), 1.0f);
  }

  return SelectRandomWeightContainerElementIterator(container, weights);
}

}  // namespace containers

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_CONTAINERS_H_
