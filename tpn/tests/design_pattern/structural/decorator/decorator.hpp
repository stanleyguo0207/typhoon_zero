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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_DECORATOR_DECORATOR_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_DECORATOR_DECORATOR_H_

#include "../../../test_include.h"

namespace decorator {

class Component {
 public:
  virtual ~Component()     = default;
  virtual void Operation() = 0;
};
using ComponentSptr = std::shared_ptr<Component>;

class ConcreteComponent : public Component {
 public:
  ConcreteComponent() { std::cout << "ConcreteComponent ctor..." << std::endl; }
  ~ConcreteComponent() {
    std::cout << "ConcreteComponent dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "ConcreteComponent Operation..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class Decorator : public Component {
 public:
  Decorator(ComponentSptr comp_sptr) : comp_sptr_(std::move(comp_sptr)) {
    std::cout << "Decorator ctor..." << std::endl;
  }
  virtual ~Decorator() { std::cout << "Decorator dtor..." << std::endl; }

  void Operation() override {
    std::cout << "Decorator Operation..." << std::endl;
  }

  void SetComponent(ComponentSptr comp_sptr) {
    this->comp_sptr_ = std::move(comp_sptr);
  }

 protected:
  ComponentSptr comp_sptr_{nullptr};
};

using DecoratorSptr = std::shared_ptr<Decorator>;

class ConcreteDecorator1 : public Decorator {
 public:
  ConcreteDecorator1(ComponentSptr comp_sptr)
      : Decorator(std::move(comp_sptr)) {
    std::cout << "ConcreteDecorator1 ctor..." << std::endl;
  }
  ~ConcreteDecorator1() {
    std::cout << "ConcreteDecorator1 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "ConcreteDecorator1 Operation..." << std::endl;
    this->comp_sptr_->Operation();
    this->AddBehavor1();
  }

  void AddBehavor1() {
    std::cout << "ConcreteDecorator1 AddBehavor1..." << std::endl;
  }
};

class ConcreteDecorator2 : public Decorator {
 public:
  ConcreteDecorator2(ComponentSptr comp_sptr)
      : Decorator(std::move(comp_sptr)) {
    std::cout << "ConcreteDecorator2 ctor..." << std::endl;
  }
  ~ConcreteDecorator2() {
    std::cout << "ConcreteDecorator2 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "ConcreteDecorator2 Operation..." << std::endl;
    this->comp_sptr_->Operation();
    this->AddBehavor2();
  }

  void AddBehavor2() {
    std::cout << "ConcreteDecorator2 AddBehavor2..." << std::endl;
  }
};

class ConcreteDecorator3 : public Decorator {
 public:
  ConcreteDecorator3(ComponentSptr comp_sptr)
      : Decorator(std::move(comp_sptr)) {
    std::cout << "ConcreteDecorator3 ctor..." << std::endl;
  }
  ~ConcreteDecorator3() {
    std::cout << "ConcreteDecorator3 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "ConcreteDecorator3 Operation..." << std::endl;
    this->comp_sptr_->Operation();
    this->AddBehavor3();
  }

  void AddBehavor3() {
    std::cout << "ConcreteDecorator3 AddBehavor3..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class DecoratorOnleyOne : public Component {
 public:
  DecoratorOnleyOne(ComponentSptr comp_sptr)
      : comp_sptr_(std::move(comp_sptr)) {
    std::cout << "DecoratorOnleyOne ctor..." << std::endl;
  }
  ~DecoratorOnleyOne() {
    std::cout << "DecoratorOnleyOne dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "DecoratorOnleyOne Operation..." << std::endl;
    this->comp_sptr_->Operation();
    this->AddBehavor();
  }

  void AddBehavor() {
    std::cout << "DecoratorOnleyOne AddBehavor..." << std::endl;
  }

 private:
  ComponentSptr comp_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "decorator//////////////////////////////////////////////////////"
               "////////////////////"
            << std::endl;
  std::cout << std::endl;

  {
    auto component = std::make_shared<ConcreteComponent>();

    DecoratorSptr decorator;
    decorator = std::make_shared<ConcreteDecorator1>(component);
    decorator = std::make_shared<ConcreteDecorator2>(decorator);
    decorator = std::make_shared<ConcreteDecorator3>(decorator);
    decorator->Operation();

    std::cout << "decorator use count = " << decorator.use_count() << std::endl;
  }

  {
    auto component = std::make_shared<ConcreteComponent>();
    auto decorator = std::make_shared<DecoratorOnleyOne>(std::move(component));
    decorator->Operation();
  }
}

}  // namespace decorator

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_DECORATOR_DECORATOR_H_
