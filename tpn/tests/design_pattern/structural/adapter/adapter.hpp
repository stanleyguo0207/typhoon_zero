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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_ADAPTER_ADAPTER_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_ADAPTER_ADAPTER_H_

#include "../../../test_include.h"

namespace adapter {

class Target {
 public:
  virtual ~Target() = default;
  virtual void Request();
};

//////////////////////////////////////////////////////////////////////////

class Adaptee {
 public:
  Adaptee() {}
  ~Adaptee() {}

  void SpecificRequest();
};

using AdapteeSptr = std::shared_ptr<Adaptee>;

//////////////////////////////////////////////////////////////////////////

class Adapter1 : public Target, private Adaptee {
 public:
  Adapter1() {}
  ~Adapter1() {}

  void Request() override {}
};

class Adapter2 : public Target {
 public:
   Adapter2(Adaptee) {}
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "adapter////////////////////////////////////////////////////////"
               "//////////////////"
            << std::endl;
  std::cout << std::endl;
}

}  // namespace adapter

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_ADAPTER_ADAPTER_H_
