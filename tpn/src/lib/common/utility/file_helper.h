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

/// 文件辅助类
class TPN_COMMON_API FileHelper {
 public:
  FileHelper() = default;
  ~FileHelper();

  /// 打开文件
  ///  @param[in]		path			文件路径
  ///  @param[in]		truncate	是否截断文件 默认不追加
  void Open(std::string_view path, bool truncate = false);

  /// 重新打开文件
  ///  @param[in]		truncate
  void Reopen(bool truncate);

  /// 刷新文件缓冲区
  void Flush();

  /// 关闭文件
  void Close();

  /// 写入文件
  ///  @param[in]		buf			数据缓冲区
  void Write(const FmtMemoryBuf &buf);

  /// 获取文件大小
  ///  @return 文件大小
  size_t Size() const;

  /// 获取文件原始路径
  ///  @return 原始路径
  std::string_view GetPath() const;

  /// 按照扩展分割路径
  ///  @param[in]		path		要分割的路径
  ///  @return pair<路径前缀, 后缀>
  ///
  /// in "sandbox/1/2/a/m.txt"
  /// out <"sandbox/1/2/a/m", ".txt">
  static std::pair<std::string, std::string> SplitByExtension(
      std::string_view path);

 public:
  static constexpr int32_t kFileOpenTyrMaxTimes = 5;  ///< 文件打开尝试最多次数
  static constexpr int32_t kFileOpenIntervalMill = 10;  ///< 文件打开间隔毫秒

 private:
  std::FILE *file_{nullptr};  ///< 关联文件
  std::string path_;          ///< 路径

  TPN_NO_COPYABLE(FileHelper)
  TPN_NO_MOVEABLE(FileHelper)
};

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_UTILITY_FILE_HELPER_H_
