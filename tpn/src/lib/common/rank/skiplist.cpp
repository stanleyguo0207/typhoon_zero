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

#include "skiplist.h"

#include "random_hub.h"

namespace tpn {

SkipListLevel::SkipListLevel() {}

SkipListLevel::~SkipListLevel() {}

SkipListNode::SkipListNode(int level) {
  levels_ = std::make_unique<SkipListLevel[]>(level);
}

SkipListNode::SkipListNode(int level, SkipListNodeUakArrUptr uaks)
    : uaks_(std::move(uaks)) {
  levels_ = std::make_unique<SkipListLevel[]>(level);
}

SkipListNode::~SkipListNode() {}

SkipList::SkipList() {
  header_ = std::make_shared<SkipListNode>(kSkipListMaxLevel);
}

SkipList::SkipList(SkipListType type) : type_(type) {
  header_ = std::make_shared<SkipListNode>(kSkipListMaxLevel);
}

SkipList::~SkipList() {}

bool SkipList::Insert(SkipListNodeUakArrUptr uaks) {
  SkipListNodeArrSptr update =
      std::make_shared<SkipListNode>(kSkipListMaxLevel);
  SkipListNodeSptr x = header_;
  for (int i = level_ - 1; i >= 0; --i) {
  }

  int level = GetRandomLevel();

  return true;
}

bool SkipList::Delete(uint64_t uid) { return true; }

bool SkipList::Update(SkipListNodeUakArrUptr uaks) { return true; }

uint64_t SkipList::GetScore(uint64_t uid) { return 0; }

SkipListType SkipList::GetType() { return type_; }

constexpr size_t SkipList::GetKeySizeByType(SkipListType type) {
  switch (type) {
    case SkipListType::kSkipListTypeScore: {  // uid + score
      return 2;
    } break;
    case SkipListType::kSkipListTypeScoreP1: {  // uid + score + p1
      return 3;
    } break;
    case SkipListType::kSkipListTypeScoreP1P2: {  // uid + score + p1 + p2
      return 4;
    } break;
    default:
      break;
  }

  return 0;  // !!危险 外面使用注意0的情况
}

int SkipList::GetRandomLevel() {
  int level = 1;
  while ((RandI32() & 0xFFFF) < (kSkipListProbability * 0xFFFF)) {
    ++level;
  }

  return (level < kSkipListMaxLevel) ? level : kSkipListMaxLevel;
}

}  // namespace tpn
