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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_

#include <vector>
#include <atomic>

#include "log_common.h"
#include "log_msg.h"
#include "utils.h"
#include "exception_hub.h"

namespace tpn {

namespace log {

/// 同步日志记录器
class TPN_COMMON_API Logger {
 public:
  /// 构造函数
  ///  @param[in]		name		记录器名称
  explicit Logger(std::string_view name);

  /// 构造函数 迭代器
  ///  @tparam			Iter		迭代器类型
  ///  @param[in]		name		记录器名称
  ///  @param[in]		begin		追加器容器开始迭代器
  ///  @param[in]		end			追加器容器结束迭代器
  template <typename Iter>
  Logger(std::string_view name, Iter begin, Iter end)
      : name_{name.data(), name.size()}, appenders_(begin, end) {}

  /// 构造函数 容器
  ///  @param[in]		name				记录器名称
  ///  @param[in]		appenders		追加器初始化列表
  Logger(std::string_view name, AppenderInitList appenders);

  /// 构造函数 迭代器
  ///  @param[in]		name							记录器名称
  ///  @param[in]		single_appender		单个追加器
  Logger(std::string_view name, AppenderSptr single_appender);

  virtual ~Logger() = default;

  /// 拷贝构造函数
  Logger(const Logger &other);

  /// 移动构造函数
  Logger(Logger &&other) noexcept;

  /// 赋值函数
  Logger &operator=(Logger other) noexcept;

  void Swap(Logger &other) noexcept;

  template <
      typename FormatString,
      std::enable_if_t<fmt::is_compile_string<FormatString>::value, int> = 0,
      typename... Args>
  void Log(SourceLocation src_loc, LogLevel level, const FormatString &fmt,
           const Args &...args) {
    LogImpl(src_loc, level, fmt, args...);
  }

  template <typename... Args>
  void Log(SourceLocation src_loc, LogLevel level, std::string_view fmt,
           const Args &...args) {
    LogImpl(src_loc, level, fmt, args...);
  }

  template <typename FormatString, typename... Args>
  void Log(LogLevel level, const FormatString &fmt, const Args &...args) {
    Log(SourceLocation{}, level, fmt, args...);
  }

  template <
      typename T,
      std::enable_if_t<std::is_convertible_v<const T &, std::string_view> &&
                           !fmt::is_compile_string<T>::value,
                       int> = 0>
  void Log(SourceLocation src_loc, LogLevel level, const T &msg) {
    Log(src_loc, level, std::string_view{msg});
  }

  template <
      typename T,
      std::enable_if_t<!std::is_convertible_v<const T &, std::string_view> &&
                           !std::is_convertible_v<const T &, std::wstring_view>,
                       int> = 0>
  void Log(SourceLocation src_loc, LogLevel level, const T &msg) {
    Log(src_loc, level, "{}", msg);
  }

  void Log(LogClock::time_point log_time, SourceLocation src_loc,
           LogLevel level, std::string_view msg);

  void Log(SourceLocation src_loc, LogLevel level, std::string_view msg);

  void Log(LogLevel level, std::string_view msg);

  template <typename T>
  void Log(LogLevel level, const T &msg) {
    Log(SourceLocation{}, level, msg);
  }

  template <typename... Args>
  void Log(SourceLocation src_loc, LogLevel level, std::wstring_view fmt,
           const Args &...args) {
    bool log_enabled = ShouldLog(level);
    if (!log_enabled) {
      return;
    }

    try {
      FmtWMemoryBuf wbuf;
      fmt::format_to(wbuf, fmt, args...);

      std::string utf8str;
      if (!WstrToUtf8(wbuf.data(), wbuf.size(), utf8str)) {
        TPN_THROW(LogException("WstrToUtf8 exception in logger"));
      }

      LogMsg log_msg(name_, level, src_loc, utf8str);
      AppenderDoLog(log_msg);
    } catch (const std::exception &ex) {
      DoErrhandler(ex.what());
    } catch (...) {
      DoErrhandler("Unkown exception in logger.");
    }
  }

  template <typename T,
            std::enable_if_t<
                std::is_convertible_v<const T &, std::wstring_view>, int> = 0>
  void Log(SourceLocation src_loc, LogLevel level, const T &msg) {
    bool log_enabled = ShouldLog(level);
    if (!log_enabled) {
      return;
    }

    try {
      std::string utf8str;
      if (!WstrToUtf8(msg.data(), msg.size(), utf8str)) {
        TPN_THROW(LogException("WstrToUtf8 exception in logger"));
      }

      LogMsg log_msg(name_, level, src_loc, utf8str);
      AppenderDoLog(log_msg);
    } catch (const std::exception &ex) {
      DoErrhandler(ex.what());
    } catch (...) {
      DoErrhandler("Unkown exception in logger.");
    }
  }

 protected:
  template <typename FormatString, typename... Args>
  void LogImpl(SourceLocation src_loc, LogLevel level, const FormatString &fmt,
               const Args &...args) {
    bool log_enabled = ShouldLog(level);
    if (!log_enabled) {
      return;
    }

    try {
      FmtMemoryBuf buf;
      fmt::format_to(buf, fmt, args...);
      LogMsg log_msg(name_, level, src_loc,
                     std::string_view(buf.data(), buf.size()));
      AppenderDoLog(log_msg);
    } catch (const std::exception &ex) {
      DoErrhandler(ex.what());
    } catch (...) {
      DoErrhandler("Unkown exception in logger.");
    }
  }

 public:
  void Flush();

  std::string_view GetName() const;

  void SetLogLevel(LogLevel level);
  LogLevel GetLogLevel() const;

  void SetFlushLevel(LogLevel level);
  LogLevel GetFlushLevel() const;

  const AppenderSptrVec &GetAppenders() const;
  AppenderSptrVec &GetAppenders();

  void SetErrHandler(ErrHandler handler);

  bool ShouldLog(LogLevel level) const;
  bool ShouldFlush(LogLevel level) const;

 protected:
  void DoErrhandler(std::string_view msg);

  virtual void AppenderDoLog(const LogMsg &msg);
  virtual void AppenderDoFlush();

 protected:
  std::string name_{""};                                 ///< 记录器名称
  std::atomic<LogLevel> level_{LogLevel::kLogLevelOff};  ///< 志记级别
  std::atomic<LogLevel> flush_level_{LogLevel::kLogLevelOff};  ///< 刷新级别
  AppenderSptrVec appenders_;                                  ///< 附加器
  ErrHandler err_handler_{nullptr};                            ///< 错误处理
};

/// 交换两个记录器
void Swap(Logger &a, Logger &b);

}  // namespace log

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_SYNC_LOGGER_H_
