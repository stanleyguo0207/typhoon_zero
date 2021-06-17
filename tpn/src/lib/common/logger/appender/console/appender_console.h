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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_CONSOLE_APPENDER_CONSOLE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_CONSOLE_APPENDER_CONSOLE_H_

#include <memory>
#include <array>
#include <mutex>

#include "fmt_wrap.h"
#include "appender_base.h"
#include "sync_factory.h"

namespace tpn {

namespace log {

/// 控制台附加器
class TPN_COMMON_API AppenderConsoleBase : public AppenderBase {
 public:
  /// 构造函数
  ///  @param[in]		file		控制台文件
  ///  @param[in]		mode		颜色显示模式
  AppenderConsoleBase(std::FILE *file, AppenderColorMode mode);
  ~AppenderConsoleBase() override = default;

  /// 设置颜色
  ///  @param[in]		level		日志级别
  ///  @param[in]		color		颜色样式
  void SetColor(LogLevel level, fmt::text_style color);

  /// 设置颜色显示模式
  ///  @param[in]		mode		颜色显示模式
  void SetColorMode(AppenderColorMode mode);

 protected:
  /// 记录日志实现接口
  ///  @param[in]		msg			日志信息
  void DoLog(const LogMsg &msg) override;

  /// 日志刷新实现接口
  void DoFlush() override;

 protected:
  static constexpr uint8_t kColorEnable =
      static_cast<uint8_t>(LogLevel::kLogLevelEnable);  ///< 日志级别长度

  std::FILE *file_{nullptr};                          ///< console文件
  bool should_do_colors_{false};                      ///< 是否使用颜色
  std::array<fmt::text_style, kColorEnable> colors_;  ///< 日志级别对应颜色
  std::mutex color_mutex_;                            ///< 颜色互斥锁

  TPN_NO_COPYABLE(AppenderConsoleBase)
};

/// stdout附加器
class TPN_COMMON_API AppenderConsoleStdout final : public AppenderConsoleBase {
 public:
  /// 构造函数
  /// @param[in]		mode		颜色模式
  explicit AppenderConsoleStdout(
      AppenderColorMode mode = AppenderColorMode::kAppenderColorModeAutomatic);
};

/// stderr附加器
class TPN_COMMON_API AppenderConsoleStderr final : public AppenderConsoleBase {
 public:
  /// 构造函数
  /// @param[in]		mode		颜色模式
  explicit AppenderConsoleStderr(
      AppenderColorMode mode = AppenderColorMode::kAppenderColorModeAutomatic);
};

}  // namespace log

using namespace log;

/// 标准输出控制台记录器工厂
template <typename Factory = SynchronousFactory>
LoggerSptr ConsoleStdoutLogger(
    std::string_view logger_name,
    AppenderColorMode mode = AppenderColorMode::kAppenderColorModeAutomatic);

/// 标准cuowu控制台记录器工厂
template <typename Factory = SynchronousFactory>
LoggerSptr ConsoleStderrLogger(
    std::string_view logger_name,
    AppenderColorMode mode = AppenderColorMode::kAppenderColorModeAutomatic);

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_APPENDER_CONSOLE_APPENDER_CONSOLE_H_
