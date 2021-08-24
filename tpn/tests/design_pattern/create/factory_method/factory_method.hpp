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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_H_

#include "../../test_include.h"

namespace factory_method {

class Product {
 public:
  virtual ~Product() = 0;

 protected:
  Product() {}
};

using ProductSptr = std::shared_ptr<Product>;

class ConcreteProduct1 : public Product {
 public:
  ConcreteProduct1() { std::cout << "ConcreteProduct1 ctor..." << std::endl; }

  ~ConcreteProduct1() { std::cout << "ConcreteProduct1 dtor..." << std::endl; }
};

class ConcreteProduct2 : public Product {
 public:
  ConcreteProduct2() { std::cout << "ConcreteProduct2 ctor..." << std::endl; }

  ~ConcreteProduct2() { std::cout << "ConcreteProduct2 dtor..." << std::endl; }
};

//////////////////////////////////////////////////////////////////////////

class Factory {
 public:
  virtual ~Factory()                  = 0;
  virtual ProductSptr CreateProduct() = 0;

 protected:
  Factory() {}
};

class ConcreteFactory : public Factory {
 public:
  ConcreteFactory() { std::cout << "ConcreteFactory ctor..." << std::endl; }

  ~ConcreteFactory() { std::cout << "ConcreteFactory ctor..." << std::endl; }

  ProductSptr CreateProduct() override {}
};

//////////////////////////////////////////////////////////////////////////

void Test() {}

}  // namespace factory_method

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_FACTORY_METHOD_H_
