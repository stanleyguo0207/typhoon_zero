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

#include "../../test_include.h"

using namespace std;

#include "fmt_wrap.h"
#include "config.h"

// formatter
#include "formatter.h"

using namespace tpn;
using namespace tpn::log;

#ifndef _TPN_LOGGER_CONFIG_TEST_FILE
#  define _TPN_LOGGER_CONFIG_TEST_FILE "config_logger_test.json"
#endif

TEST_CASE("formatter", "[logger]") {
  // LogMsg msg("test", LogLevel::kLogLevelInfo,
  //            SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            "This is test formatter.\n");
  // FmtMemoryBuf buf;
  // try {
  //   fmt::format_to(buf, "msg: {}", msg);
  // } catch (const std::exception &ex) {
  //   fmt::print("error {}\n", ex.what());
  // }
  // std::string_view strv(buf.data(), buf.size());
  // fmt::print("{}", strv);
  //
  // AsyncLogMsg async_msg(msg);
  // buf.clear();
  // fmt::format_to(buf, "async msg: {}", msg);
  // strv = {buf.data(), buf.size()};
  // fmt::print("{}", strv);
}

#include "logger.h"

// console
#include "appender_console.h"

TEST_CASE("console", "[logger]") {
  // auto console_appender = std::make_shared<AppenderConsoleStdout>();
  // console_appender->SetLevel(LogLevel::kLogLevelTrace);
  //
  // Logger logger("console", console_appender);
  // logger.SetLogLevel(LogLevel::kLogLevelTrace);
  // logger.SetFlushLevel(LogLevel::kLogLevelInfo);
  //
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelTrace, "Test trace. {}", 1);
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelDebug, "Test debug. {}", 2.0);
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelInfo, "Test info. {}", "3");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelWarn, "Test warning. {}", "警告");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelError, L"Test error. {}", L"error msg 错误");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelFatal, "Test fatal. {}",
  //            std::vector<int>{1, 2, 3});
}

// daily_file
#include "appender_daily_file.h"

TEST_CASE("file", "[logger]") {
  // std::string error;
  // g_config->Load(_TPN_LOGGER_CONFIG_TEST_FILE, {}, error);
  // if (!error.empty()) {
  //   cout << error.c_str() << endl;
  //   return;
  // }
  //
  // auto console_appender = std::make_shared<AppenderConsoleStdout>();
  // console_appender->SetLevel(LogLevel::kLogLevelTrace);
  //
  // auto daily_appender =
  //     std::make_shared<AppenderDailyFile>("log/file/test.log", 0, 0, false, 2);
  // daily_appender->SetLevel(LogLevel::kLogLevelDebug);
  //
  // Logger logger("console", {console_appender, daily_appender});
  // logger.SetLogLevel(LogLevel::kLogLevelTrace);
  // logger.SetFlushLevel(LogLevel::kLogLevelInfo);
  //
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelTrace, "Test trace. {}", 1);
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelDebug, "Test debug. {}", 2.0);
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelInfo, "Test info. {}", "3");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelWarn, "Test warning. {}", "警告");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelError, L"Test error. {}", L"error msg 错误");
  // logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //            LogLevel::kLogLevelFatal, "Test fatal. {}",
  //            std::vector<int>{1, 2, 3});
}

#include <thread>

TEST_CASE("daily", "[logger]") {
  // std::string error;
  // g_config->Load(_TPN_LOGGER_CONFIG_TEST_FILE, {}, error);
  // if (!error.empty()) {
  //   cout << error.c_str() << endl;
  //   return;
  // }
  //
  // auto console_appender = std::make_shared<AppenderConsoleStdout>();
  // console_appender->SetLevel(LogLevel::kLogLevelTrace);
  //
  // auto daily_appender =
  //     std::make_shared<AppenderDailyFile>("log/daily/test.log", 0, 0, false, 4);
  // daily_appender->SetLevel(LogLevel::kLogLevelDebug);
  //
  // Logger logger("console", {console_appender, daily_appender});
  // logger.SetLogLevel(LogLevel::kLogLevelTrace);
  // logger.SetFlushLevel(LogLevel::kLogLevelInfo);
  //
  // for (int i = 0; i < 10; ++i) {
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelTrace, "Test trace. {}", 1);
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelDebug, "Test debug. {}", 2.0);
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelInfo, "Test info. {}", "3");
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelWarn, "Test warning. {}", "警告");
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelError, L"Test error. {}", L"error msg 错误");
  //   logger.Log(SourceLocation(__FILE__, __FUNCTION__, __LINE__),
  //              LogLevel::kLogLevelFatal, "Test fatal. {}",
  //              std::vector<int>{1, 2, 3});
  //
  //   std::this_thread::sleep_for(3s);
  // }
}

// log interface
#include "log.h"

TEST_CASE("log", "[logger]") {
  if (auto error = g_config->Load(_TPN_LOGGER_CONFIG_TEST_FILE, {})) {
    fmt::print(stderr, "Error in config file {}, error {}\n",
               _TPN_LOGGER_CONFIG_TEST_FILE, *error);
    return;
  }

  tpn::log::Init();
  std::shared_ptr<void> log_handle(nullptr,
                                   [](void *) { tpn::log::Shutdown(); });

  LOG_INFO("Test Logger start");

  LOG_DEBUG(L"wstr chinese: {}", L"你好");
  LOG_INFO("str args: {}", "I am 台风");
  LOG_WARN("rangers: {}", std::vector(12, 10));
  LOG_ERROR("num: {1:f}, {0:d}", 20, 300.0);
  LOG_ERROR("num: {numi:d}, {numf:f}", "numi"_a = 20, "numf"_a = 300.0);
  // 编译期间检查不了 无法检查 fmt::arg
  // LOG_DEBUG("num: {numi}, {numf}", fmt::arg("numi", 88),
  //           fmt::arg("numf", 888.0));
  LOG_DEBUG("num: {}", "{0:10}"_format(1000));
  LOG_FATAL("check format: {:>5d}", 99);

  LOG_INFO("Test Logger will shutdown in 5s");

  std::this_thread::sleep_for(5s);
}
