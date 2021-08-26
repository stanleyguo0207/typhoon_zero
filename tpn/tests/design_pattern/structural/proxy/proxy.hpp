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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_PROXY_PROXY_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_PROXY_PROXY_H_

#include "../../..//test_include.h"

namespace proxy {

class Subject {
 public:
  virtual ~Subject()     = default;
  virtual void Request() = 0;
};

using SubjectSptr = std::shared_ptr<Subject>;

class ConcreteSubject : public Subject {
 public:
  ConcreteSubject() { std::cout << "ConcreteSubject ctor..." << std::endl; };
  ~ConcreteSubject() { std::cout << "ConcreteSubject dtor..." << std::endl; };

  void Request() override {
    std::cout << "ConcreteSubject Request..." << std::endl;
  }
};

class Proxy : public Subject {
 public:
  Proxy() { std::cout << "Proxy ctor..." << std::endl; }
  ~Proxy() { std::cout << "Proxy dtor..." << std::endl; }

  void Request() override {
    std::cout << "Proxy Request..." << std::endl;
    if (nullptr == this->subject_sptr_) {
      this->subject_sptr_ = std::make_shared<ConcreteSubject>();
    }
    this->DoSomething1();
    this->subject_sptr_->Request();
    this->DoSomething2();
  }

  void DoSomething1() { std::cout << "Proxy DoSomething1..." << std::endl; }

  void DoSomething2() { std::cout << "Proxy DoSomething2..." << std::endl; }

 private:
  SubjectSptr subject_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "proxy//////////////////////////////////////////////////////////"
               "////////////////"
            << std::endl;
  std::cout << std::endl;

  auto proxy = std::make_shared<Proxy>();
  proxy->Request();
}

}  // namespace proxy

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_PROXY_PROXY_H_
