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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_FILE_HELPER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_FILE_HELPER_H_

#include <string>

#include "define.h"
#include "fmt_wrap.h"

namespace tpn {

///< 文件打开尝试最多次数
constexpr int32_t kFileOpenTyrMaxTimes = 5;

///< 文件打开间隔毫秒
constexpr int32_t kFileOpenIntervalMill = 10;

/// 文件辅助类
class TPN_COMMON_API FileHelper {
 public:
  FileHelper() = default;
  ~FileHelper();

  void Open(std::string_view path, bool truncate = false);

  void Reopen(bool truncate);

  void Flush();

  void Close();

  void Write(const FmtMemoryBuf &buf);

  size_t Size() const;

  std::string_view GetPath() const;

  static std::pair<std::string, std::string> SplitByExtension(
      std::string_view path);

 private:
  std::FILE *file_{nullptr};  ///< 关联文件
  std::string path_;          ///< 路径

  TPN_NO_COPYABLE(FileHelper)
  TPN_NO_MOVEABLE(FileHelper)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_FILE_HELPER_H_
