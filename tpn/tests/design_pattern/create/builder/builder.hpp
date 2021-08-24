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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_BUILDER_BUILDER_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_BUILDER_BUILDER_H_

#include "../../../test_include.h"

namespace builder {

class Product {
 public:
  Product() { std::cout << "Product ctor..." << std::endl; }
  ~Product() { std::cout << "Product dtor..." << std::endl; }

  void SetPartA(std::string_view strv) {
    this->part_a_.assign(strv.data(), strv.size());
  }

  void SetPartB(std::string_view strv) {
    this->part_b_.assign(strv.data(), strv.size());
  }

  void SetPartC(std::string_view strv) {
    this->part_c_.assign(strv.data(), strv.size());
  }

 private:
  std::string part_a_;
  std::string part_b_;
  std::string part_c_;
};

using ProductSptr = std::shared_ptr<Product>;

//////////////////////////////////////////////////////////////////////////

class Builder {
 public:
  virtual ~Builder() = default;

  virtual void BuildPartA()        = 0;
  virtual void BuildPartB()        = 0;
  virtual void BuildPartC()        = 0;
  virtual ProductSptr GetProduct() = 0;
};

using BuilderSptr = std::shared_ptr<Builder>;

class ConcreteBuilder1 : public Builder {
 public:
  ConcreteBuilder1() {
    std::cout << "ConcreteBuilder1 ctor..." << std::endl;
    this->product_sptr_ = std::make_shared<Product>();
  }

  ~ConcreteBuilder1() { std::cout << "ConcreteBuilder1 dtor..." << std::endl; }

  void BuildPartA() override {
    std::cout << "ConcreteBuilder1 BuildPartA..." << std::endl;
    this->product_sptr_->SetPartA("A");
  }

  void BuildPartB() override {
    std::cout << "ConcreteBuilder1 BuildPartB..." << std::endl;
    this->product_sptr_->SetPartB("B");
  }

  void BuildPartC() override {
    std::cout << "ConcreteBuilder1 BuildPartC..." << std::endl;
    this->product_sptr_->SetPartC("C");
  }

  ProductSptr GetProduct() override { return this->product_sptr_; }

 private:
  ProductSptr product_sptr_{nullptr};
};

class ConcreteBuilder2 : public Builder {
 public:
  ConcreteBuilder2() {
    std::cout << "ConcreteBuilder2 ctor..." << std::endl;
    this->product_sptr_ = std::make_shared<Product>();
  }

  ~ConcreteBuilder2() { std::cout << "ConcreteBuilder2 dtor..." << std::endl; }

  void BuildPartA() override {
    std::cout << "ConcreteBuilder2 BuildPartA..." << std::endl;
    this->product_sptr_->SetPartA("A");
  }

  void BuildPartB() override {
    std::cout << "ConcreteBuilder2 BuildPartB..." << std::endl;
    this->product_sptr_->SetPartB("B");
  }

  void BuildPartC() override {
    std::cout << "ConcreteBuilder2 BuildPartC..." << std::endl;
    this->product_sptr_->SetPartC("C");
  }

  ProductSptr GetProduct() override { return this->product_sptr_; }

 private:
  ProductSptr product_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

class Director {
 public:
  Director(BuilderSptr builder_sptr) : builder_sptr_(std::move(builder_sptr)) {}
  ~Director() {}

  void Construct() {
    this->builder_sptr_->BuildPartA();
    this->builder_sptr_->BuildPartB();
    this->builder_sptr_->BuildPartC();
  }

 private:
  BuilderSptr builder_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "builder////////////////////////////////////////////////////////"
               "//////////////////"
            << std::endl;
  std::cout << std::endl;

  auto director1 =
      std::make_shared<Director>(std::make_shared<ConcreteBuilder1>());
  director1->Construct();

  auto director2 =
      std::make_shared<Director>(std::make_shared<ConcreteBuilder2>());
  director2->Construct();
}

}  // namespace builder

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_CREATE_BUILDER_BUILDER_H_
