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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_TEMPLATE_TEMPLATE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_TEMPLATE_TEMPLATE_H_

#include "../../../test_include.h"

namespace template_method {

class AbstractClass {
 public:
  ~AbstractClass() { std::cout << "AbstractClass dtor..." << std::endl; }

  void TemplateMethod() {
    std::cout << "AbstractClass TemplateMethod..." << std::endl;
    this->PrimitiveOperation1();
    this->PrimitiveOperation2();
  }

  virtual void PrimitiveOperation1() = 0;
  virtual void PrimitiveOperation2() = 0;
};

using AbstractClassSptr = std::shared_ptr<AbstractClass>;

class ConcreteClass1 : public AbstractClass {
 public:
  ConcreteClass1() { std::cout << "ConcreteClass1 ctor..." << std::endl; }
  ~ConcreteClass1() { std::cout << "ConcreteClass1 dtor..." << std::endl; }

  void PrimitiveOperation1() override {
    std::cout << "ConcreteClass1 PrimitiveOperation1..." << std::endl;
  }
  void PrimitiveOperation2() override {
    std::cout << "ConcreteClass1 PrimitiveOperation2..." << std::endl;
  }
};

class ConcreteClass2 : public AbstractClass {
 public:
  ConcreteClass2() { std::cout << "ConcreteClass2 ctor..." << std::endl; }
  ~ConcreteClass2() { std::cout << "ConcreteClass2 dtor..." << std::endl; }

  void PrimitiveOperation1() override {
    std::cout << "ConcreteClass2 PrimitiveOperation1..." << std::endl;
  }
  void PrimitiveOperation2() override {
    std::cout << "ConcreteClass2 PrimitiveOperation2..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "template "
               "method/////////////////////////////////////////////////////////"
               "/////////////////"
            << std::endl;
  std::cout << std::endl;

  AbstractClassSptr abstract = std::make_shared<ConcreteClass1>();
  abstract->TemplateMethod();

  abstract = std::make_shared<ConcreteClass2>();
  abstract->TemplateMethod();
}

}  // namespace template_method

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_TEMPLATE_TEMPLATE_H_
