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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_PATTERN_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_PATTERN_H_

#include "log_common.h"
#include "log_msg.h"

namespace tpn {

namespace log {

/// 模式识别管理器
/// 常用格式只写了一种，如果要自定义的话 需要在 @formatter 里面 做parse重载
class TPN_COMMON_API Pattern {
 public:
  /// 初始化
  void Init();

  /// 格式化日志信息
  /// 默认格式 [级别] [时间] [源文件定位信息] [线程id] 日志内容
  template <typename FormatContext>
  auto Format(const LogMsg &msg, FormatContext &ctx) {
    auto secs =
        std::chrono::duration_cast<Seconds>(msg.time.time_since_epoch());
    if (secs != last_log_secs_) {
      last_log_secs_ = secs;
      cached_tm_     = GetTime(msg);
    }

    return fmt::format_to(
        ctx.out(), "[{}] [{:%Y-%m-%d %H:%M:%S}] [{}] [{}] [{}] {}",
        fmt::format("{:>5}", ToLogLevelStr(msg.level)), cached_tm_,
        msg.logger_name, msg.src_loc, msg.thread_id, msg.content);
  }

 private:
  /// 日志中提取模式时间
  std::tm GetTime(const LogMsg &msg) const;

 private:
  PatternTimeType pattern_time_type_{
      PatternTimeType::kPatternTimeTypeLocal};  ///< 模式时间
  std::tm cached_tm_{};                         ///< 缓存时间
  Seconds last_log_secs_;                       ///< 上次志记时间

  TPN_SINGLETON_DECL(Pattern)
};

}  // namespace log

}  // namespace tpn

/// global log pattern instance
#define g_log_pattern tpn::log::Pattern::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_PATTERN_H_
