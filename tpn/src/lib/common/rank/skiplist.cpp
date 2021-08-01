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

#include <vector>

#include "random_hub.h"
#include "utils.h"

namespace tpn {

SkipListLevel::SkipListLevel() {}

SkipListLevel::~SkipListLevel() {}

SkipListNodeSptr SkipListLevel::GetForward() { return forward_; }

SkipListNodeSptr SkipListLevel::GetForward() const { return forward_; }

void SkipListLevel::SetForward(SkipListNodeSptr node_sptr) {
  forward_ = std::move(node_sptr);
}

size_t &SkipListLevel::GetSpan() { return span_; }

const size_t &SkipListLevel::GetSpan() const { return span_; }

void SkipListLevel::SetSpan(size_t span) { span_ = span; }

SkipListNode::SkipListNode(int level) {
  levels_ = std::make_unique<SkipListLevel[]>(level);
}

SkipListNode::SkipListNode(int level, SkipListNodeUakArrUptr uaks)
    : uaks_(std::move(uaks)) {
  levels_ = std::make_unique<SkipListLevel[]>(level);
}

SkipListNode::~SkipListNode() {}

uint64_t *SkipListNode::GetUaks() { return uaks_.get(); }

uint64_t *SkipListNode::GetUaks() const { return uaks_.get(); }

void SkipListNode::SetUaks(SkipListNodeUakArrUptr uaks) {
  uaks_ = std::move(uaks);
}

SkipListNodeSptr SkipListNode::GetBackward() { return backward_; }

SkipListNodeSptr SkipListNode::GetBackward() const { return backward_; }

void SkipListNode::SetBackward(SkipListNodeSptr node_sptr) {
  backward_ = std::move(node_sptr);
}

SkipListLevel *SkipListNode::GetLevels() { return levels_.get(); }

SkipListLevel *SkipListNode::GetLevels() const { return levels_.get(); }

void SkipListNode::SetLevels(SkipListLevelArrUptr levels) {
  levels_ = std::move(levels);
}

SkipList::SkipList(SkipListType type) : type_(type) {
  header_ = std::make_shared<SkipListNode>(kSkipListMaxLevel);
}

SkipList::~SkipList() {}

bool SkipList::Insert(SkipListNodeUakArrUptr uaks) {
  std::vector<SkipListNodeSptr> update(kSkipListMaxLevel);
  size_t rank[kSkipListMaxLevel];

  SkipListNodeSptr x = header_;
  for (int i = level_ - 1; i >= 0; --i) {
    rank[i] = (i == level_ - 1) ? 0 : rank[i - 1];
    while (x->GetLevels()[i].GetForward() &&
           (CompUaks(x->GetLevels()[i].GetForward()->GetUaks(), uaks.get()))) {
      rank[i] += x->GetLevels()[i].GetSpan();
      x = x->GetLevels()[i].GetForward();
    }
    update[i] = x;
  }

  int level = GetRandomLevel();
  if (level > level_) {
    for (int i = level_; i < level; ++i) {
      rank[i]   = 0;
      update[i] = header_;
      update[i]->GetLevels()[i].SetSpan(length_);
    }
    level_ = level;
  }

  x = std::make_shared<SkipListNode>(level, std::move(uaks));
  // uaks 失效

  for (int i = 0; i < level; ++i) {
    x->GetLevels()[i].SetForward(update[i]->GetLevels()[i].GetForward());
    update[i]->GetLevels()->SetForward(x);

    x->GetLevels()[i].SetSpan(update[i]->GetLevels()[i].GetSpan() -
                              (rank[0] - rank[i]));
    update[i]->GetLevels()[i].SetSpan((rank[0] - rank[i]) + 1);
  }

  for (int i = level; i < level_; ++i) {
    ++(update[i]->GetLevels()[i].GetSpan());
  }

  x->SetBackward(update[0] == header_ ? nullptr : update[0]);
  if (x->GetLevels()[0].GetForward()) {
    x->GetLevels()[0].GetForward()->SetBackward(x);
  } else {
    tail_ = x;
  }

  ++length_;

  return true;
}

bool SkipList::Delete(uint64_t uid) { return true; }

bool SkipList::Update(SkipListNodeUakArrUptr uaks) { return true; }

uint64_t SkipList::GetScore(uint64_t uid) { return 0; }

SkipListType SkipList::GetType() { return type_; }

bool SkipList::CompUaks(uint64_t left[], uint64_t right[]) {
  size_t key_size = GetKeySizeByType(type_);
  if (key_size > 0) {
    uint16_t order = type_ & 0xFF00;
    uint16_t mask  = (0x1 << 8);
    for (size_t i = 1; i < key_size; ++i) {
      if (mask & order) {  // asc
        if (*(left + i) > *(right + i)) {
          return true;
        }
      } else {  // desc
        if (*(left + i) < *(right + i)) {
          return true;
        }
      }

      mask <<= 1;
    }
  }
  return false;
}

constexpr size_t SkipList::GetKeySizeByType(SkipListType type) {
  // 只验证低8位
  switch (type & 0x00FF) {
    case SkipListType::kSkipListTypeS0: {  // uid + score
      return 2;
    } break;
    case SkipListType::kSkipListTypeS0P1: {  // uid + score + p1
      return 3;
    } break;
    case SkipListType::kSkipListTypeS0P1P2: {  // uid + score + p1 + p2
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
