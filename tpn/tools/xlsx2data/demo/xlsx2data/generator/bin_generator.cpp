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


#include "bin_generator.h"

#include <fstream>
#include <filesystem>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/util/json_util.h>
#include <rapidjson/document.h>
#include <rapidjson/writer.h>
#include <rapidjson/filereadstream.h>

#include "log.h"
#include "config.h"
#include "data_hub.pb.h"
#include "generator_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

BinGenerator::BinGenerator() {}

BinGenerator::~BinGenerator() {}

bool BinGenerator::Generate() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;
  std::shared_ptr<void> protobuf_handle(
    nullptr, [](void *) { google::protobuf::ShutdownProtobufLibrary(); });

  std::string json_file_path = fmt::format(
    "{}/{}.json", g_config->GetStringDefault("xlsx_json_dir", "xlsx2data/json"),
    g_xlsx2data_generator->GetFilePrefix());

  std::string bin_file_path = fmt::format(
    "{}/{}.bin", g_config->GetStringDefault("xlsx_bin_dir", "xlsx2data/bin"),
    g_xlsx2data_generator->GetFilePrefix());

  auto json_path = fs::path(json_file_path);
  json_path.make_preferred();
  auto json_file = fs::absolute(json_path);

  auto bin_path = fs::path(bin_file_path);
  bin_path.make_preferred();
  auto bin_file = fs::absolute(bin_path);
  if (!fs::exists(bin_file.parent_path())) {
    fs::create_directories(bin_file.parent_path());
  }
  try {
#if (TPN_COMPILER == TPN_COMPILER_MSVC)
    const char *mask = "rb";
#else
    const char *mask = "r";
#endif
    auto *fp  = std::fopen(json_file.generic_string().c_str(), mask);
    auto len  = fs::file_size(json_path);
    char *buf = new char[len + 1];
    rapidjson::FileReadStream is(fp, buf, len + 1);
    rapidjson::Document d;
    d.ParseStream<rapidjson::kParseCommentsFlag>(is);
    if (d.HasParseError()) {
      LOG_ERROR("parse json error, {}", d.GetParseError());
      return false;
    }

    rapidjson::StringBuffer buffer;
    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
    d.Accept(writer);

    data::DataHubMap data_map;
    auto status = ::google::protobuf::util::JsonStringToMessage(buffer.GetString(), &data_map);
    if (!status.ok()) {
      LOG_ERROR("Failed to write data_hub protobuf bin.error: {}", status.message());
      return false;
    }

    std::fstream output(bin_file_path, std::fstream::out | std::fstream::trunc | std::fstream::binary);
    if (!data_map.SerializeToOstream(&output)) {
      LOG_ERROR("Failed to write data_hub protobuf bin.");
      return false;
    }
  } catch (fs::filesystem_error &e) {
    LOG_ERROR("{},error: {}", json_file_path, e.what());
    return false;
  } catch (const std::exception &ex) {
    LOG_ERROR("{},error: {}", json_file_path, ex.what());
    return false;
  }
  return true;
}

}  // namespace xlsx

}  // namespace tpn

