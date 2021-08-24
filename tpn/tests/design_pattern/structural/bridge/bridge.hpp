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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_BRIDGE_BRIDGE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_BRIDGE_BRIDGE_H_

#include "../../../test_include.h"

namespace bridge {

class AbstractionImplement {
 public:
  virtual ~AbstractionImplement() = default;
  virtual void Operation()        = 0;
};

using AbstractionImplementSptr = std::shared_ptr<AbstractionImplement>;

//////////////////////////////////////////////////////////////////////////

class Abstraction {
 public:
  virtual ~Abstraction() = default;
  virtual void Operation() = 0;
};

class RefinedAbstraction1 : public Abstraction {
 public:
  RefinedAbstraction1(AbstractionImplementSptr impl) : impl_(std::move(impl)) {
    std::cout << "RefinedAbstraction1 ctor..." << std::endl;
  }
  ~RefinedAbstraction1() {
    std::cout << "RefinedAbstraction1 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "RefinedAbstraction1 Operation..." << std::endl;
    this->impl_->Operation();
  }

 private:
  AbstractionImplementSptr impl_{nullptr};
};

class RefinedAbstraction2 : public Abstraction {
 public:
  RefinedAbstraction2(AbstractionImplementSptr impl) : impl_(std::move(impl)) {
    std::cout << "RefinedAbstraction2 ctor..." << std::endl;
  }
  ~RefinedAbstraction2() {
    std::cout << "RefinedAbstraction2 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "RefinedAbstraction2 Operation..." << std::endl;
    this->impl_->Operation();
  }

 private:
  AbstractionImplementSptr impl_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

class AbstractionImplement1 : public AbstractionImplement {
 public:
  AbstractionImplement1() {
    std::cout << "AbstractionImplement1 ctor..." << std::endl;
  }

  ~AbstractionImplement1() {
    std::cout << "AbstractionImplement1 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "AbstractionImplement1 Operation..." << std::endl;
  }
};

class AbstractionImplement2 : public AbstractionImplement {
 public:
  AbstractionImplement2() {
    std::cout << "AbstractionImplement2 ctor..." << std::endl;
  }

  ~AbstractionImplement2() {
    std::cout << "AbstractionImplement2 dtor..." << std::endl;
  }

  void Operation() override {
    std::cout << "AbstractionImplement2 Operation..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "bridge/////////////////////////////////////////////////////////"
               "/////////////////"
            << std::endl;
  std::cout << std::endl;

  {
    auto abs = std::make_shared<RefinedAbstraction1>(
        std::make_shared<AbstractionImplement1>());
    abs->Operation();
  }

  {
    auto abs = std::make_shared<RefinedAbstraction1>(
        std::make_shared<AbstractionImplement2>());
    abs->Operation();
  }

  {
    auto abs = std::make_shared<RefinedAbstraction2>(
        std::make_shared<AbstractionImplement1>());
    abs->Operation();
  }

  {
    auto abs = std::make_shared<RefinedAbstraction2>(
        std::make_shared<AbstractionImplement2>());
    abs->Operation();
  }
}

}  // namespace bridge

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_BRIDGE_BRIDGE_H_
