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

#include <locale>

#include <utfcpp/utf8.h>

TEST_CASE("utf16to8", "[utfcpp]") {
  std::string s1  = "第一";
  std::wstring s2 = L"第二";

  std::cout << s1 << std::endl;

  std::string s3;

  try {
    s3.resize(s2.size() * 4);

    if (s2.size()) {
      char *oend = utf8::utf16to8(s2.c_str(), s2.c_str() + s2.size(), &s3[0]);
      s3.resize(oend - (&s3[0]));
    }
  } catch (const std::exception &) {
    s3.clear();
  }

  if (!s3.empty()) {
    std::cout << s3 << std::endl;
  }
}
