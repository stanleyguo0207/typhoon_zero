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
#include "debug_hub.h"

namespace tpn {

namespace rank {

namespace {

constexpr size_t kSkipListMaxLevel    = 32;    ///< 2 ^ 64 元素
constexpr double kSkipListProbability = 0.25;  ///< 随机层数概率

/// 获取节点层数
/// 不能超过最高层数限制
///  @return 返回随机后的节点层数
TPN_INLINE int GetRandomLevel() {
  int level = 1;
  while ((RandI32() & 0xFFFF) < (kSkipListProbability * 0xFFFF)) {
    ++level;
  }

  return (level < kSkipListMaxLevel) ? level : kSkipListMaxLevel;
}

}  // namespace

SkipListLevel::SkipListLevel() {}

SkipListLevel::~SkipListLevel() {}

SkipListNodeSptr SkipListLevel::GetForward() { return forward_; }

SkipListNodeSptr SkipListLevel::GetForward() const { return forward_; }

void SkipListLevel::SetForward(SkipListNodeSptr node_sptr) {
  forward_ = std::move(node_sptr);
}

size_t SkipListLevel::GetSpan() { return span_; }

const size_t SkipListLevel::GetSpan() const { return span_; }

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

const uint64_t SkipListNode::GetScore() const { return uaks_[1]; }

const uint64_t SkipListNode::GetP1() const { return uaks_[2]; }

const uint64_t SkipListNode::GetP2() const { return uaks_[3]; }

uint64_t *SkipListNode::GetUaks() { return uaks_.get(); }

const uint64_t *SkipListNode::GetUaks() const { return uaks_.get(); }

void SkipListNode::SetUaks(SkipListNodeUakArrUptr uaks) {
  uaks_ = std::move(uaks);
}

SkipListNodeSptr SkipListNode::GetBackward() { return backward_; }

const SkipListNodeSptr SkipListNode::GetBackward() const { return backward_; }

void SkipListNode::SetBackward(SkipListNodeSptr node_sptr) {
  backward_ = std::move(node_sptr);
}

SkipListLevel *SkipListNode::GetLevels() { return levels_.get(); }

const SkipListLevel *SkipListNode::GetLevels() const { return levels_.get(); }

void SkipListNode::SetLevels(SkipListLevelArrUptr levels) {
  levels_ = std::move(levels);
}

SkipList::SkipList(uint16_t type)
    : type_(type), uaks_size_(GetSizeByRankKeyType(GetRankKeyType(type_))) {
  TPN_ASSERT(0 != uaks_size_, "uaks_size error: {} type: {}", uaks_size_,
             type_);
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
    update[i]->GetLevels()[i].SetSpan(update[i]->GetLevels()[i].GetSpan() + 1);
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
  Delete(uaks[0]);
  Insert(std::move(uaks));

  return true;
}

bool SkipList::Delete(uint64_t uid) { return Delete(GetNodeByUid(uid)); }

uint64_t SkipList::GetScore(uint64_t uid) {
  return GetScore(GetNodeByUid(uid));
}

size_t SkipList::GetRank(uint64_t uid) {
  auto node_sptr = GetNodeByUid(uid);
  if (nullptr != node_sptr) {
    size_t rank        = 0;
    SkipListNodeSptr x = header_;
    for (int i = level_ - 1; i >= 0; --i) {
      while (x->GetLevels()[i].GetForward() &&
             (uid == x->GetLevels()[i].GetForward()->GetUid() ||
              CompUaks(x->GetLevels()[i].GetForward()->GetUaks(),
                       node_sptr->GetUaks()))) {
        rank += x->GetLevels()[i].GetSpan();
        x = x->GetLevels()[i].GetForward();
      }

      if (x == node_sptr) {
        return rank;
      }
    }
  }
  return 0;
}

size_t SkipList::GetRevRank(uint64_t uid) {
  auto rank = GetRank(uid);
  if (0 == rank) {
    return 0;
  }

  return length_ - rank + 1;
}

uint64_t SkipList::GetUidByRank(size_t rank) {
  return GetUid(GetNodeByRank(rank));
}

uint64_t SkipList::GetUidByRevRank(size_t rank) {
  return GetUid(GetNodeByRank(length_ + 1 - rank));
}

std::vector<uint64_t> SkipList::GetRange(size_t rank_start /* = 0 */,
                                         size_t rank_end /* = 0 */) {
  return std::move(GetRangeWithFlag(rank_start, rank_end));
}

std::vector<uint64_t> SkipList::GetRevRange(size_t rank_start /* = 0 */,
                                            size_t rank_end /* = 0 */) {
  return std::move(GetRangeWithFlag(rank_start, rank_end, true));
}

std::vector<std::pair<uint64_t, uint64_t>> SkipList::GetRangeWithScore(
    size_t rank_start /* = 0 */, size_t rank_end /* = 0 */) {
  return std::move(GetRangeWithScoreWithFlag(rank_start, rank_end));
}

std::vector<std::pair<uint64_t, uint64_t>> SkipList::GetRevRangeWithScore(
    size_t rank_start /* = 0 */, size_t rank_end /* = 0 */) {
  return std::move(GetRangeWithScoreWithFlag(rank_start, rank_end, true));
}

uint16_t SkipList::GetType() const { return type_; }

size_t SkipList::GetUaksSize() const { return uaks_size_; }

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

  fmt::print(os, "TAIL: UAKS {}-{}", tail_ ? tail_->GetUaks()[0] : 0,
             tail_ ? tail_->GetUaks()[1] : 0);

  LOG_INFO("Rank Show : {}\n", os.str());
}

SkipListNodeSptr SkipList::GetNodeByUid(uint64_t uid) {
  auto iter = uid_umap_.find(uid);
  if (uid_umap_.end() == iter) {
    return nullptr;
  }

  return iter->second;
}

SkipListNodeSptr SkipList::GetNodeByRank(size_t rank) {
  if (0 == rank) {
    return nullptr;
  }

  SkipListNodeSptr x = header_;
  size_t traversed   = 0;

  for (int i = level_ - 1; i >= 0; --i) {
    while (x->GetLevels()[i].GetForward() &&
           (traversed + x->GetLevels()[i].GetSpan()) <= rank) {
      traversed += x->GetLevels()[i].GetSpan();
      x = x->GetLevels()[i].GetForward();
    }

    if (traversed == rank) {
      return x;
    }
  }

  return nullptr;
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
      update[i]->GetLevels()[i].SetSpan(update[i]->GetLevels()[i].GetSpan() +
                                        node_sptr->GetLevels()[i].GetSpan() -
                                        1);

      update[i]->GetLevels()[i].SetForward(
          node_sptr->GetLevels()[i].GetForward());
    } else {
      update[i]->GetLevels()[i].SetSpan(update[i]->GetLevels()[i].GetSpan() -
                                        1);
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

  // 检查底层结构是否发生错误 这个函数很影响性能
  // CheckLength();

  return true;
}

std::vector<uint64_t> SkipList::GetRangeWithFlag(size_t rank_start /* = 0 */,
                                                 size_t rank_end /* = 0 */,
                                                 bool reverse /* = false */) {
  if (0 == rank_start || rank_start > length_) {
    rank_start = 1;
  }

  if (0 == rank_end || rank_end > length_) {
    rank_end = length_;
  }

  if (rank_end < rank_start) {
    rank_end = rank_start;
  }

  size_t range = rank_end - rank_start + 1;

  std::vector<uint64_t> ans;

  SkipListNodeSptr x = reverse ? GetNodeByRank(length_ + 1 - rank_start)
                               : GetNodeByRank(rank_start);
  while (range--) {
    ans.emplace_back(GetUid(x));
    x = reverse ? x->GetBackward() : x->GetLevels()[0].GetForward();
  }

  return std::move(ans);
}

std::vector<std::pair<uint64_t, uint64_t>> SkipList::GetRangeWithScoreWithFlag(
    size_t rank_start /* = 0 */, size_t rank_end /* = 0 */,
    bool reverse /* = false */) {
  if (0 == rank_start || rank_start > length_) {
    rank_start = 1;
  }

  if (0 == rank_end || rank_end > length_) {
    rank_end = length_;
  }

  if (rank_end < rank_start) {
    rank_end = rank_start;
  }

  size_t range = rank_end - rank_start + 1;

  std::vector<std::pair<uint64_t, uint64_t>> ans;

  SkipListNodeSptr x = reverse ? GetNodeByRank(length_ + 1 - rank_start)
                               : GetNodeByRank(rank_start);
  while (range--) {
    ans.emplace_back(GetUid(x), GetScore(x));
    x = reverse ? x->GetBackward() : x->GetLevels()[0].GetForward();
  }

  return std::move(ans);
}

const uint64_t SkipList::GetUid(SkipListNodeSptr node_sptr) const {
  return nullptr == node_sptr ? 0 : (uaks_size_ > 0 ? node_sptr->GetUid() : 0);
}

const uint64_t SkipList::GetScore(SkipListNodeSptr node_sptr) const {
  return nullptr == node_sptr ? 0
                              : (uaks_size_ > 1 ? node_sptr->GetScore() : 0);
}

const uint64_t SkipList::GetP1(SkipListNodeSptr node_sptr) const {
  return nullptr == node_sptr ? 0 : (uaks_size_ > 2 ? node_sptr->GetP1() : 0);
}

const uint64_t SkipList::GetP2(SkipListNodeSptr node_sptr) const {
  return nullptr == node_sptr ? 0 : (uaks_size_ > 3 ? node_sptr->GetP2() : 0);
}

bool SkipList::CompUaks(uint64_t left[], uint64_t right[]) {
  uint8_t order = GetRankKeyOrderType(type_);
  uint8_t mask  = 0x1;
  for (size_t i = 1; i < uaks_size_; ++i) {
    if (mask & order) {  // desc
      if (left[i] > right[i]) {
        return true;
      } else if (left[i] < right[i]) {
        return false;
      }
    } else {  // asc
      if (left[i] < right[i]) {
        return true;
      } else if (left[i] > right[i]) {
        return false;
      }
    }

    mask <<= 1;
  }

  return left[0] < right[0];
}

void SkipList::CheckLength() {
#if defined(TPN_DEBUG)
  size_t uid_size = uid_umap_.size();
  TPN_ASSERT(uid_size == length_, "uid map size: {} != length: {}", uid_size,
             length_);

  size_t real_size = 0;
  auto x           = header_;
  while (x->GetLevels()[0].GetForward()) {
    ++real_size;
    x = x->GetLevels()[0].GetForward();
  }

  TPN_ASSERT(real_size == uid_size, "uid map size: {} != real_size: {}",
             uid_size, real_size);
#endif
}

}  // namespace rank

}  // namespace tpn
