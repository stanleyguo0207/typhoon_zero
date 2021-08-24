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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_PROTOTYPE_PROTOTYPE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_PROTOTYPE_PROTOTYPE_H_

#include "../../../test_include.h"

namespace prototype {

class Prototype;
using PrototypeSptr = std::shared_ptr<Prototype>;

class Prototype {
 public:
  virtual ~Prototype()                = default;
  virtual PrototypeSptr Clone() const = 0;
};

class ConcretePrototype1 : public Prototype {
 public:
  ConcretePrototype1() {
    std::cout << "ConcretePrototype1 ctor..." << std::endl;
  }

  ~ConcretePrototype1() {
    std::cout << "ConcretePrototype1 dtor..." << std::endl;
  }

  ConcretePrototype1(const ConcretePrototype1 &other) {
    std::cout << "ConcretePrototype1 copy ctor..." << std::endl;
  }

  PrototypeSptr Clone() const override {
    return std::make_shared<ConcretePrototype1>(*this);
  }
};

class ConcretePrototype2 : public Prototype {
 public:
  ConcretePrototype2() {
    std::cout << "ConcretePrototype2 ctor..." << std::endl;
  }

  ~ConcretePrototype2() {
    std::cout << "ConcretePrototype2 dtor..." << std::endl;
  }

  ConcretePrototype2(const ConcretePrototype2 &other) {
    std::cout << "ConcretePrototype2 copy ctor..." << std::endl;
  }

  PrototypeSptr Clone() const override {
    return std::make_shared<ConcretePrototype2>(*this);
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "prototype//////////////////////////////////////////////////////"
               "////////////////////"
            << std::endl;
  std::cout << std::endl;

  auto prototype1       = std::make_shared<ConcretePrototype1>();
  auto prototype1_clone = prototype1->Clone();

  auto prototype2       = std::make_shared<ConcretePrototype2>();
  auto prototype2_clone = prototype2->Clone();
}

}  // namespace prototype

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_PROTOTYPE_PROTOTYPE_H_
