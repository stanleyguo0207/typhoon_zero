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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_FACTORY_METHOD_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_FACTORY_METHOD_H_

#include "../../../test_include.h"

namespace factory_method {

class Product {
 public:
  virtual ~Product()         = default;
  virtual void Description() = 0;
};

using ProductSptr = std::shared_ptr<Product>;

class ConcreteProduct1 : public Product {
 public:
  ConcreteProduct1() { std::cout << "ConcreteProduct1 ctor..." << std::endl; }
  ~ConcreteProduct1() { std::cout << "ConcreteProduct1 dtor..." << std::endl; }

  void Description() override {
    std::cout << "This is ConcreteProduct1..." << std::endl;
  }
};

class ConcreteProduct2 : public Product {
 public:
  ConcreteProduct2() { std::cout << "ConcreteProduct2 ctor..." << std::endl; }
  ~ConcreteProduct2() { std::cout << "ConcreteProduct2 dtor..." << std::endl; }

  void Description() override {
    std::cout << "This is ConcreteProduct2..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class Factory {
 public:
  virtual ~Factory()                  = default;
  virtual ProductSptr CreateProduct() = 0;
};

class ConcreteFactory1 : public Factory {
 public:
  ConcreteFactory1() { std::cout << "ConcreteFactory1 ctor..." << std::endl; }

  ~ConcreteFactory1() { std::cout << "ConcreteFactory1 dtor..." << std::endl; }

  ProductSptr CreateProduct() override {
    return std::make_shared<ConcreteProduct1>();
  }
};

class ConcreteFactory2 : public Factory {
 public:
  ConcreteFactory2() { std::cout << "ConcreteFactory2 ctor..." << std::endl; }

  ~ConcreteFactory2() { std::cout << "ConcreteFactory2 dtor..." << std::endl; }

  ProductSptr CreateProduct() override {
    return std::make_shared<ConcreteProduct2>();
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;

  auto factory1 = std::make_shared<ConcreteFactory1>();
  auto product1 = factory1->CreateProduct();
  product1->Description();

  auto factory2 = std::make_shared<ConcreteFactory2>();
  auto product2 = factory2->CreateProduct();
  product2->Description();
}

}  // namespace factory_method

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_FACTORY_METHOD_H_
