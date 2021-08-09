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

#include <string_view>

namespace tpn {

namespace xlsx {

namespace {

static constexpr std::string_view s_formatter_char = " ";  // 缩进方式

}  // namespace

bool SheetTitleIsOutput(std::string_view title) {
  if (title.empty()) {
    return false;
  }

  return '@' == title[0];
}

std::string GetDataHubItemName(std::string_view name) {
  return fmt::format("DataHub{}Item", name);
}

std::string GetDataHubMapName(std::string_view name) {
  return fmt::format("DataHub{}Map", name);
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
