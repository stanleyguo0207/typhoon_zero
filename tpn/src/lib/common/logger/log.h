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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_H_

#include "log_hub.h"
#include "async_factory.h"

namespace tpn {

namespace log {

/// 默认记录器工厂
using DefaultFactory = AsynchronousFactory;

/// 使用默认记录器工厂生产记录器
///  @tparam      Appender          追加器类型
///  @tparam      AppenderArgs...   追加器所需参数
///  @param[in]   logger_name       记录器名称
///  @param[in]   args...           追加器构造所需参数
template <typename Apppender, typename... AppenderArgs>
TPN_INLINE LoggerSptr Create(std::string_view logger_name,
                             AppenderArgs &&...args) {
  return DefaultFactory::Create<Appender>(logger_name,
                                          std::forward<AppenderArgs>(args)...);
}

/// 初始化 此方法必须在主线程中启动
/// 日志模块需要使用Shutdown()来卸载
/// @code
/// tpn::log::Init();
/// std::shared_ptr<void> log_handle(nullptr, [](void *) { tpn::log::Shutdown(); });
/// @endcode
void Init();

/// 注册记录器
///  @param[in]   new_logger    新的记录器
void RegisterLogger(LoggerSptr new_logger);

/// 初始化记录器
/// 如果开了自动注册 会注册到日志中枢
///  @param[in]   new_logger    新的记录器
void InitializeLogger(LoggerSptr new_logger);

/// 根据名称获取记录器
///  @param[in]   logger_name   记录器名称
///  @return 注册过的记录器或者为空
LoggerSptr GetLoggerByName(std::string_view logger_name);

/// 获取默认记录器原始指针
///  @return 默认记录器指针或者为空
Logger *GetDefaultLoggerRaw();

/// 获取默认记录器
///  @return 默认记录器或者为空
LoggerSptr GetDefaultLogger();

/// 设置默认记录器
///  @param[in]   new_default_logger    新的默认记录器
void SetDefaultLogger(LoggerSptr new_default_logger);

/// 设置日志中枢全局志记级别
void SetGlobalLevel(LogLevel level);

/// 设置日志中枢全局刷新级别
void SetGlobalFlushLevel(LogLevel level);

/// 刷新所有记录的记录器
void FlushAll();

/// 间隔刷新
///  @tapram      Rep
///  @tapram      Period
///  @param[in]   interval    刷新间隔
template <typename Rep, typename Period>
void FlushEvery(std::chrono::duration<Rep, Period> interval);

/// 设置日志中枢全局错误处理函数
///  @param[in]   err_handler     错误处理函数
void SetErrHandler(ErrHandler err_handler);

/// 操作所有记录器
///  @param[in]   func    可调用函数
void ApplyAll(const std::function<void(const LoggerSptr)> &func);

/// 通过名称移除记录器
void Drop(std::string_view logger_name);

/// 移除所有记录器
void DropAll();

/// 停止日志中枢
void Shutdown();

/// 设置日志中枢自动注册标志
///  @param[in]   automatic_registration    自动注册标志
void SetAutomaticRegistration(bool automatic_registration);

}  // namespace log

}  // namespace tpn

#if (TPN_COMPILER_MSVC == TPN_COMPILER)
#  define LOGGER_CALL_BEGIN \
    __pragma(warning(push)) __pragma(warning(disable : 4127))
#  define LOGGER_CALL_END __pragma(warning(pop))
#else
#  define LOGGER_CALL_BEGIN
#  define LOGGER_CALL_END
#endif

#define LOGGER_CALL(logger, level, format, ...)                              \
  LOGGER_CALL_BEGIN do {                                                     \
    (logger)->Log(SourceLocation{__FILE__, __FUNCTION__, __LINE__}, (level), \
                  FMT_STRING((format)), ##__VA_ARGS__);                      \
  }                                                                          \
  while (0) LOGGER_CALL_END

#define LOGGER_TRACE(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelTrace, __VA_ARGS__)
#define LOGGER_DEBUG(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelDebug, __VA_ARGS__)
#define LOGGER_INFO(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelInfo, __VA_ARGS__)
#define LOGGER_WARN(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelWarn, __VA_ARGS__)
#define LOGGER_ERROR(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelError, __VA_ARGS__)
#define LOGGER_FATAL(logger, ...) \
  LOGGER_CALL(logger, tpn::log::LogLevel::kLogLevelFatal, __VA_ARGS__)

/// 日志 级别 trace
#define LOG_TRACE(...) \
  LOGGER_TRACE(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)
/// 日志 级别 debug
#define LOG_DEBUG(...) \
  LOGGER_DEBUG(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)
/// 日志 级别 info
#define LOG_INFO(...) LOGGER_INFO(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)
/// 日志 级别 warn
#define LOG_WARN(...) LOGGER_WARN(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)
/// 日志 级别 error
#define LOG_ERROR(...) \
  LOGGER_ERROR(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)
/// 日志 级别 fatal
#define LOG_FATAL(...) \
  LOGGER_FATAL(tpn::log::GetDefaultLoggerRaw(), __VA_ARGS__)

#if defined(TPN_NETDEBUG)
#  define NET_TRACE(...) LOG_TRACE(__VA_ARGS__)
#  define NET_DEBUG(...) LOG_DEBUG(__VA_ARGS__)
#  define NET_INFO(...) LOG_INFO(__VA_ARGS__)
#  define NET_WARN(...) LOG_WARN(__VA_ARGS__)
#  define NET_ERROR(...) LOG_ERROR(__VA_ARGS__)
#  define NET_FATAL(...) LOG_FATAL(__VA_ARGS__)
#else
#  define NET_TRACE(...)
#  define NET_DEBUG(...)
#  define NET_INFO(...) LOG_INFO(__VA_ARGS__)
#  define NET_WARN(...)
#  define NET_ERROR(...) LOG_ERROR(__VA_ARGS__)
#  define NET_FATAL(...) LOG_FATAL(__VA_ARGS__)
#endif

#if defined(TPN_AOIDEBUG)
#  define AOI_TRACE(...) LOG_TRACE(__VA_ARGS__)
#  define AOI_DEBUG(...) LOG_DEBUG(__VA_ARGS__)
#  define AOI_INFO(...) LOG_INFO(__VA_ARGS__)
#  define AOI_WARN(...) LOG_WARN(__VA_ARGS__)
#  define AOI_ERROR(...) LOG_ERROR(__VA_ARGS__)
#  define AOI_FATAL(...) LOG_FATAL(__VA_ARGS__)
#else
#  define AOI_TRACE(...)
#  define AOI_DEBUG(...)
#  define AOI_INFO(...)
#  define AOI_WARN(...)
#  define AOI_ERROR(...)
#  define AOI_FATAL(...)
#endif

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_H_
