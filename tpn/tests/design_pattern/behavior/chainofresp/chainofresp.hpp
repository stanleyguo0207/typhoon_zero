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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_CHAINOFRESP_CHAINOFRESP_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_CHAINOFRESP_CHAINOFRESP_H_

#include "../../../test_include.h"

namespace chainofresp {

class Handle;
using HandleSptr = std::shared_ptr<Handle>;

class Handle {
 public:
  virtual ~Handle() = default;

  virtual void HandleRequest() = 0;

  void SetSuccessor(HandleSptr succ) {
    std::cout << "Handle SetSuccessor..." << std::endl;
    this->succ_ = std::move(succ);
  }

  HandleSptr GetSuccessor() {
    std::cout << "Handle GetSuccessor..." << std::endl;
    return this->succ_;
  }

 protected:
  Handle() { std::cout << "Handle ctor..." << std::endl; }
  Handle(HandleSptr succ) { std::cout << "Handle ctor sptr..." << std::endl; }

 private:
  HandleSptr succ_{nullptr};
};

class ConcreteHandle1 : public Handle {
 public:
  ConcreteHandle1() { std::cout << "ConcreteHandle1 ctor..." << std::endl; }
  ConcreteHandle1(HandleSptr succ) : Handle(std::move(succ)) {
    std::cout << "ConcreteHandle1 ctor sptr..." << std::endl;
  }
  ~ConcreteHandle1() { std::cout << "ConcreteHandle1 dtor..." << std::endl; }

  void HandleRequest() override {
    std::cout << "ConcreteHandle1 HandleRequest..." << std::endl;
    if (auto succ = this->GetSuccessor()) {
      std::cout << "ConcreteHandle1 HandleRequest continue..." << std::endl;
      succ->HandleRequest();
    } else {
      std::cout << "ConcreteHandle1 HandleRequest last..." << std::endl;
    }
  }
};

class ConcreteHandle2 : public Handle {
 public:
  ConcreteHandle2() { std::cout << "ConcreteHandle2 ctor..." << std::endl; }
  ConcreteHandle2(HandleSptr succ) : Handle(std::move(succ)) {
    std::cout << "ConcreteHandle2 ctor sptr..." << std::endl;
  }
  ~ConcreteHandle2() { std::cout << "ConcreteHandle2 dtor..." << std::endl; }

  void HandleRequest() override {
    std::cout << "ConcreteHandle2 HandleRequest..." << std::endl;
    if (auto succ = this->GetSuccessor()) {
      std::cout << "ConcreteHandle2 HandleRequest continue..." << std::endl;
      succ->HandleRequest();
    } else {
      std::cout << "ConcreteHandle2 HandleRequest last..." << std::endl;
    }
  }
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "chainofresp////////////////////////////////////////////////////"
               "/////////////////////"
            << std::endl;
  std::cout << std::endl;

  HandleSptr hdl1 = std::make_shared<ConcreteHandle1>();
  HandleSptr hdl2 = std::make_shared<ConcreteHandle2>();

  hdl1->SetSuccessor(hdl2);
  hdl1->HandleRequest();
}

}  // namespace chainofresp

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_CHAINOFRESP_CHAINOFRESP_H_
