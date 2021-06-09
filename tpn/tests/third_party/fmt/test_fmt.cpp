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

#include <fmt/format.h>
#include <fmt/chrono.h>
#include <fmt/color.h>

TEST_CASE("fmt", "[fmt]") {
  using namespace std::literals::chrono_literals;
  fmt::print("Default format: {} {}\n", 42s, 100ms);
  fmt::print("strftime-like format: {:%H:%M:%S}\n", 3h + 15min + 30s);
}

TEST_CASE("color", "[fmt]") {
  fmt::print(fmt::emphasis::bold | fg(fmt::color::red),
             "Elapsed time: {0:.2f} seconds", 1.23);
}
