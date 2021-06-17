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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_FILE_APPENDER_DAILY_FILE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_FILE_APPENDER_DAILY_FILE_H_

#include <string>

#include "appender_base.h"
#include "file_helper.h"
#include "circular_queue.h"
#include "sync_factory.h"

namespace tpn {

namespace log {

/// 轮转日志追加器
class TPN_COMMON_API AppenderDailyFile : public AppenderBase {
 public:
  /// 构造函数
  ///  @param[in]		base_name					基础路径
  ///  @param[in]		rotation_hour			轮转小时
  ///  @param[in]		rotation_minute		轮转分钟
  ///  @param[in]		truncate					截断
  ///  @param[in]		max_files					保留最大文件数量
  explicit AppenderDailyFile(std::string_view base_name, int rotation_hour,
                             int rotation_minute, bool truncate = false,
                             uint16_t max_files = 7);

  /// 获取日志路径
  std::string_view GetPath() const;

 protected:
  /// 记录日志实现接口
  ///  @param[in]		msg			日志信息
  void DoLog(const LogMsg &msg) override;

  /// 日志刷新实现接口
  void DoFlush() override;

 private:
  /// 初始化
  void Init();

  /// 获取下次轮转时间点
  ///  @return 下次论转时间点
  LogClock::time_point NextRotationTp();

  /// 移除旧的文件
  void DeleteOld();

 private:
  FileHelper helper_;                  ///< 文件辅助
  std::string base_name_{""};          ///< 文件基础名称
  int rotation_h_{0};                  ///< 轮转小时
  int rotation_m_{0};                  ///< 轮转分钟
  LogClock::time_point rotation_tp_;   ///< 论转时间点
  bool truncate_{false};               ///< 截断标识
  uint16_t max_files_{0};              ///< 最大文件数量
  CircularQueue<std::string> path_q_;  ///< 路径循环队列
};

}  // namespace log

using namespace log;

/// 每日文件记录器工厂
template <typename Factory = SynchronousFactory>
LoggerSptr DailyFileLogger(std::string_view logger_name,
                           std::string_view base_name, int rotation_hour = 0,
                           int rotation_minute = 0, bool truncate = false,
                           uint16_t max_files = 7);

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_FILE_APPENDER_DAILY_FILE_H_
