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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_ABSTACT_FACTORY_ABSTACT_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_ABSTACT_FACTORY_ABSTACT_H_

#include "../../../test_include.h"

namespace factory_abstact {

class AbstactProductA {
 public:
  virtual ~AbstactProductA() = default;
  virtual void Description() = 0;
};

using AbstactProductASptr = std::shared_ptr<AbstactProductA>;

class AbstactProductB {
 public:
  virtual ~AbstactProductB() = default;
  virtual void Description() = 0;
};

using AbstactProductBSptr = std::shared_ptr<AbstactProductB>;

class ConcreteAbstactProductA1 : public AbstactProductA {
 public:
  ConcreteAbstactProductA1() {
    std::cout << "ConcreteAbstactProductA1 ctor..." << std::endl;
  }
  ~ConcreteAbstactProductA1() {
    std::cout << "ConcreteAbstactProductA1 dtor..." << std::endl;
  }

  void Description() override {
    std::cout << "This is ConcreteAbstactProductA1..." << std::endl;
  }
};

class ConcreteAbstactProductA2 : public AbstactProductA {
 public:
  ConcreteAbstactProductA2() {
    std::cout << "ConcreteAbstactProductA2 ctor..." << std::endl;
  }
  ~ConcreteAbstactProductA2() {
    std::cout << "ConcreteAbstactProductA2 dtor..." << std::endl;
  }

  void Description() override {
    std::cout << "This is ConcreteAbstactProductA2..." << std::endl;
  }
};

class ConcreteAbstactProductB1 : public AbstactProductB {
 public:
  ConcreteAbstactProductB1() {
    std::cout << "ConcreteAbstactProductB1 ctor..." << std::endl;
  }
  ~ConcreteAbstactProductB1() {
    std::cout << "ConcreteAbstactProductB1 dtor..." << std::endl;
  }

  void Description() override {
    std::cout << "This is ConcreteAbstactProductB1..." << std::endl;
  }
};

class ConcreteAbstactProductB2 : public AbstactProductB {
 public:
  ConcreteAbstactProductB2() {
    std::cout << "ConcreteAbstactProductB2 ctor..." << std::endl;
  }
  ~ConcreteAbstactProductB2() {
    std::cout << "ConcreteAbstactProductB2 dtor..." << std::endl;
  }

  void Description() override {
    std::cout << "This is ConcreteAbstactProductB2..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class AbstactFactory {
 public:
  virtual ~AbstactFactory()                    = default;
  virtual AbstactProductASptr CreateProductA() = 0;
  virtual AbstactProductBSptr CreateProductB() = 0;
};

class ConcreteFactory1 : public AbstactFactory {
 public:
  ConcreteFactory1() { std::cout << "ConcreteFactory1 ctor..." << std::endl; }

  ~ConcreteFactory1() { std::cout << "ConcreteFactory1 dtor..." << std::endl; }

  AbstactProductASptr CreateProductA() override {
    return std::make_shared<ConcreteAbstactProductA1>();
  }

  AbstactProductBSptr CreateProductB() override {
    return std::make_shared<ConcreteAbstactProductB1>();
  }
};

class ConcreteFactory2 : public AbstactFactory {
 public:
  ConcreteFactory2() { std::cout << "ConcreteFactory2 ctor..." << std::endl; }

  ~ConcreteFactory2() { std::cout << "ConcreteFactory2 dtor..." << std::endl; }

  AbstactProductASptr CreateProductA() override {
    return std::make_shared<ConcreteAbstactProductA2>();
  }

  AbstactProductBSptr CreateProductB() override {
    return std::make_shared<ConcreteAbstactProductB2>();
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "factory_abstact////////////////////////////////////////////////"
               "//////////////////////////"
            << std::endl;
  std::cout << std::endl;

  auto factory1  = std::make_shared<ConcreteFactory1>();
  auto productA1 = factory1->CreateProductA();
  productA1->Description();
  auto productB1 = factory1->CreateProductB();
  productB1->Description();

  auto factory2  = std::make_shared<ConcreteFactory2>();
  auto productA2 = factory2->CreateProductA();
  productA2->Description();
  auto productB2 = factory2->CreateProductB();
  productB2->Description();
}

}  // namespace factory_abstact

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_ABSTACT_FACTORY_ABSTACT_H_
