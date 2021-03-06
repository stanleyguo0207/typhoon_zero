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

#include "../../test_include.h"

#include <ctime>

#include "fmt_wrap.h"

using namespace std;
using namespace tpn;

// debug_hub
#include "common.h"
#include "debug_hub.h"

//TEST_CASE("debug_hub", "[common]") {
//  TPN_ASSERT(3 < 5, "assert 3 < 5");
//  // TPN_ASSERT(3 > 5, "assert 3 > 5 maybe error : time{}", time(nullptr));
//}

// configmgr
#include "config.h"

//TEST_CASE("config", "[common]") {
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  auto host = g_config->GetStringDefault("host", "127.0.0.1");
//  cout << "host: " << host.c_str() << endl;
//
//  auto num_1 = g_config->GetI32Default("num_1", -1);
//  cout << "num_1: " << num_1 << endl;
//
//  auto num_2 = g_config->GetI64Default("num_2", 1);
//  cout << "num_2: " << num_2 << endl;
//
//  auto num_3 = g_config->GetU32Default("num_3", 2);
//  cout << "num_3: " << num_3 << endl;
//
//  auto num_4 = g_config->GetU64Default("num_4", 3);
//  cout << "num_4: " << num_4 << endl;
//
//  auto num_5 = g_config->GetU64Default("num_5", 4);
//  cout << "num_5: " << num_5 << endl;
//
//  auto float_1 = g_config->GetFloatDefault("float_1", 1.0);
//  cout << "float_1: " << float_1 << endl;
//
//  auto double_1 = g_config->GetDoubleDefault("double_1", -1.0);
//  cout << "double_1: " << double_1 << endl;
//}

// random_hub
#include "random_hub.h"

//TEST_CASE("random base", "[common]") {
//  fmt::print("Rand32          {}\n", Rand32());
//  fmt::print("RandI32         {}\n", RandI32());
//  fmt::print("RandU32         {}\n", RandU32());
//  fmt::print("RandFloat       {}\n", RandFloat());
//  fmt::print("RandDouble      {}\n", RandDouble());
//  fmt::print("RandMS          {}\n", RandMS());
//  fmt::print("RandTime        {}\n",
//             static_cast<uint32_t>(
//                 RandTime(MilliSeconds(300), MilliSeconds(500)).count()));
//  fmt::print("RandNorm        {}\n", RandNorm());
//  fmt::print("RandChance      {}\n", RandChance());
//  fmt::print("RollRhanceFloat {}\n", RollChanceFloat(80.5));
//  fmt::print("RollChanceI32   {}\n", RollChanceI32(30));
//}
//
//TEST_CASE("random weight", "[common]") {
//  const int kThousand10  = 10000;
//  const int kThousand100 = 100000;
//
//  double weights[] = {1000.0, 1500.0, 2000.0, 1500.0, 4000.0, 3000.0};
//  std::unordered_map<uint32_t, uint32_t> result_map;
//
//  for (int i = 0; i < kThousand10; ++i) {
//    auto n = RandU32Weighted(weights, std::size(weights));
//    ++result_map[n];
//  }
//
//  for (auto &&[n, count] : result_map) {
//    fmt::print("{}:    {}\t{}\n", n, count,
//               std::string(static_cast<int>(count * 100.0 / kThousand10), '*')
//                   .c_str());
//  }
//}

// containers
#include "containers.h"
using namespace tpn::containers;

//TEST_CASE("containers", "[common]") {
//  struct Item {
//    Item() = default;
//    Item(int id_in, double weight_in) : id(id_in), weight(weight_in) {}
//
//    int id{0};
//    double weight{0.0f};
//  };
//
//  std::vector<Item> bag;
//
//  for (int i = 0; i < 10; ++i) {
//    bag.emplace_back(i + 1, RandDouble(1000, 8000));
//  }
//
//  for (auto &&item : bag) {
//    fmt::print("bag item id:{} weight:{}\n", item.id, item.weight);
//  }
//
//  fmt::print("\n");
//
//  auto rand_iter = SelectRandomWeightContainerElementIterator(
//      bag, [](const auto &_la) { return _la.weight; });
//  if (rand_iter != bag.end()) {
//    fmt::print("weight rand item id:{} weight:{}\n", rand_iter->id,
//               rand_iter->weight);
//  }
//
//  RandomShuffe(bag);
//
//  for (auto &&item : bag) {
//    fmt::print("bag item id:{} weight:{}\n", item.id, item.weight);
//  }
//
//  fmt::print("\n");
//
//  auto &item = SelectRandomContainerElement(bag);
//  fmt::print("rand item id:{} weight:{}\n", item.id, item.weight);
//}

// byte_converter
#include "byte_converter.h"

//TEST_CASE("byte", "[common]") {
//  uint32_t data = 0x12345678;
//
//  fmt::print("{:#x}\n", data);
//  EndianRefMakeBig(data);
//  fmt::print("{:#x}\n", data);
//  EndianRefMakeBig(data);
//  EndianRefMakeLittle(data);
//  fmt::print("{:#x}\n", data);
//}

// utils
#include "utils.h"

//TEST_CASE("utf8", "[common]") {
//  std::string str = "hello 世界";
//  fmt::print("str {}\n", str);
//
//  std::string str2;
//  std::wstring wstr = L"";
//  if (Utf8ToWstr(str, wstr)) {
//    WstrToUtf8(wstr, str2);
//    fmt::print("wstr {}\n", str2);
//  }
//
//  size_t len = Utf8Length(str2);
//  if (len > 1) {
//    Utf8Truncate(str2, len - 1);
//    fmt::print("str2 truncate 5 {}\n", str2);
//  }
//}
//
//TEST_CASE("tokenize", "[common]") {
//  std::string str = "1-100-200;2-200-300;3-400-200";
//  auto tk1        = Tokenize(str, ";");
//  for (auto &&s : tk1) {
//    auto tk2 = Tokenize(s, "-");
//    fmt::print("{}\n", tk2);
//  }
//}
//
//TEST_CASE("tokenizer", "[common]") {
//  std::string str = "1-100-200;2-200-300;3-400-200";
//  auto tk1        = Tokenizer(str, ';');
//  for (auto &&s : tk1) {
//    auto tk2 = Tokenizer(s, '-');
//    fmt::print("{}\n", tk2);
//  }
//}

// filesystem
#include <filesystem>

namespace fs = std::filesystem;

//TEST_CASE("filesystem", "[common]") {
//  auto path_orgi = fs::path{"/project/typhoon_test/filesystem/nothing"};
//  path_orgi.make_preferred();
//  auto path_file = fs::absolute(path_orgi);
//
//  fmt::print("root_name: {}\n", path_file.root_name());
//  fmt::print("root_directory: {}\n", path_file.root_directory());
//  fmt::print("root_path: {}\n", path_file.root_path());
//  fmt::print("relative_path: {}\n", path_file.relative_path());
//  fmt::print("parent_path: {}\n", path_file.parent_path());
//  fmt::print("filename: {}\n", path_file.filename());
//  fmt::print("stem: {}\n", path_file.stem());
//  fmt::print("extension: {}\n", path_file.extension());
//}

// file_helper
#include "file_helper.h"

//TEST_CASE("file_helper", "[common]") {
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  std::string test_file = "file_helper/out/test_file.log";
//  FileHelper helper;
//  try {
//    helper.Open(test_file);
//    fmt::print("open {} success.\n", helper.GetPath());
//    FmtMemoryBuf buf;
//    fmt::format_to(buf, "{}\n", "this is test file_helper.");
//    helper.Write(buf);
//    helper.Flush();
//    fmt::print("{} file size {}.\n", helper.GetPath(), helper.Size());
//    helper.Close();
//  } catch (const std::exception &ex) {
//    fmt::print("open file failed {}.\n", ex.what());
//  }
//
//  auto &&[parent, filename] = FileHelper::SplitByExtension(test_file);
//
//  fmt::print("file parent_path: {}.\n", parent);
//  fmt::print("file extension: {}.\n", filename);
//
//  auto path_orgi = fs::path{test_file};
//  path_orgi.make_preferred();
//  auto path_file = fs::absolute(path_orgi);
//  fmt::print("parent_path: {}\n", path_file.parent_path());
//  fmt::print("filename: {}\n", path_file.filename());
//  fmt::print("stem: {}\n", path_file.stem());
//  fmt::print("extension: {}\n", path_file.extension());
//}

// // buffer_warp
// #include <cstring>
//
// #include "buffer_wrap.h"
// #include "linear_buffer.h"
//
// TEST_CASE("buffer_wrap", "[common]") {
//   BufferWrap<LinearBuffer> buffer(4096, 1024);
//   string str = "hello world!";
//   int len    = str.length() + 1;
//   std::memcpy(buffer.prepare(str.length() + 1).data(), str.data(), len);
//   buffer.commit(len);
//
//   std::string_view strv(
//       reinterpret_cast<std::string_view::const_pointer>(buffer.data().data()),
//       buffer.data().size());
//   fmt::print("buffer data: {} size: {}\n", strv, buffer.data().size());
//   buffer.consume(buffer.data().size());
//   fmt::print("buffer size: {}\n", buffer.data().size());
// }

// byte_buffer
#include "byte_buffer.h"
#include "platform.h"
#include "chrono_wrap.h"

//TEST_CASE("byte_buffer", "[common]") {
//  ByteBuffer bbuffer;
//  auto now = SystemClock::now();
//  bbuffer.AppendPackedTime(SystemClock::to_time_t(now));
//  uint32_t now_time = bbuffer.ReadPackedTime();
//  fmt::print("now time: {}\n", now_time);
//}

// TEST_CASE("byte_buffer2", "[common]") {
//   // BufferWrap<LinearBuffer> buffer(4096, 1024);
//   BufferWrap<asio::streambuf> buffer(4096, 1024);
//   string str = "hello world!";
//   int len    = str.length() + 1;
//   std::memcpy(buffer.prepare(str.length() + 1).data(), str.data(), len);
//   buffer.commit(len);
//   std::string_view strv(
//       reinterpret_cast<std::string_view::const_pointer>(buffer.data().data()),
//       buffer.data().size());
//   fmt::print("buffer data: {} size: {}\n", strv, buffer.data().size());
//
//   // ByteBuffer bbuffer(buffer);
//   // std::string b_str;
//   // bbuffer >> b_str;
//   // fmt::print("b_str: {}\n", b_str);
//   // fmt::print("buffer size: {}\n", buffer.data().size());
//   // auto now = SystemClock::now();
//   // bbuffer.AppendPackedTime(SystemClock::to_time_t(now));
//   // uint32_t now_time = bbuffer.ReadPackedTime();
//   // fmt::print("now time: {}\n", now_time);
// }

#include "banner.h"
//TEST_CASE("banner", "[common]") {
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  BannerShow("test common banner", [&]() {});
//}

#include "log.h"
//
//#include "skiplist.h"
//TEST_CASE("skiplist", "[common]") {
//  using namespace tpn::rank;
//
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  tpn::log::Init();
//  std::shared_ptr<void> log_handle(nullptr,
//                                   [](void *) { tpn::log::Shutdown(); });
//
//  uint16_t sp_list_type = TransformRankType(RankType::kRankTypeTest);
//  SkipList sp_list(sp_list_type);
//
//  size_t uaks_size = GetSizeByRankKeyType(GetRankKeyType(sp_list_type));
//
//  for (int i = 0; i < 20000; ++i) {
//    auto data = std::make_unique<uint64_t[]>(uaks_size);
//    data[0]   = 1001 + i;
//    data[1]   = RandU32();
//    sp_list.Insert(std::move(data));
//  }
//
//  sp_list.PrintStorage();
//
//  LOG_DEBUG("found 1005 score: {} rank: {} revrank: {}", sp_list.GetScore(1005),
//            sp_list.GetRank(1005), sp_list.GetRevRank(1005));
//
//  {
//    auto data = std::make_unique<uint64_t[]>(uaks_size);
//    data[0]   = 1005;
//    data[1]   = RandU32();
//    sp_list.Update(std::move(data));
//
//    sp_list.PrintStorage();
//
//    LOG_DEBUG("found 1005 score: {} rank: {} revrank: {}",
//              sp_list.GetScore(1005), sp_list.GetRank(1005),
//              sp_list.GetRevRank(1005));
//  }
//
//  {
//    LOG_DEBUG("rank1: {}, rank2: {}, rank3: {}", sp_list.GetUidByRank(1),
//              sp_list.GetUidByRank(2), sp_list.GetUidByRank(700));
//
//    LOG_DEBUG("revrank1: {}, revrank2: {}, revrank3: {}",
//              sp_list.GetUidByRevRank(1), sp_list.GetUidByRevRank(2),
//              sp_list.GetUidByRevRank(800));
//
//    LOG_DEBUG("range 1-10, {}", sp_list.GetRange(1, 10));
//    LOG_DEBUG("rev range 1-10, {}", sp_list.GetRevRange(1, 10));
//  }
//
//  for (int i = 0; i < 20000; ++i) {
//    sp_list.Delete(1001 + i);
//    // sp_list.PrintStorage();
//    // fmt::print("\n");
//  }
//
//  {
//    auto data = std::make_unique<uint64_t[]>(uaks_size);
//    data[0]   = 1010;
//    data[1]   = RandU32();
//    sp_list.Update(std::move(data));
//
//    sp_list.PrintStorage();
//
//    LOG_DEBUG("found 1010 score: {} rank: {} revrank: {}",
//              sp_list.GetScore(1010), sp_list.GetRank(1010),
//              sp_list.GetRevRank(1010));
//  }
//
//  LOG_INFO("rank test end");
//
//  std::this_thread::sleep_for(2s);
//}
//
//#include "rank_hub.h"
//TEST_CASE("rank", "[common]") {
//  using namespace tpn::rank;
//
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  tpn::log::Init();
//  std::shared_ptr<void> log_handle(nullptr,
//                                   [](void *) { tpn::log::Shutdown(); });
//
//  g_rank_hub->Init();
//
//  LOG_INFO("rank test start");
//  LOG_INFO("rank test update start");
//
//  constexpr int test_max = 20;
//  RankType type          = RankType::kRankTypeTest;
//
//  // 1000000 百分级别测试通过
//  for (int i = 0; i < test_max; ++i) {
//    //g_rank_hub->UpdateRank(type, 1000001 + i, 400);
//    g_rank_hub->UpdateRank(type, 1000001 + i, Rand32());
//  }
//
//  LOG_INFO("rank test update end");
//
//  g_rank_hub->PrintStorage(type);
//
//  LOG_INFO("rank test search start");
//
//  for (int i = 0; i < test_max; ++i) {
//    auto uid = 1000001 + i;
//    LOG_DEBUG("found {} score: {} rank: {} revrank: {}", uid,
//              g_rank_hub->GetScore(type, uid), g_rank_hub->GetRank(type, uid),
//              g_rank_hub->GetRevRank(type, uid));
//  }
//
//  LOG_INFO("rank test search end");
//
//  LOG_INFO("rank test update start");
//
//  for (int i = 0; i < test_max; ++i) {
//    g_rank_hub->UpdateRank(type, 1000001 + i, Rand32());
//  }
//
//  g_rank_hub->PrintStorage(type);
//
//  LOG_INFO("rank test update end");
//
//  LOG_DEBUG("range: {}", g_rank_hub->GetRange(type));
//  LOG_DEBUG("range with score: {}", g_rank_hub->GetRangeWithScore(type));
//
//  LOG_DEBUG("rev range: {}", g_rank_hub->GetRevRange(type));
//  LOG_DEBUG("rev range with score: {}", g_rank_hub->GetRevRangeWithScore(type));
//
//  LOG_INFO("rank test remove start");
//
//  for (int i = 0; i < test_max; ++i) {
//    g_rank_hub->RemoveRank(type, 1000001 + i);
//  }
//
//  g_rank_hub->PrintStorage(type);
//
//  LOG_INFO("rank test remove end");
//
//  LOG_INFO("rank test end");
//
//  std::this_thread::sleep_for(5s);
//}
//
//TEST_CASE("rank_bench", "[common]") {
//  using namespace tpn::rank;
//
//#ifndef _TPN_COMMON_CONFIG_TEST_FILE
//#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
//#endif
//
//  if (auto error = g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {})) {
//    cout << *error << endl;
//    return;
//  }
//
//  tpn::log::Init();
//  std::shared_ptr<void> log_handle(nullptr,
//                                   [](void *) { tpn::log::Shutdown(); });
//
//  g_rank_hub->Init();
//
//  LOG_INFO("rank test start");
//  LOG_INFO("rank test update start");
//
//  /// 一百万
//  constexpr int test_max = 1000000;
//
//  for (int i = 0; i < test_max; ++i) {
//    g_rank_hub->UpdateRank(RankType::kRankTypeSA, 1000001 + i, Rand32());
//  }
//
//  LOG_INFO("rank test update end");
//
//  LOG_INFO("rank test search start");
//
//  for (int i = 0; i < test_max; ++i) {
//    auto uid = 1000001 + i;
//    g_rank_hub->GetScore(RankType::kRankTypeSA, uid);
//    g_rank_hub->GetRank(RankType::kRankTypeSA, uid);
//    g_rank_hub->GetRevRank(RankType::kRankTypeSA, uid);
//  }
//
//  // 非法数据查询
//  for (int i = 0; i < test_max / 2; ++i) {
//    auto uid = 2000001 + i;
//    g_rank_hub->GetScore(RankType::kRankTypeSA, uid);
//    g_rank_hub->GetRank(RankType::kRankTypeSA, uid);
//    g_rank_hub->GetRevRank(RankType::kRankTypeSA, uid);
//  }
//
//  LOG_INFO("rank test search end");
//
//  LOG_INFO("rank test update start");
//
//  for (int i = 0; i < test_max; ++i) {
//    g_rank_hub->UpdateRank(RankType::kRankTypeSA, 1000001 + i, Rand32());
//  }
//
//  LOG_INFO("rank test update end");
//
//  LOG_DEBUG("range: {}", g_rank_hub->GetRange(RankType::kRankTypeSA, 1, 100));
//  LOG_DEBUG("range with score: {}",
//            g_rank_hub->GetRangeWithScore(RankType::kRankTypeSA, 1, 100));
//
//  LOG_DEBUG("rev range: {}",
//            g_rank_hub->GetRevRange(RankType::kRankTypeSA, 1, 100));
//  LOG_DEBUG("rev range with score: {}",
//            g_rank_hub->GetRevRangeWithScore(RankType::kRankTypeSA, 1, 100));
//
//  LOG_INFO("rank test remove start");
//
//  for (int i = 0; i < test_max; ++i) {
//    g_rank_hub->RemoveRank(RankType::kRankTypeTest, 1000001 + i);
//  }
//
//  g_rank_hub->PrintStorage(RankType::kRankTypeTest);
//
//  LOG_INFO("rank test remove end");
//
//  LOG_INFO("rank test end");
//
//  std::this_thread::sleep_for(5s);
//}

#include "linked_list.h"

struct TestLinkedListNode {
  int32_t desc_num_{0};
  LinkedListElement l1_;
  std::string desc_;
  LinkedListElement l2_;
  LinkedListElement l3_;
};

struct TestLinkedListHead {
  std::string desc_;
  int32_t desc_num_{0};
  LinkedListHead lh1_;
  LinkedListHead lh2_;
  LinkedListHead lh3_;
};

TEST_CASE("linked_list", "[common]") {
  TestLinkedListHead list_head;
  list_head.desc_.assign("HEAD");
  list_head.desc_num_ = 10000;

  TestLinkedListNode node1;
  node1.desc_.assign("NODE 20001");
  node1.desc_num_ = 20001;

  list_head.lh1_.InsertFirst(&node1.l1_);
  list_head.lh2_.InsertFirst(&node1.l2_);
  list_head.lh3_.InsertFirst(&node1.l3_);

  TestLinkedListNode node2;
  node2.desc_.assign("NODE 20002");
  node2.desc_num_ = 20002;

  list_head.lh1_.InsertLast(&node2.l1_);
  list_head.lh2_.InsertLast(&node2.l2_);
  list_head.lh3_.InsertLast(&node2.l3_);

  TestLinkedListNode node3;
  node3.desc_.assign("NODE 20003");
  node3.desc_num_ = 20003;

  list_head.lh1_.InsertFirst(&node3.l1_);
  list_head.lh2_.InsertFirst(&node3.l2_);
  list_head.lh3_.InsertFirst(&node3.l3_);

  TestLinkedListNode *node_ptr = nullptr;

  for (auto iter = LinkedListHead::iterator(list_head.lh1_.GetFirst());
       iter != LinkedListHead::iterator(nullptr); ++iter) {
    node_ptr = TPN_CONTAINER_OF(&(*iter), TestLinkedListNode, l1_);
    fmt::print("{} -- {}\n", node_ptr->desc_, node_ptr->desc_num_);
  }

  LinkedListElement *elem_ptr = nullptr;

  while (elem_ptr = list_head.lh1_.GetFirst()) {
    elem_ptr->Delink();
  }

  while (elem_ptr = list_head.lh2_.GetFirst()) {
    elem_ptr->Delink();
  }

  while (elem_ptr = list_head.lh3_.GetFirst()) {
    elem_ptr->Delink();
  }

  ;
}
