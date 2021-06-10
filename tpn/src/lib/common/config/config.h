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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_CONFIG_CONFIG_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_CONFIG_CONFIG_H_

#include <mutex>
#include <vector>
#include <string>
#include <string_view>

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>

#include "define.h"

namespace tpn {

/// 全局配置管理器
class TPN_COMMON_API ConfigMgr {
 public:
  /// 加载配置文件
  ///  @param[in]		path		配置文件全路径
  ///  @param[in]		args		读取配置文件时参数
  ///  @param[out]	error		读取配置错误信息
  ///  @param[in]		reload	重新加载标志 默认false
  ///  @return 是否加载配置成功
  bool Load(std::string_view path, std::vector<std::string> args,
            std::string &error, bool reload = false);

  /// 重新加载配置文件
  ///  @param[out]	error		读取配置错误信息
  ///  @return 是否加载配置成功
  bool Reload(std::string &error);

  /// 获取配置文件全路径
  ///  @return 配置文件全局路径
  std::string_view GetPath() const;

  /// 获取读取配置文件时参数
  ///  @reutrn 读取配置文件时参数
  const std::vector<std::string> &GetArguments() const;

  /// 获取string类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  std::string GetStringDefault(std::string_view key,
                               std::string_view def) const;

  /// 获取bool类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  bool GetBoolDefault(std::string_view key, bool def) const;

  /// 获取int32_t类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  int32_t GetI32Default(std::string_view key, int32_t def) const;

  /// 获取int64_t类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  int64_t GetI64Default(std::string_view key, int64_t def) const;

  /// 获取uint32_t类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  uint32_t GetU32Default(std::string_view key, uint32_t def) const;

  /// 获取uint64_t类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  uint64_t GetU64Default(std::string_view key, uint64_t def) const;

  /// 获取float类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  float GetFloatDefault(std::string_view key, float def) const;

  /// 获取double类型配置值
  ///  @param[in]		key			配置key
  ///  @param[in]		def			默认值
  ///  @return 配置val
  double GetDoubleDefault(std::string_view key, double def) const;

 private:
  std::string path_;               ///< 配置文件全路径
  std::vector<std::string> args_;  ///< 读取配置文件时参数
  std::mutex load_mutex_;          ///< 读取文件互斥锁
  rapidjson::Document document_;   ///< 配置数据

  TPN_SINGLETON_DECL(ConfigMgr)
};

}  // namespace tpn

/// global config manager instance
#define g_config tpn::ConfigMgr::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_CONFIG_CONFIG_H_
