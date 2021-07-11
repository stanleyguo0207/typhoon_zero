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

#include "log.h"

namespace tpn {

namespace log {

void Init() { g_log_hub->Init(); }

void RegisterLogger(LoggerSptr new_logger) {
  g_log_hub->RegisterLogger(std::move(new_logger));
}

void InitializeLogger(LoggerSptr new_logger) {
  g_log_hub->InitializeLogger(std::move(new_logger));
}

LoggerSptr GetLoggerByName(std::string_view logger_name) {
  return g_log_hub->GetLoggerByName(logger_name);
}

Logger *GetDefaultLoggerRaw() { return g_log_hub->GetDefaultLoggerRaw(); }

LoggerSptr GetDefaultLogger() { return g_log_hub->GetDefaultLogger(); }

void SetDefaultLogger(LoggerSptr new_default_logger) {
  g_log_hub->SetDefaultLogger(std::move(new_default_logger));
}

void SetGlobalLevel(LogLevel level) { g_log_hub->SetGlobalLevel(level); }

void SetGlobalFlushLevel(LogLevel level) {
  g_log_hub->SetGlobalFlushLevel(level);
}

void FlushAll() { g_log_hub->FlushAll(); }

template <typename Rep, typename Period>
void FlushEvery(std::chrono::duration<Rep, Period> interval) {
  g_log_hub->FlushEvery(interval);
}

void SetErrHandler(ErrHandler err_handler) {
  g_log_hub->SetErrHandler(std::move(err_handler));
}

void ApplyAll(const std::function<void(const LoggerSptr)> &func) {
  g_log_hub->ApplyAll(func);
}

void Drop(std::string_view logger_name) { g_log_hub->Drop(logger_name); }

void DropAll() { g_log_hub->DropAll(); }

void Shutdown() { g_log_hub->Shutdown(); }

void SetAutomaticRegistration(bool automatic_registration) {
  g_log_hub->SetAutomaticRegistration(automatic_registration);
}

}  // namespace log

}  // namespace tpn
