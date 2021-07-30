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

/// 跳表节点类型
enum class SkipListType : uint8_t {
  kSkipListTypeNone      = 0,  ///< 默认
  kSkipListTypeScore     = 1,  ///< 1|score
  kSkipListTypeScoreP1   = 2,  ///< 1|score 2|p1
  kSkipListTypeScoreP1P2 = 3,  ///< 1|score 2|p1 3|p2
};

class SkipListNode;
using SkipListNodeSptr = std::shared_ptr<SkipListNode>;

/// 跳表层结构
class SkipListLevel {};

/// 跳表节点
class SkipListNode {};

/// 跳表
class SkipList {};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_RANK_SKIPLIST_H_
