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

#include "appender_daily_file.h"

#include <filesystem>

#include "exception_hub.h"
#include "platform.h"
#include "debug_hub.h"

namespace fs = std::filesystem;

namespace tpn {

namespace log {

namespace {

/// 每个文件追加器日期计算辅助类
static std::string CalcFilename(std::string_view filename, const tm &now_tm) {
  auto &&[basename, ext] = FileHelper::SplitByExtension(filename);
  return fmt::format("{}_{:04d}-{:02d}-{:02d}{}", basename,
                     now_tm.tm_year + 1900, now_tm.tm_mon + 1, now_tm.tm_mday,
                     ext);
}

}  // namespace

AppenderDailyFile::AppenderDailyFile(std::string_view base_name,
                                     int rotation_hour, int rotation_minute,
                                     bool truncate /* = false */,
                                     uint16_t max_files /* = 7 */)
    : base_name_{base_name.data(), base_name.size()},
      rotation_h_(rotation_hour),
      rotation_m_(rotation_minute),
      truncate_(truncate),
      max_files_(max_files) {
  if (rotation_hour < 0 || rotation_hour > 23 || rotation_minute < 0 ||
      rotation_minute > 59) {
    TPN_THROW(FileException(
        fmt::format("AppenderDailyFile::AppenderDailyFile Invalid rotation "
                    "time in ctor. hour:{} minute:{}",
                    rotation_hour, rotation_minute)));
  }
  auto filename = CalcFilename(base_name_, Localtime());
  helper_.Open(filename, truncate_);
  rotation_tp_ = NextRotationTp();

  if (max_files > 0) {
    Init();
  }
}

std::string_view AppenderDailyFile::GetPath() const {
  return helper_.GetPath();
}

void AppenderDailyFile::DoLog(const LogMsg &msg) {
  bool should_ratate = msg.time >= rotation_tp_;
  if (should_ratate) {
    auto filename =
        CalcFilename(base_name_, Localtime(LogClock::to_time_t(msg.time)));
    helper_.Open(filename, truncate_);
    rotation_tp_ = NextRotationTp();
  }

  FmtMemoryBuf formatted;
  fmt::format_to(formatted, "{}{}", fmt::format("{}", msg), TPN_EOL);
  helper_.Write(formatted);

  if (should_ratate && max_files_ > 0) {
    DeleteOld();
  }
}

void AppenderDailyFile::DoFlush() { helper_.Flush(); }

void AppenderDailyFile::Init() {
  path_q_ = CircularQueue<std::string>(static_cast<size_t>(max_files_));
  std::vector<std::string> filenames;
  auto now = LogClock::now();
  while (filenames.size() < max_files_) {
    auto filename =
        CalcFilename(base_name_, Localtime(LogClock::to_time_t(now)));
    try {
      if (!fs::exists(filename)) {
        break;
      }
    } catch (...) {
      TPN_THROW(FileException(
          fmt::format("AppenderDailyFile::Init error {}", filename)));
      return;
    }
    filenames.emplace_back(filename);
    now -= 24h;
  }
  for (auto iter = filenames.rbegin(); iter != filenames.rend(); ++iter) {
    path_q_.EnQueue(std::move(*iter));
  }
}

LogClock::time_point AppenderDailyFile::NextRotationTp() {
  auto now           = LogClock::now();
  std::tm date       = Localtime(LogClock::to_time_t(now));
  date.tm_hour       = rotation_h_;
  date.tm_min        = rotation_m_;
  date.tm_sec        = 0;
  auto rotation_time = LogClock::from_time_t(std::mktime(&date));
  if (rotation_time > now) {
    return rotation_time;
  }
  return {rotation_time + 24h};
}

void AppenderDailyFile::DeleteOld() {
  auto current_file = helper_.GetPath();
  if (path_q_.IsFull()) {
    auto old_filename = std::move(path_q_.GetFront());
    path_q_.DeQueue();
    auto path_orgi = fs::path{old_filename};
    path_orgi.make_preferred();
    auto old_path = fs::absolute(path_orgi);

    try {
      if (fs::exists(old_path)) {
        if (!fs::remove(old_path)) {
          path_q_.EnQueue(
              std::string{current_file.data(), current_file.size()});
          TPN_THROW(FileException(
              fmt::format("Failed removing daily file {}", old_path)));
        }
      }
    } catch (const fs::filesystem_error &e) {
      TPN_THROW(FileException(fmt::format(
          "AppenderDailyFile::DeleteOld failed, what() : {}.", e.what())));
    } catch (const std::exception &ex) {
      TPN_THROW(FileException(fmt::format(
          "AppenderDailyFile::DeleteOld failed, what() : {}.", ex.what())));
    }
  }
  path_q_.EnQueue(std::string{current_file.data(), current_file.size()});
}

}  // namespace log

}  // namespace tpn
