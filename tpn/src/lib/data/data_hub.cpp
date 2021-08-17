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

#include "data_hub.h"

#include <fstream>
#include <filesystem>

#include "utils.h"
#include "config.h"
#include "debug_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace data {

bool DataHubMgr::Load(std::string_view path, std::string &error,
                      bool reload /*  = false */) {
  if (!reload) {
    path_ = {path.data(), path.size()};
  }
  auto data_path = fs::path(path_);
  data_path.make_preferred();
  auto data_file = fs::absolute(data_path);
  DataHubMap data_map;
  try {
    std::fstream input(data_file, std::fstream::in | std::fstream::binary);
    if (!input) {
      error = "file not found (" + path_ + ") ";
      return false;
    } else if (!data_map.ParseFromIstream(&input)) {
      error = "file parse failed (" + path_ + ") ";
      return false;
    }
  } catch (fs::filesystem_error &e) {
    error = std::string{e.what()} + " (" + path_ + ") ";
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()} + " (" + path_ + ") ";
    return false;
  }

  if (!Init(data_map)) {
    error = "Init error.";
    return false;
  }

  return true;
}

bool DataHubMgr::Reload(std::string &error) { return Load({}, error, true); }

std::string_view DataHubMgr::GetPath() { return path_; }

/// uint32_t-id
const DataHubEntryItem::Item *DataHubMgr::GetDataHubEntryItem(
    uint32_t id) const {
  std::string map_key = "" + ToString(id);
  auto iter           = item_map_.find(map_key);
  return item_map_.end() == iter ? nullptr : &(iter->second);
}

/// uint32_t-level
const DataHubEntryLevel::Level *DataHubMgr::GetDataHubEntryLevel(
    uint32_t level) const {
  std::string map_key = "" + ToString(level);
  auto iter           = level_map_.find(map_key);
  return level_map_.end() == iter ? nullptr : &(iter->second);
}

/// uint32_t-id
const DataHubEntryPack::Pack *DataHubMgr::GetDataHubEntryPack(
    uint32_t id) const {
  std::string map_key = "" + ToString(id);
  auto iter           = pack_map_.find(map_key);
  return pack_map_.end() == iter ? nullptr : &(iter->second);
}

/// uint32_t-id
const DataHubEntryShop::Shop *DataHubMgr::GetDataHubEntryShop(
    uint32_t id) const {
  std::string map_key = "" + ToString(id);
  auto iter           = shop_map_.find(map_key);
  return shop_map_.end() == iter ? nullptr : &(iter->second);
}

/// uint32_t-id uint32_t-level
const DataHubEntrySkill::Skill *DataHubMgr::GetDataHubEntrySkill(
    uint32_t id, uint32_t level) const {
  std::string map_key = "" + ToString(id) + ToString(level);
  auto iter           = skill_map_.find(map_key);
  return skill_map_.end() == iter ? nullptr : &(iter->second);
}

bool DataHubMgr::Init(DataHubMap &data_map) {
  for (auto &&[key, val] : data_map.datas()) {
    if (val.Is<DataHubEntryItem>()) {
      DataHubEntryItem item;
      val.UnpackTo(&item);
      for (auto &&data : item.datas()) {
        std::string map_key = "" + ToString(data.id());
        TPN_ASSERT(0 == item_map_.count(map_key), "map key repeated. {}",
                   map_key);
        item_map_.emplace(map_key, data);
      }
    } else if (val.Is<DataHubEntryLevel>()) {
      DataHubEntryLevel level;
      val.UnpackTo(&level);
      for (auto &&data : level.datas()) {
        std::string map_key = "" + ToString(data.level());
        TPN_ASSERT(0 == level_map_.count(map_key), "map key repeated. {}",
                   map_key);
        level_map_.emplace(map_key, data);
      }
    } else if (val.Is<DataHubEntryPack>()) {
      DataHubEntryPack pack;
      val.UnpackTo(&pack);
      for (auto &&data : pack.datas()) {
        std::string map_key = "" + ToString(data.id());
        TPN_ASSERT(0 == pack_map_.count(map_key), "map key repeated. {}",
                   map_key);
        pack_map_.emplace(map_key, data);
      }
    } else if (val.Is<DataHubEntryShop>()) {
      DataHubEntryShop shop;
      val.UnpackTo(&shop);
      for (auto &&data : shop.datas()) {
        std::string map_key = "" + ToString(data.id());
        TPN_ASSERT(0 == shop_map_.count(map_key), "map key repeated. {}",
                   map_key);
        shop_map_.emplace(map_key, data);
      }
    } else if (val.Is<DataHubEntrySkill>()) {
      DataHubEntrySkill skill;
      val.UnpackTo(&skill);
      for (auto &&data : skill.datas()) {
        std::string map_key = "" + ToString(data.id()) + ToString(data.level());
        TPN_ASSERT(0 == skill_map_.count(map_key), "map key repeated. {}",
                   map_key);
        skill_map_.emplace(map_key, data);
      }
    }
  }
  return true;
}

TPN_SINGLETON_IMPL(DataHubMgr)
}  // namespace data

}  // namespace tpn
