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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_MSG_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_MSG_H_

#include <string_view>

#include "log_common.h"

namespace tpn {

namespace log {

/// 日志信息
/// 只支持同步的日志信息，如果使用异步的消息，需要保存logger_name和content
struct TPN_COMMON_API LogMsg {
  LogMsg() = default;

  /// 构造函数
  ///  @param[in]		logger_name_in		日志名称
  ///  @param[in]		level_in					日志级别
  ///  @param[in]		time_in						日志记录时间
  ///  @param[in]		src_loc_in				源文件定位信息
  ///  @param[in]		content_in				日志内容
  LogMsg(std::string_view logger_name_in, LogLevel level_in,
         LogClock::time_point time_in, SourceLocation src_loc_in,
         std::string_view content_in);

  /// 构造函数
  ///  @param[in]		logger_name_in		日志名称
  ///  @param[in]		level_in					日志级别
  ///  @param[in]		src_loc_in				源文件定位信息
  ///  @param[in]		content_in				日志内容
  LogMsg(std::string_view logger_name_in, LogLevel level_in,
         SourceLocation src_loc_in, std::string_view content_in);

  /// 构造函数
  ///  @param[in]		logger_name_in		日志名称
  ///  @param[in]		level_in					日志级别
  ///  @param[in]		content_in				日志内容
  LogMsg(std::string_view logger_name_in, LogLevel level_in,
         std::string_view content_in);

  std::string_view logger_name;            ///< 日志名称
  LogLevel level{LogLevel::kLogLevelOff};  ///< 日志级别
  LogClock::time_point time;               ///< 日志记录时间点
  SourceLocation src_loc;                  ///< 源文件定位信息
  std::string_view content;                ///< 日志内容
  size_t thread_id{0};                     ///< 线程id

  TPN_DEFAULT_COPY(LogMsg);
  TPN_DEFAULT_MOVE(LogMsg);
};

/// 异步日志消息
/// 因日志消息中存放的数据可能失效，所以数据需要存在缓冲区中
struct TPN_COMMON_API AsyncLogMsg : public LogMsg {
  AsyncLogMsg() = default;

  /// 构造函数
  ///  @param[in]		msg			日志消息
  explicit AsyncLogMsg(const LogMsg &msg);

  /// 拷贝构造函数
  AsyncLogMsg(const AsyncLogMsg &other);

  /// 移动构造函数
  AsyncLogMsg(AsyncLogMsg &&other) noexcept;

  /// 拷贝赋值函数
  AsyncLogMsg &operator=(const AsyncLogMsg &other);

  /// 移动赋值函数
  AsyncLogMsg &operator=(AsyncLogMsg &&other) noexcept;

 private:
  /// 更新数据缓冲区
  void UpdateBuf();

 private:
  FmtMemoryBuf buf_;  ///< 数据缓冲
};

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_MSG_H_
