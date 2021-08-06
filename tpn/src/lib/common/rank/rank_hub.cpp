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

#include "rank_hub.h"

#include "skiplist.h"
#include "utils.h"
#include "debug_hub.h"

namespace tpn {

namespace rank {

void RankHub::Init() {
  for (uint16_t type = EnumToUnderlyType(RankType::kRankTypeTest);
       type < EnumToUnderlyType(RankType::kRankTypeMax); ++type) {
    ranks_[type] = std::make_unique<RankList>(
        TransformRankType(static_cast<RankType>(type)));
  }
}

bool RankHub::UpdateRank(RankType type, uint64_t uid, uint64_t score,
                         uint64_t p1 /* = 0 */, uint64_t p2 /* = 0 */) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr,
             "rank not exist, type: {}, uid: {}, score: {}, p1: {}, p2: {}",
             type, uid, score, p1, p2);

  auto uaks_size = rank_ptr->GetUaksSize();
  TPN_ASSERT(uaks_size > 1,
             "rank not exist, type: {}, uid: {}, score: {}, p1: {}, p2: {}",
             type, uid, score, p1, p2);

  auto uaks = std::make_unique<uint64_t[]>(uaks_size);
  uaks[0]   = uid;
  uaks[1]   = score;
  if (uaks_size > 2) {
    uaks[2] = p1;
  }
  if (uaks_size > 3) {
    uaks[3] = p2;
  }

  return rank_ptr->Update(std::move(uaks));
}

bool RankHub::RemoveRank(RankType type, uint64_t uid) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, uid: {}", type, uid);

  return rank_ptr->Delete(uid);
}

uint64_t RankHub::GetScore(RankType type, uint64_t uid) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, uid: {}", type, uid);

  return rank_ptr->GetScore(uid);
}

size_t RankHub::GetRank(RankType type, uint64_t uid) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, uid: {}", type, uid);

  return rank_ptr->GetRank(uid);
}

size_t RankHub::GetRevRank(RankType type, uint64_t uid) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, uid: {}", type, uid);

  return rank_ptr->GetRevRank(uid);
}

uint64_t RankHub::GetUidByRank(RankType type, size_t rank) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rank: {}", type, rank);

  return rank_ptr->GetUidByRank(rank);
}

uint64_t RankHub::GetUidByRevRank(RankType type, size_t rank) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rev rank: {}", type, rank);

  return rank_ptr->GetUidByRevRank(rank);
}

std::vector<uint64_t> RankHub::GetRange(RankType type,
                                        size_t rank_start /* = 0 */,
                                        size_t rank_end /* = 0 */) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rank start: {} - end: {}",
             type, rank_start, rank_end);

  return std::move(rank_ptr->GetRange(rank_start, rank_end));
}

std::vector<uint64_t> RankHub::GetRevRange(RankType type,
                                           size_t rank_start /* = 0 */,
                                           size_t rank_end /* = 0 */) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rank start: {} - end: {}",
             type, rank_start, rank_end);

  return std::move(rank_ptr->GetRevRange(rank_start, rank_end));
}

std::vector<std::pair<uint64_t, uint64_t>> RankHub::GetRangeWithScore(
    RankType type, size_t rank_start /* = 0 */, size_t rank_end /* = 0 */) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rank start: {} - end: {}",
             type, rank_start, rank_end);

  return std::move(rank_ptr->GetRangeWithScore(rank_start, rank_end));
}

std::vector<std::pair<uint64_t, uint64_t>> RankHub::GetRevRangeWithScore(
    RankType type, size_t rank_start /* = 0 */, size_t rank_end /* = 0 */) {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}, rank start: {} - end: {}",
             type, rank_start, rank_end);

  return std::move(rank_ptr->GetRevRangeWithScore(rank_start, rank_end));
}

void RankHub::PrintStorage(RankType type) const {
  auto rank_ptr = ranks_[EnumToUnderlyType(type)].get();
  TPN_ASSERT(rank_ptr, "rank not exist, type: {}", type);

  return rank_ptr->PrintStorage();
}

TPN_SINGLETON_IMPL(RankHub)

}  // namespace rank

}  // namespace tpn
