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

TEST_CASE("debug_hub", "[common]") {
  TPN_ASSERT(3 < 5, "assert 3 < 5");
  // TPN_ASSERT(3 > 5, "assert 3 > 5 maybe error : time{}", time(nullptr));
}

// configmgr
#include "config.h"

TEST_CASE("config", "[common]") {
#ifndef _TPN_COMMON_CONFIG_TEST_FILE
#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
#endif

  std::string error;
  g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {}, error);
  if (!error.empty()) {
    cout << error.c_str() << endl;
    return;
  }

  auto host = g_config->GetStringDefault("host", "127.0.0.1");
  cout << "host: " << host.c_str() << endl;

  auto num_1 = g_config->GetI32Default("num_1", -1);
  cout << "num_1: " << num_1 << endl;

  auto num_2 = g_config->GetI64Default("num_2", 1);
  cout << "num_2: " << num_2 << endl;

  auto num_3 = g_config->GetU32Default("num_3", 2);
  cout << "num_3: " << num_3 << endl;

  auto num_4 = g_config->GetU64Default("num_4", 3);
  cout << "num_4: " << num_4 << endl;

  auto num_5 = g_config->GetU64Default("num_5", 4);
  cout << "num_5: " << num_5 << endl;

  auto float_1 = g_config->GetFloatDefault("float_1", 1.0);
  cout << "float_1: " << float_1 << endl;

  auto double_1 = g_config->GetDoubleDefault("double_1", -1.0);
  cout << "double_1: " << double_1 << endl;
}

// random_hub
#include "random_hub.h"

TEST_CASE("random base", "[common]") {
  fmt::print("Rand32          {}\n", Rand32());
  fmt::print("RandI32         {}\n", RandI32());
  fmt::print("RandU32         {}\n", RandU32());
  fmt::print("RandFloat       {}\n", RandFloat());
  fmt::print("RandDouble      {}\n", RandDouble());
  fmt::print("RandMS          {}\n", RandMS());
  fmt::print("RandTime        {}\n",
             static_cast<uint32_t>(
                 RandTime(MilliSeconds(300), MilliSeconds(500)).count()));
  fmt::print("RandNorm        {}\n", RandNorm());
  fmt::print("RandChance      {}\n", RandChance());
  fmt::print("RollRhanceFloat {}\n", RollChanceFloat(80.5));
  fmt::print("RollChanceI32   {}\n", RollChanceI32(30));
}

TEST_CASE("random weight", "[common]") {
  const int kThousand10  = 10000;
  const int kThousand100 = 100000;

  double weights[] = {1000.0, 1500.0, 2000.0, 1500.0, 4000.0, 3000.0};
  std::unordered_map<uint32_t, uint32_t> result_map;

  for (int i = 0; i < kThousand10; ++i) {
    auto n = RandU32Weighted(weights, std::size(weights));
    ++result_map[n];
  }

  for (auto &&[n, count] : result_map) {
    fmt::print("{}:    {}\t{}\n", n, count,
               std::string(static_cast<int>(count * 100.0 / kThousand10), '*')
                   .c_str());
  }
}

// containers
#include "containers.h"
using namespace tpn::containers;

TEST_CASE("containers", "[common]") {
  struct Item {
    Item() = default;
    Item(int id_in, double weight_in) : id(id_in), weight(weight_in) {}

    int id{0};
    double weight{0.0f};
  };

  std::vector<Item> bag;

  for (int i = 0; i < 10; ++i) {
    bag.emplace_back(i + 1, RandDouble(1000, 8000));
  }

  for (auto &&item : bag) {
    fmt::print("bag item id:{} weight:{}\n", item.id, item.weight);
  }

  fmt::print("\n");

  auto rand_iter = SelectRandomWeightContainerElementIterator(
      bag, [](const auto &_la) { return _la.weight; });
  if (rand_iter != bag.end()) {
    fmt::print("weight rand item id:{} weight:{}\n", rand_iter->id,
               rand_iter->weight);
  }

  RandomShuffe(bag);

  for (auto &&item : bag) {
    fmt::print("bag item id:{} weight:{}\n", item.id, item.weight);
  }

  fmt::print("\n");

  auto &item = SelectRandomContainerElement(bag);
  fmt::print("rand item id:{} weight:{}\n", item.id, item.weight);
}

// byte_converter
#include "byte_converter.h"

TEST_CASE("byte", "[common]") {
  uint32_t data = 0x12345678;

  fmt::print("{:#x}\n", data);
  EndianRefMakeBig(data);
  fmt::print("{:#x}\n", data);
  EndianRefMakeBig(data);
  EndianRefMakeLittle(data);
  fmt::print("{:#x}\n", data);
}

// utils
#include "utils.h"

TEST_CASE("utf8", "[common]") {
  std::string str = "hello 世界";
  fmt::print("str {}\n", str);

  std::string str2;
  std::wstring wstr = L"";
  if (Utf8ToWstr(str, wstr)) {
    WstrToUtf8(wstr, str2);
    fmt::print("wstr {}\n", str2);
  }

  size_t len = Utf8Length(str2);
  if (len > 1) {
    Utf8Truncate(str2, len - 1);
    fmt::print("str2 truncate 5 {}\n", str2);
  }
}

TEST_CASE("tokenize", "[common]") {
  std::string str = "1-100-200;2-200-300;3-400-200";
  auto tk1        = Tokenize(str, ";");
  for (auto &&s : tk1) {
    auto tk2 = Tokenize(s, "-");
    fmt::print("{}\n", tk2);
  }
}

TEST_CASE("tokenizer", "[common]") {
  std::string str = "1-100-200;2-200-300;3-400-200";
  auto tk1        = Tokenizer(str, ';');
  for (auto &&s : tk1) {
    auto tk2 = Tokenizer(s, '-');
    fmt::print("{}\n", tk2);
  }
}

// filesystem
#include <filesystem>

namespace fs = std::filesystem;

TEST_CASE("filesystem", "[common]") {
  auto path_orgi = fs::path{"/project/typhoon_test/filesystem/nothing"};
  path_orgi.make_preferred();
  auto path_file = fs::absolute(path_orgi);

  fmt::print("root_name: {}\n", path_file.root_name());
  fmt::print("root_directory: {}\n", path_file.root_directory());
  fmt::print("root_path: {}\n", path_file.root_path());
  fmt::print("relative_path: {}\n", path_file.relative_path());
  fmt::print("parent_path: {}\n", path_file.parent_path());
  fmt::print("filename: {}\n", path_file.filename());
  fmt::print("stem: {}\n", path_file.stem());
  fmt::print("extension: {}\n", path_file.extension());
}

// file_helper
#include "file_helper.h"

TEST_CASE("file_helper", "[common]") {
#ifndef _TPN_COMMON_CONFIG_TEST_FILE
#  define _TPN_COMMON_CONFIG_TEST_FILE "config_common_test.json"
#endif

  std::string error;
  g_config->Load(_TPN_COMMON_CONFIG_TEST_FILE, {}, error);
  if (!error.empty()) {
    cout << error.c_str() << endl;
    return;
  }

  std::string test_file = "file_helper/out/test_file.log";
  FileHelper helper;
  try {
    helper.Open(test_file);
    fmt::print("open {} success.\n", helper.GetPath());
    FmtMemoryBuf buf;
    fmt::format_to(buf, "{}\n", "this is test file_helper.");
    helper.Write(buf);
    helper.Flush();
    fmt::print("{} file size {}.\n", helper.GetPath(), helper.Size());
    helper.Close();
  } catch (const std::exception &ex) {
    fmt::print("open file failed {}.\n", ex.what());
  }

  auto &&[parent, filename] = FileHelper::SplitByExtension(test_file);

  fmt::print("file parent_path: {}.\n", parent);
  fmt::print("file extension: {}.\n", filename);

  auto path_orgi = fs::path{test_file};
  path_orgi.make_preferred();
  auto path_file = fs::absolute(path_orgi);
  fmt::print("parent_path: {}\n", path_file.parent_path());
  fmt::print("filename: {}\n", path_file.filename());
  fmt::print("stem: {}\n", path_file.stem());
  fmt::print("extension: {}\n", path_file.extension());
}

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

TEST_CASE("byte_buffer", "[common]") {
  ByteBuffer bbuffer;
  auto now = SystemClock::now();
  bbuffer.AppendPackedTime(SystemClock::to_time_t(now));
  uint32_t now_time = bbuffer.ReadPackedTime();
  fmt::print("now time: {}\n", now_time);
}

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
