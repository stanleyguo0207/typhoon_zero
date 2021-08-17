//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            �?└┬┘├─┘├─┤│ ││ ││││
//            �? �?�? �?┴└─┘└─┘┘└┘
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

#ifndef __TYPHOON_DATA_HUB_H__
#define __TYPHOON_DATA_HUB_H__

#include <map>
#include <string>
#include <string_view>

#include "define.h"
#include "data_hub.pb.h"

namespace tpn {

namespace data {

class TPN_DATA_API DataHubMgr {
 public:
  bool Load(std::string_view path, std::string &error, bool reload = false);
  bool Reload(std::string &error);
  bool Init(DataHubMap &data_map);
  std::string_view GetPath();

 public:
  /// uint32_t-id
  const DataHubEntryItem::Item *GetDataHubEntryItem(uint32_t id) const;
 private:
  std::map<std::string, DataHubEntryItem::Item> item_map_;

 public:
  /// uint32_t-level
  const DataHubEntryLevel::Level *GetDataHubEntryLevel(uint32_t level) const;
 private:
  std::map<std::string, DataHubEntryLevel::Level> level_map_;

 public:
  /// uint32_t-id
  const DataHubEntryPack::Pack *GetDataHubEntryPack(uint32_t id) const;
 private:
  std::map<std::string, DataHubEntryPack::Pack> pack_map_;

 public:
  /// uint32_t-id
  const DataHubEntryShop::Shop *GetDataHubEntryShop(uint32_t id) const;
 private:
  std::map<std::string, DataHubEntryShop::Shop> shop_map_;

 public:
  /// uint32_t-id uint32_t-level
  const DataHubEntrySkill::Skill *GetDataHubEntrySkill(uint32_t id, uint32_t level) const;
 private:
  std::map<std::string, DataHubEntrySkill::Skill> skill_map_;

 private:
  std::string path_;

  TPN_SINGLETON_DECL(DataHubMgr)
};

}  // namespace data

}  // namespace tpn

/// global data manager instance
#define g_data_hub tpn::data::DataHubMgr::Instance()

#endif  // __TYPHOON_DATA_HUB_H__

