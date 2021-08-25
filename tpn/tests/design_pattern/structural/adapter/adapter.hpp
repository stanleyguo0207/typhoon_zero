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
  virtual void Request() { std::cout << "Target Request..." << std::endl; }
};

//////////////////////////////////////////////////////////////////////////

class Adaptee {
 public:
  Adaptee() { std::cout << "Adaptee ctor..." << std::endl; }
  ~Adaptee() { std::cout << "Adaptee dtor..." << std::endl; }

  void SpecificRequest() {
    std::cout << "Adaptee SpecificRequest..." << std::endl;
  }
};

using AdapteeSptr = std::shared_ptr<Adaptee>;

//////////////////////////////////////////////////////////////////////////

class Adapter1 : public Target, private Adaptee {
 public:
  Adapter1() { std::cout << "Adapter1 ctor..." << std::endl; }
  ~Adapter1() { std::cout << "Adapter1 dtor..." << std::endl; }

  void Request() override {
    std::cout << "Adapter1 Request..." << std::endl;
    this->SpecificRequest();
  }
};

class Adapter2 : public Target {
 public:
  Adapter2() {
    std::cout << "Adapter2 ctor..." << std::endl;
    this->adaptee_sptr_ = std::make_shared<Adaptee>();
  }
  Adapter2(AdapteeSptr adaptee_sptr) : adaptee_sptr_(std::move(adaptee_sptr)) {
    std::cout << "Adapter2 ctor with AdapteeSptr..." << std::endl;
  }
  ~Adapter2() { std::cout << "Adapter2 dtor..." << std::endl; }

  void Request() override {
    std::cout << "Adapter2 Request..." << std::endl;
    this->adaptee_sptr_->SpecificRequest();
  }

 private:
  AdapteeSptr adaptee_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "adapter////////////////////////////////////////////////////////"
               "//////////////////"
            << std::endl;
  std::cout << std::endl;

  {
    auto target = std::make_shared<Adapter1>();
    target->Request();
  }

  {
    auto target = std::make_shared<Adapter2>(std::make_shared<Adaptee>());
    target->Request();
  }

  {
    auto target = std::make_shared<Adapter2>();
    target->Request();
  }
}

}  // namespace adapter

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_ADAPTER_ADAPTER_H_
