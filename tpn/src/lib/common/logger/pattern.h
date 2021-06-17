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
#include "log_hub.h"

namespace tpn {

namespace log {

/// 模式识别管理器
/// 常用格式只写了一种，如果要自定义的话 需要在 @formatter 里面 做parse重载
class TPN_COMMON_API Pattern {
 public:
  /// 格式化日志信息
  /// 默认格式 [级别] [时间] [源文件定位信息] [线程id] 日志内容
  template <typename FormatContext>
  auto Format(const LogMsg &msg, FormatContext &ctx) {
    auto secs =
        std::chrono::duration_cast<Seconds>(msg.time.time_since_epoch());
    if (secs != last_log_secs_) {
      last_log_secs_ = secs;
      cached_tm_     = g_log_hub->GetTime(msg.time);
    }

    return fmt::format_to(
        ctx.out(),
        "[{level}] [{time:%Y-%m-%d %H:%M:%S}] [{src_loc}] "
        "[{thread_id}] {content}",
        "level"_a = fmt::format("{:>5}", ToLogLevelStr(msg.level)),
        "time"_a = cached_tm_, "src_loc"_a = msg.src_loc,
        "thread_id"_a = msg.thread_id, "content"_a = msg.content);
  }

 private:
  std::tm cached_tm_{};    ///< 缓存时间
  Seconds last_log_secs_;  ///< 上次志记时间

  TPN_SINGLETON_DECL(Pattern)
};

}  // namespace log

}  // namespace tpn

/// global log pattern instance
#define g_log_pattern tpn::log::Pattern::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_PATTERN_H_
