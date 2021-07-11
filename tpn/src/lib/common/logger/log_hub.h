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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_

#include <mutex>
#include <string>
#include <unordered_map>

#include "chrono_wrap.h"
#include "log_common.h"

namespace tpn {

namespace log {

/// 日志中枢
class TPN_COMMON_API LogHub {
 public:
  /// 初始化
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
  void FlushEvery(std::chrono::duration<Rep, Period> interval) {
    std::lock_guard<std::mutex> lock(flush_mutex_);
    auto task         = [this] { this->FlushAll(); };
    periodic_flusher_ = std::make_unique<PeriodicWorker>(task, interval);
  }

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

  /// 获取日志中枢时间格式
  /// 只能从配置中修改 不支持全局修改
  ///  @return 日志时间格式
  PatternTimeType GetPatternTimeType() const;

  /// 根据模式时间返回时间结构
  ///  @return tp对应的时间结构
  std::tm GetTime(LogClock::time_point tp) const;

  /// 获取日志格式类型
  ///  @return 格式类型
  uint32_t GetFormatType() const;

  /// 获取线程池锁
  ///  @return 线程池锁
  std::recursive_mutex &GetThreadPoolMutex();

  /// 设置线程池
  ///  @param[in]   thread_pool     线程池
  void SetThreadPool(ThreadPoolSptr thread_pool);

  /// 获取线程池
  ///  @return 日志线程池
  ThreadPoolSptr GetThreadPool();

 private:
  /// 如果存在日志名称则抛出异常
  ///  @param[in]   logger_name     要查询的日志名称
  void ThrowIfExists(std::string_view logger_name);

  /// 注册记录器
  ///  @param[in]   new_logger      新的日志记录器
  void DoRegisterLogger(LoggerSptr new_logger);

 private:
  std::mutex logger_map_mutex_;  ///< 日志记录器锁
  std::mutex flush_mutex_;       ///< 刷新锁
  std::unordered_map<std::string_view, LoggerSptr>
      loggers_;  ///< 注册的日志记录器
  std::unordered_map<std::string, LogLevel> levels_;  ///< 注册的日志级别
  LogLevel global_log_level_{LogLevel::kLogLevelDebug};  ///< 全局志记级别
  LogLevel global_flush_level_{LogLevel::kLogLevelDebug};  ///< 全局刷新级别
  ErrHandler global_err_handler_{nullptr};  ///< 全局错误处理
  LoggerSptr default_logger_{nullptr};      ///< 默认记录器
  bool automatic_registration_{true};       ///< 是否默认注册
  PatternTimeType pattern_time_type_{
      PatternTimeType::kPatternTimeTypeLocal};  ///< 模式时间
  uint32_t format_type_{0};                     ///< 日志格式类型
  std::recursive_mutex thread_pool_mutex_;      ///< 线程池锁
  ThreadPoolSptr thread_pool_;                  ///< 异步线程池
  PeriodicWorkerUptr periodic_flusher_;         ///< 周期刷新器

  TPN_SINGLETON_DECL(LogHub)
};

}  // namespace log

}  // namespace tpn

/// global log hub instance
#define g_log_hub tpn::log::LogHub::Instance()

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_LOGGER_LOG_HUB_H_
