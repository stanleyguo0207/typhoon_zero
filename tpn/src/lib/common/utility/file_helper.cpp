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

#include "file_helper.h"

#include <thread>
#include <filesystem>

#include "config.h"
#include "chrono_wrap.h"
#include "exception_hub.h"

namespace fs = std::filesystem;

namespace tpn {

FileHelper::~FileHelper() { Close(); }

void FileHelper::Open(std::string_view path, bool truncate /* = false */) {
  Close();

  path_ = {path.data(), path.size()};

  auto *mode = truncate ? "wb" : "ab";

  auto path_orgi = fs::path{path};
  path_orgi.make_preferred();
  auto path_file = fs::absolute(path_orgi);

  try {
    int32_t max_tries =
        g_config->GetI32Default("file_open_try_times", kFileOpenTyrMaxTimes);

    int32_t interval = g_config->GetI32Default(
        "file_open_interval_milliseconds", kFileOpenIntervalMill);

    for (auto tries = 0; tries < max_tries; tries++) {
      if (!fs::exists(path_file.parent_path())) {
        fs::create_directories(path_file.parent_path());
      }
      file_ = std::fopen(path_file.generic_string().c_str(), mode);
      if (nullptr != file_) {
        return;
      }
      std::this_thread::sleep_for(MilliSeconds(interval));
    }
  } catch (const fs::filesystem_error &e) {
    TPN_THROW(FileException(fmt::format(
        "FileHelper::Open failed, path : {}. what() : {}", path_, e.what())));
  } catch (const std::exception) {
    TPN_THROW(FileException(
        fmt::format("FileHelper::Open failed, path : {}.", path_)));
  }

  TPN_THROW(FileException(
      fmt::format("FileHelper::Open failed, Over times, path : {}.", path_)));
}

void FileHelper::Reopen(bool truncate) {
  if (path_.empty()) {
    TPN_THROW(
        FileException("FileHelper::Reopen failed, was not opened before."));
  }
  Open(path_, truncate);
}

void FileHelper::Flush() { std::fflush(file_); }

void FileHelper::Close() {
  if (nullptr != file_) {
    std::fclose(file_);
    file_ = nullptr;
  }
}

void FileHelper::Write(const FmtMemoryBuf &buf) {
  size_t size = buf.size();
  if (size != std::fwrite(buf.data(), sizeof(char), size, file_)) {
    TPN_THROW(FileException(
        fmt::format("FileHelper::Write failed, path {}.", path_)));
  }
}

size_t FileHelper::Size() const {
  if (nullptr == file_) {
    TPN_THROW(FileException(
        fmt::format("FileHelper::Size failed, {} is closed.", path_)));
  }

  try {
    auto path_orgi = fs::path{path_};
    path_orgi.make_preferred();
    return fs::file_size(path_orgi);
  } catch (const fs::filesystem_error &e) {
    TPN_THROW(FileException(fmt::format(
        "FileHelper::Size failed, path {}, what(): {}", path_, e.what())));
  }
}

std::string_view FileHelper::GetPath() const { return path_; }

std::pair<std::string, std::string> FileHelper::SplitByExtension(
    std::string_view path) {
  auto exist = path.rfind('.');

  if (exist == std::string_view::npos || 0 == exist ||
      exist == path.size() - 1) {
    return std::make_pair(std::string{path.data(), path.size()}, std::string());
  }

  auto folder_index = path.rfind(fs::path::preferred_separator);
  if (folder_index != std::string_view::npos && folder_index >= exist - 1) {
    return std::make_pair(std::string{path.data(), path.size()}, std::string());
  }

  std::string_view parent_path = path.substr(0, exist);
  std::string_view extension   = path.substr(exist);

  return std::make_pair(std::string{parent_path.data(), parent_path.size()},
                        std::string{extension.data(), extension.size()});
}

}  // namespace tpn
