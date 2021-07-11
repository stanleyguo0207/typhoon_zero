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

#include "async_logger.h"

#include <memory>

#include "thread_pool.h"
#include "debug_hub.h"
#include "exception_hub.h"
#include "fmt_wrap.h"
#include "appender.h"

namespace tpn {

namespace log {

AsyncLogger::AsyncLogger(std::string_view name, AppenderInitList appenders,
                         ThreadPoolWptr thread_pool)
    : AsyncLogger(name, appenders.begin(), appenders.end(),
                  std::move(thread_pool)) {}

AsyncLogger::AsyncLogger(std::string_view name, AppenderSptr single_appender,
                         ThreadPoolWptr thread_pool)
    : AsyncLogger(name, {std::move(single_appender)}, std::move(thread_pool)) {}

void AsyncLogger::AppenderDoLog(const LogMsg &msg) {
  if (auto pool_ptr = thread_pool_.lock()) {
    pool_ptr->Submit(&tpn::log::AsyncLogger::BlackendDoLog, this->GetSelfSptr(),
                     AsyncLogMsg{msg});
  } else {
    TPN_THROW(LogException(
        fmt::format("Async Log: thread pool doesn't exist anymore")));
  }
}

void AsyncLogger::AppenderDoFlush() {
  if (auto pool_ptr = thread_pool_.lock()) {
    pool_ptr->Submit(&tpn::log::AsyncLogger::BlackendDoFlush,
                     this->GetSelfSptr());
  } else {
    TPN_THROW(LogException(
        fmt::format("Async Log: thread pool doesn't exist anymore")));
  }
}

void AsyncLogger::BlackendDoLog(const AsyncLogMsg &async_msg) {
  for (auto &&appender : appenders_) {
    if (appender->ShouldLog(async_msg.level)) {
      try {
        appender->Log(async_msg);
      } catch (const std::exception &ex) {
        DoErrhandler(ex.what());
      } catch (...) {
        DoErrhandler("Unkown exception in logger.");
      }
    }
  }

  if (ShouldFlush(async_msg.level)) {
    BlackendDoFlush();
  }
}

void AsyncLogger::BlackendDoFlush() {
  for (auto &&appender : appenders_) {
    try {
      appender->Flush();
    } catch (const std::exception &ex) {
      DoErrhandler(ex.what());
    } catch (...) {
      DoErrhandler("Unkown exception in logger.");
    }
  }
}

}  // namespace log

}  // namespace tpn
