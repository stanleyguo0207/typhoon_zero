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

#include "platform.h"

#if defined(_WIN32)
#  include <direct.h>
#  include <io.h>
#  include <process.h>
#  include <windows.h>
#else
#  include <fcntl.h>
#  include <unistd.h>

#  if defined(__linux__)
#    include <sys/syscall.h>
#    include <sys/time.h>
#  endif
#endif

namespace tpn {

namespace {

size_t GetCurrentThreadIdInternal() noexcept {
#if defined(_WIN32)
  return static_cast<size_t>(::GetCurrentThreadId());
#elif defined(__linux__)
#  if defined(__ANDROID__) && defined(__ANDROID_API__) && (__ANDROID_API__ < 21)
#    define SYS_gettid __NR_gettid
#  endif
  return static_cast<size_t>(::syscall(SYS_gettid));
#else
  return static_cast<size_t>(
      std::hash<std::thread::id>()(std::this_thread::get_id()));
#endif
}

}  // namespace

size_t GetCurrentThreadId() noexcept {
  static thread_local const size_t s_current_thread_id =
      GetCurrentThreadIdInternal();
  return s_current_thread_id;
}

std::tm Localtime(const std::time_t &time_tt) noexcept {
  std::tm tm{};
#if defined(_WIN32)
  ::localtime_s(&tm, &time_tt);
#else
  ::localtime_r(&time_tt, &tm);
#endif
  return tm;
}

std::tm Localtime() noexcept {
  std::time_t now_t = ::time(nullptr);
  return Localtime(now_t);
}

std::tm GmTime(const time_t &time_tt) noexcept {
  std::tm tm{};
#if defined(_WIN32)
  ::gmtime_s(&tm, &time_tt);
#else
  ::gmtime_r(&time_tt, &tm);
#endif
  return tm;
}

std::tm GmTime() noexcept {
  std::time_t now_t = ::time(nullptr);
  return GmTime(now_t);
}

}  // namespace tpn
