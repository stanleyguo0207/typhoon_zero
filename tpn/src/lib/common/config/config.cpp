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
// but WITHOUT ANY WARRANTY{} without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "config.h"

#include <filesystem>

#include <rapidjson/filereadstream.h>

#include "debug_hub.h"

namespace fs = std::filesystem;

namespace tpn {

bool ConfigMgr::Load(std::string_view path, std::vector<std::string> args,
                     std::string &error, bool reload /*  = false */) {
  std::lock_guard<std::mutex> lock(load_mutex_);

  if (!reload) {
    path_ = path;
    args_ = std::move(args);
  }

  auto json_path = fs::path(path);
  json_path.make_preferred();
  auto json_file = fs::absolute(json_path);
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
    document_.SetNull();
    document_.ParseStream<rapidjson::kParseCommentsFlag>(is);
    std::fclose(fp);
    delete[] buf;

    if (document_.HasParseError()) {
      error = std::string("parse error") + " (" + path_ + ") ";
      return false;
    }
  } catch (fs::filesystem_error &e) {
    error = std::string{e.what()} + " (" + path_ + ") ";
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()} + " (" + path_ + ") ";
    return false;
  }

  return true;
}

bool ConfigMgr::Reload(std::string &error) { return Load({}, {}, error, true); }

std::string_view ConfigMgr::GetPath() const { return path_; }

const std::vector<std::string> &ConfigMgr::GetArguments() const {
  return args_;
}

std::string ConfigMgr::GetStringDefault(std::string_view key,
                                        std::string_view def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsString(),
               "Wrong Type not string key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return std::string{document_[key.data()].GetString()};
  } else {
    return std::string{def};
  }
}

bool ConfigMgr::GetBoolDefault(std::string_view key, bool def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsBool(),
               "Wrong Type not bool key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetBool();
  } else {
    return def;
  }
}

int32_t ConfigMgr::GetI32Default(std::string_view key, int32_t def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsInt(),
               "Wrong Type not int key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetInt();
  } else {
    return def;
  }
}

int64_t ConfigMgr::GetI64Default(std::string_view key, int64_t def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsInt64(),
               "Wrong Type not int64 key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetInt64();
  } else {
    return def;
  }
}

uint32_t ConfigMgr::GetU32Default(std::string_view key, uint32_t def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsUint(),
               "Wrong Type not uint key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetUint();
  } else {
    return def;
  }
}

uint64_t ConfigMgr::GetU64Default(std::string_view key, uint64_t def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsUint64(),
               "Wrong Type not uint64 key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetUint64();
  } else {
    return def;
  }
}

float ConfigMgr::GetFloatDefault(std::string_view key, float def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsFloat(),
               "Wrong Type not float key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetFloat();
  } else {
    return def;
  }
}

double ConfigMgr::GetDoubleDefault(std::string_view key, double def) const {
  TPN_ASSERT(!document_.IsNull(), "Please Call Load Before");
  if (document_.HasMember(key.data())) {
    TPN_ASSERT(document_[key.data()].IsDouble(),
               "Wrong Type not double key:{} def:{} key_type:{}", key, def,
               document_[key.data()].GetType());
    return document_[key.data()].GetDouble();
  } else {
    return def;
  }
}

TPN_SINGLETON_IMPL(ConfigMgr)
}  // namespace tpn
