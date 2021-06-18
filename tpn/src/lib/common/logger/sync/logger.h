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
#include "traits_hub.h"

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

  /// 交换两个记录器
  ///  @param[out]	other			函数执行完，变成交换的记录器
  void Swap(Logger &other) noexcept;

  /// 志记
  /// 处理fmt::compile_string可转化为fmt::string_view的模式串
  ///  @tparam			FormatString		日志模式串类型
  ///  @tparam			Args...					日志模式串参数
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args...					日志模式串参数
  template <
      typename FormatString,
      std::enable_if_t<
          std::is_convertible_v<const FormatString &, FmtStringView>, int> = 0,
      typename... Args>
  void LogWitchCheck(SourceLocation src_loc, LogLevel level,
                     const FormatString &fmt, Args &&...args) {
    LogStrv(std::move(src_loc), std::move(level), fmt,
            fmt::make_args_checked<Args...>(fmt, args...));
  }

  /// 志记
  /// 处理fmt::compile_string可转化为fmt::wstring_view的模式串
  ///  @tparam			FormatString		日志模式串类型
  ///  @tparam			Args...					日志模式串参数
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args...					日志模式串参数
  template <
      typename FormatString,
      std::enable_if_t<
          std::is_convertible_v<const FormatString &, FmtWStringView>, int> = 0,
      typename... Args>
  void LogWitchCheck(SourceLocation src_loc, LogLevel level,
                     const FormatString &fmt, Args &&...args) {
    LogWStrv(std::move(src_loc), std::move(level), fmt,
             fmt::make_args_checked<Args...>(fmt, args...));
  }

  /// 志记
  /// 处理fmt::compile_string
  ///  @tparam			FormatString		日志模式串类型
  ///  @tparam			Args...					日志模式串参数
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args...					日志模式串参数
  template <typename FormatString,
            std::enable_if_t<tpn::is_compile_string_v<FormatString>, int> = 0,
            typename... Args>
  void Log(SourceLocation src_loc, LogLevel level, const FormatString &fmt,
           Args &&...args) {
    LogWitchCheck(std::move(src_loc), std::move(level),
                  std::forward<decltype(fmt)>(fmt),
                  std::forward<decltype(args)>(args)...);
  }

  /// 志记
  /// 处理非fmt::compile_string 此种方法暂无需求 空接口
  ///  @tparam			FormatString		日志模式串类型
  ///  @tparam			Args...					日志模式串参数
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args...					日志模式串参数
  template <typename FormatString,
            std::enable_if_t<!tpn::is_compile_string_v<FormatString>, int> = 0,
            typename... Args>
  [[maybe_unused]] void Log([[maybe_unused]] SourceLocation src_loc,
                            [[maybe_unused]] LogLevel level,
                            [[maybe_unused]] const FormatString &fmt,
                            [[maybe_unused]] Args &&...args) {
    TPN_ASSERT(false, "logger only support compile string");
  }

 protected:
  /// 志记
  /// 处理fmt::wstring_view的模式串
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args						日志模式串参数
  void LogStrv(SourceLocation src_loc, LogLevel level, FmtStringView fmt,
               FmtFormatArgs args);

  /// 志记
  /// 处理fmt::wstring_view的模式串
  ///  @param[in]		src_loc					源文件定位信息
  ///  @param[in]		level						志记级别
  ///  @param[in]		fmt							日志模式串
  ///  @param[in]		args						日志模式串参数
  void LogWStrv(SourceLocation src_loc, LogLevel level, FmtWStringView fmt,
                FmtWFormatArgs args);

 public:
  /// 主动刷新
  void Flush();

  /// 获取记录器名称
  ///  @return 记录器名称
  std::string_view GetName() const;

  /// 设置记录器顶层志记级别
  /// 此接口会影响本记录器中所有追加器的级别，小于记录器级别的追加器将无效
  ///  @param[in]		level			志记级别
  void SetLogLevel(LogLevel level);

  /// 获取记录器顶层志记级别
  ///  @return 记录器顶层志记级别
  LogLevel GetLogLevel() const;

  /// 设置记录器顶层刷新级别
  /// 此接口会影响本记录器中所有追加器的级别，小于记录器级别的追加器将无效
  ///  @param[in]		level			刷新级别
  void SetFlushLevel(LogLevel level);

  /// 获取记录器顶层刷新级别
  ///  @return 记录器顶层刷新级别
  LogLevel GetFlushLevel() const;

  /// 获取记录器中所有追加器集合
  ///  @return 记录器中追加器集合
  const AppenderSptrVec &GetAppenders() const;

  /// 获取记录器中所有追加器集合
  ///  @return 记录器中追加器集合
  AppenderSptrVec &GetAppenders();

  /// 设置日志模块异常处理程序
  ///  @param[in]		handler			异常处理程序 函数签名@see ErrHandler
  void SetErrHandler(ErrHandler handler);

  /// 是否满足志记要求
  ///  @param[in]		level			检查志记级别
  ///  @return true 满足志记级别
  bool ShouldLog(LogLevel level) const;

  /// 是否满足刷新要求
  ///  @param[in]		level			检查刷新级别
  ///  @return true 满足刷新级别
  bool ShouldFlush(LogLevel level) const;

 protected:
  /// 异常处理
  ///  @param[in]		msg			异常信息
  void DoErrhandler(std::string_view msg);

  /// 追加器记录日志 接口
  ///  @param[in]		msg			志记信息
  virtual void AppenderDoLog(const LogMsg &msg);

  /// 追加器刷新 接口
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
