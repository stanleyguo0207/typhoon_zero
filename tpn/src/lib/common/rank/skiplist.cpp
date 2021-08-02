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
#include <sstream>

#include "random_hub.h"
#include "utils.h"
#include "fmt_wrap.h"
#include "log.h"

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

const uint64_t SkipListNode::GetUid() const { return uaks_[0]; }

uint64_t *SkipListNode::GetUaks() { return uaks_.get(); }

const uint64_t *SkipListNode::GetUaks() const { return uaks_.get(); }

void SkipListNode::SetUaks(SkipListNodeUakArrUptr uaks) {
  uaks_ = std::move(uaks);
}

SkipListNodeSptr SkipListNode::GetBackward() { return backward_; }

SkipListNodeSptr SkipListNode::GetBackward() const { return backward_; }

void SkipListNode::SetBackward(SkipListNodeSptr node_sptr) {
  backward_ = std::move(node_sptr);
}

SkipListLevel *SkipListNode::GetLevels() { return levels_.get(); }

const SkipListLevel *SkipListNode::GetLevels() const { return levels_.get(); }

void SkipListNode::SetLevels(SkipListLevelArrUptr levels) {
  levels_ = std::move(levels);
}

SkipList::SkipList(uint16_t type) : type_(type) {
  header_ = std::make_shared<SkipListNode>(kSkipListMaxLevel);
}

SkipList::~SkipList() {}

bool SkipList::Insert(SkipListNodeUakArrUptr uaks) {
  std::vector<SkipListNodeSptr> update(kSkipListMaxLevel);
  size_t rank[kSkipListMaxLevel] = {0};

  SkipListNodeSptr x = header_;
  for (int i = level_ - 1; i >= 0; --i) {
    rank[i] = (i == level_ - 1) ? 0 : rank[i + 1];
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
    update[i]->GetLevels()[i].SetForward(x);

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

  // 添加到hash表里面
  uid_umap_[x->GetUid()] = x;

  return true;
}

bool SkipList::Update(SkipListNodeUakArrUptr uaks) {
  size_t key_size = GetKeySizeByType(type_);
  if (0 == key_size) {
    return false;
  }

  Delete(uaks[0]);
  Insert(std::move(uaks));

  return true;
}

bool SkipList::Delete(uint64_t uid) { return Delete(GetNodeByUid(uid)); }

uint64_t SkipList::GetScore(uint64_t uid) {
  auto node_sptr = GetNodeByUid(uid);
  if (nullptr != node_sptr) {
    return node_sptr->GetUaks()[1];
  }
  return 0;
}

size_t SkipList::GetRank(uint64_t uid) {
  auto node_sptr = GetNodeByUid(uid);
  if (nullptr != node_sptr) {
    size_t rank        = 0;
    SkipListNodeSptr x = header_;
    for (int i = level_ - 1; i >= 0; --i) {
      while (x->GetLevels()[i].GetForward() &&
             (CompUaks(x->GetLevels()[i].GetForward()->GetUaks(),
                       node_sptr->GetUaks()))) {
        rank += x->GetLevels()[i].GetSpan();
        x = x->GetLevels()[i].GetForward();
      }

      fmt::print("{}\n", rank);
      if (x != header_ && x->GetUid() == uid) {
        return rank;
      }
    }
  }
  return 0;
}

uint16_t SkipList::GetType() { return type_; }

void SkipList::PrintStorage() const {
  std::ostringstream os;
  fmt::print(os, "SkipList Info : \n");
  fmt::print(os, "SIZE: {} LEVEL: {}\n", length_, level_);

  SkipListNodeSptr x;
  for (int i = level_ - 1; i >= 0; --i) {
    fmt::print(os, "CUR LEVEL: {}\n", i);
    x = header_;
    while (x->GetLevels()[i].GetForward()) {
      fmt::print(os, "SPAN: {} UAKS {}-{} => ", x->GetLevels()[i].GetSpan(),
                 x->GetLevels()[i].GetForward()->GetUaks()[0],
                 x->GetLevels()[i].GetForward()->GetUaks()[1]);
      x = x->GetLevels()[i].GetForward();
    }
    fmt::print(os, "\n");
  }

  LOG_INFO("Rank Show : {}\n", os.str());
}

constexpr size_t SkipList::GetKeySizeByType(uint16_t type) {
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

SkipListNodeSptr SkipList::GetNodeByUid(uint64_t uid) {
  auto iter = uid_umap_.find(uid);
  if (uid_umap_.end() == iter) {
    return nullptr;
  }

  return iter->second;
}

bool SkipList::Delete(SkipListNodeSptr node_sptr) {
  if (nullptr == node_sptr) {
    return false;
  }

  std::vector<SkipListNodeSptr> update(kSkipListMaxLevel);
  SkipListNodeSptr x = header_;

  for (int i = level_ - 1; i >= 0; --i) {
    while (x->GetLevels()[i].GetForward() &&
           (CompUaks(x->GetLevels()[i].GetForward()->GetUaks(),
                     node_sptr->GetUaks()))) {
      x = x->GetLevels()[i].GetForward();
    }
    update[i] = x;
  }

  return Delete(std::move(node_sptr), std::move(update));
}

bool SkipList::Delete(SkipListNodeSptr node_sptr,
                      std::vector<SkipListNodeSptr> update) {
  if (nullptr == node_sptr || update.empty()) {
    return false;
  }

  for (int i = 0; i < level_; ++i) {
    if (update[i]->GetLevels()[i].GetForward() == node_sptr) {
      update[i]->GetLevels()[i].GetSpan() +=
          node_sptr->GetLevels()[i].GetSpan() - 1;
      update[i]->GetLevels()[i].SetForward(
          node_sptr->GetLevels()[i].GetForward());
    } else {
      update[i]->GetLevels()[i].GetSpan() -= 1;
    }
  }

  if (node_sptr->GetLevels()[0].GetForward()) {
    node_sptr->GetLevels()[0].GetForward()->SetBackward(
        node_sptr->GetBackward());
  } else {
    tail_ = node_sptr->GetBackward();
  }

  while (level_ > 1 &&
         nullptr == header_->GetLevels()[level_ - 1].GetForward()) {
    --level_;
  }

  --length_;

  uid_umap_.erase(node_sptr->GetUid());

  return true;
}

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

}  // namespace tpn
