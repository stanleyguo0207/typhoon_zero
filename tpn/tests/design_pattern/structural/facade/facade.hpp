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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FACADE_FACADE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FACADE_FACADE_H_

#include "../../../test_include.h"

namespace facade {

class SubSystem1 {
 public:
  SubSystem1() { std::cout << "SubSystem1 ctor..." << std::endl; }
  ~SubSystem1() { std::cout << "SubSystem1 dtor..." << std::endl; }

  void Operation() { std::cout << "SubSystem1 Operation..." << std::endl; }
};

using SubSystem1Sptr = std::shared_ptr<SubSystem1>;

class SubSystem2 {
 public:
  SubSystem2() { std::cout << "SubSystem2 ctor..." << std::endl; }
  ~SubSystem2() { std::cout << "SubSystem2 dtor..." << std::endl; }

  void Operation() { std::cout << "SubSystem2 Operation..." << std::endl; }
};

using SubSystem2Sptr = std::shared_ptr<SubSystem2>;

class Facade {
 public:
  Facade() {
    std::cout << "Facade ctor..." << std::endl;
    this->subsys1_sptr_ = std::make_shared<SubSystem1>();
    this->subsys2_sptr_ = std::make_shared<SubSystem2>();
  }
  ~Facade() { std::cout << "Facade dtor..." << std::endl; }

  void OperationWrapper() {
    std::cout << "Facade OperationWrapper..." << std::endl;
    this->subsys1_sptr_->Operation();
    this->subsys2_sptr_->Operation();
  }

 private:
  SubSystem1Sptr subsys1_sptr_{nullptr};
  SubSystem2Sptr subsys2_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "facade/////////////////////////////////////////////////////////"
               "/////////////////"
            << std::endl;
  std::cout << std::endl;

  auto facade = std::make_shared<Facade>();
  facade->OperationWrapper();
}

}  // namespace facade

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FACADE_FACADE_H_
