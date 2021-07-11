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

#include "chrono_wrap.h"
#include "utils.h"
#include "log_common.h"
#include "log_msg.h"
#include "log_hub.h"

namespace tpn {

namespace log {

/// 模式识别管理器
/// 常用格式只写了一种，如果要自定义的话 需要在 @formatter 里面 做parse重载
class TPN_COMMON_API Pattern {
 public:
  /// 格式化日志信息
  template <typename FormatContext>
  auto Format(const LogMsg &msg, FormatContext &ctx) {
    uint32_t format_type = g_log_hub->GetFormatType();

    if (format_type & EnumToUnderlyType(FormatType::kFormatTypeTimeCache)) {
      auto secs =
          std::chrono::duration_cast<Seconds>(msg.time.time_since_epoch());
      if (secs != last_log_secs_) {
        last_log_secs_ = secs;
        cache_datetime_.clear();
        fmt::format_to(cache_datetime_, "{:%Y-%m-%d %H:%M:%S}.{:03}",
                       g_log_hub->GetTime(msg.time),
                       TimeFraction<MilliSeconds>(msg.time).count());
      }
    }

    return fmt::format_to(
        ctx.out(), "{time}{level}{tid}{src_loc}{content}",
        "time"_a =
            (format_type & EnumToUnderlyType(FormatType::kFormatTypeTimeCache)
                 ? "[{}] "_format(cache_datetime_.data())
                 : "[{:%Y-%m-%d %H:%M:%S}.{:03}] "_format(
                       g_log_hub->GetTime(msg.time),
                       TimeFraction<MilliSeconds>(msg.time).count())),
        "level"_a =
            (format_type & EnumToUnderlyType(FormatType::kFormatTypeDebugLevel)
                 ? "[{:>5}] "_format(ToLogLevelStr(msg.level))
                 : "[{}] "_format(ToLogLevelShortStr(msg.level))),
        "tid"_a =
            (format_type & EnumToUnderlyType(FormatType::kFormatTypeNoThreadId)
                 ? ""
                 : "[{}] "_format(msg.thread_id)),
        "src_loc"_a =
            (format_type &
                     EnumToUnderlyType(FormatType::kFormatTypeNoSourceLocation)
                 ? ""
                 : "[{}] "_format(msg.src_loc)),
        "content"_a = msg.content);
  }

 private:
  Seconds last_log_secs_;        ///< 上次志记时间
  FmtMemoryBuf cache_datetime_;  ///< 上次缓冲时间

  TPN_SINGLETON_DECL(Pattern)
};

}  // namespace log

}  // namespace tpn

/// global log pattern instance
#define g_log_pattern tpn::log::Pattern::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_PATTERN_H_
