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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STATE_STATE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STATE_STATE_H_

#include "../../../test_include.h"

namespace state {

class Context;
using ContextSptr = std::shared_ptr<Context>;

class State {
 public:
  virtual ~State()                     = default;
  virtual void Handle(ContextSptr ctx) = 0;
};

using StateSptr = std::shared_ptr<State>;

class ConcreteState1;
class ConcreteState2;
class ConcreteState3;

class ConcreteState1 : public State {
 public:
  ConcreteState1() { std::cout << "ConcreteState1 ctor..." << std::endl; }
  ~ConcreteState1() { std::cout << "ConcreteState1 dtor..." << std::endl; }

  void Handle(ContextSptr ctx) override;
};

class ConcreteState2 : public State {
 public:
  ConcreteState2() { std::cout << "ConcreteState2 ctor..." << std::endl; }
  ~ConcreteState2() { std::cout << "ConcreteState2 dtor..." << std::endl; }

  void Handle(ContextSptr ctx) override;
};

class ConcreteState3 : public State {
 public:
  ConcreteState3() { std::cout << "ConcreteState3 ctor..." << std::endl; }
  ~ConcreteState3() { std::cout << "ConcreteState3 dtor..." << std::endl; }

  void Handle(ContextSptr ctx) override;
};

//////////////////////////////////////////////////////////////////////////

class Context : public std::enable_shared_from_this<Context> {
 public:
  Context(StateSptr state_sptr) : state_sptr_(std::move(state_sptr)) {
    std::cout << "Context ctor..." << std::endl;
  }
  ~Context() { std::cout << "Context dtor..." << std::endl; }

  void Request() {
    std::cout << "Context Request..." << std::endl;
    if (this->state_sptr_) {
      this->state_sptr_->Handle(shared_from_this());
    }
  }

  void ChangeState(StateSptr state_sptr) {
    std::cout << "Context ChangeState..." << std::endl;
    this->state_sptr_ = std::move(state_sptr);
  }

 private:
  StateSptr state_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void ConcreteState1::Handle(ContextSptr ctx) {
  std::cout << "ConcreteState1 Handle..." << std::endl;
  ctx->ChangeState(std::make_shared<ConcreteState2>());
}

void ConcreteState2::Handle(ContextSptr ctx) {
  std::cout << "ConcreteState2 Handle..." << std::endl;
  ctx->ChangeState(std::make_shared<ConcreteState3>());
}

void ConcreteState3::Handle(ContextSptr ctx) {
  std::cout << "ConcreteState3 Handle..." << std::endl;
  ctx->ChangeState(std::make_shared<ConcreteState1>());
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "state//////////////////////////////////////////////////////////"
               "////////////////"
            << std::endl;
  std::cout << std::endl;

  ContextSptr ctx =
      std::make_shared<Context>(std::make_shared<ConcreteState1>());
  ctx->Request();
  ctx->Request();
  ctx->Request();
  ctx->Request();
  ctx->Request();
  ctx->Request();

  std::cout << "Context use : " << ctx.use_count() << std::endl;
}

}  // namespace state

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STATE_STATE_H_
