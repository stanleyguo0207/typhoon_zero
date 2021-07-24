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

#include <cstring>

#include <algorithm>

#if defined(_WIN32)
#  include <direct.h>
#  include <io.h>
#  include <process.h>

#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
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

bool IsColorTerminal() noexcept {
#ifdef _WIN32
  return true;
#else
  static constexpr std::array<const char *, 14> terms = {
      {"ansi", "color", "console", "cygwin", "gnome", "konsole", "kterm",
       "linux", "msys", "putty", "rxvt", "screen", "vt100", "xterm"}};

  const char *env_p = std::getenv("TERM");
  if (env_p == nullptr) {
    return false;
  }

  static const bool result = std::any_of(
      terms.begin(), terms.end(),
      [&](const char *term) { return std::strstr(env_p, term) != nullptr; });
  return result;
#endif
}

bool InTerminal(FILE *file) noexcept {
#ifdef _WIN32
  return 0 != ::_isatty(_fileno(file));
#else
  return 0 != ::isatty(fileno(file));
#endif
}

}  // namespace tpn
