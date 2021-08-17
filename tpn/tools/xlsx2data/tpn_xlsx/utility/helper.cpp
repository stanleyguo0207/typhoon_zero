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

#include "helper.h"

#include "generator_hub.h"

namespace tpn {

namespace xlsx {

namespace {

static constexpr std::string_view s_formatter_char = " ";  // 缩进方式

}  // namespace

bool WorkBookIsOutput(std::string_view file_name) {
  if (file_name.empty()) {
    return false;
  }

  return std::isalpha(file_name[0]);
}

bool SheetTitleIsOutput(std::string_view title) {
  if (title.empty()) {
    return false;
  }

  return '@' == title[0];
}

std::string GetSheetTitle(std::string_view title) {
  if (!SheetTitleIsOutput(title)) {
    return "";
  }

  title.remove_prefix(1);
  return std::string(title.data(), title.length());
}

std::string GetDataHubEntryName(std::string_view name) {
  return fmt::format("DataHubEntry{}", name);
}

std::string GetJsonTypeName(std::string_view name) {
  return fmt::format("type.googleapis.com/tpn.data.{}", name);
}

std::string CapitalizeFirstLetter(std::string_view strv) {
  std::string ret(strv.data(), strv.length());
  if (ret.empty()) {
    return ret;
  }

  ret[0] = ::toupper(ret[0]);
  return ret;
}

std::string LowercaseFirstLetter(std::string_view strv) {
  std::string ret(strv.data(), strv.length());
  if (ret.empty()) {
    return ret;
  }

  ret[0] = ::tolower(ret[0]);
  return ret;
}

Printer::Printer() {}

Printer::~Printer() {}

void Printer::Indent() { space_ += 2; }

void Printer::Outdent() {
  space_ -= 2;
  if (space_ < 0) {
    space_ = 0;
  }
}

void Printer::Reset() {
  buf_.clear();
  space_ = 0;
}

void Printer::Print(std::string_view strv) {
  if (strv.empty()) {
    return;
  }
  if (space_ > 0) {
    fmt::format_to(FmtBufferAppender(buf_), "{0:>{1}}", s_formatter_char,
                   space_);
  }

  buf_.append(strv.data(), strv.data() + strv.size());
}

void Printer::Println(std::string_view strv) {
  Print(strv);
  buf_.push_back('\n');
}

FmtMemoryBuf &Printer::GetBuf() { return buf_; }

}  // namespace xlsx

}  // namespace tpn
