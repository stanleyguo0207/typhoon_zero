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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STRATEGY_STRATEGY_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STRATEGY_STRATEGY_H_

namespace strategy {

class Strategy {
 public:
  ~Strategy() { std::cout << "Strategy dtor..." << std::endl; }
  virtual void AlgrithmInterface() = 0;
};

using StrategySptr = std::shared_ptr<Strategy>;

class ConcreteStrategy1 : public Strategy {
 public:
  ConcreteStrategy1() { std::cout << "ConcreteStrategy1 ctor..." << std::endl; }
  ~ConcreteStrategy1() {
    std::cout << "ConcreteStrategy1 dtor..." << std::endl;
  }

  void AlgrithmInterface() override {
    std::cout << "ConcreteStrategy1 AlgrithmInterface..." << std::endl;
  }
};

class ConcreteStrategy2 : public Strategy {
 public:
  ConcreteStrategy2() { std::cout << "ConcreteStrategy2 ctor..." << std::endl; }
  ~ConcreteStrategy2() {
    std::cout << "ConcreteStrategy2 dtor..." << std::endl;
  }

  void AlgrithmInterface() override {
    std::cout << "ConcreteStrategy2 AlgrithmInterface..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class Context {
 public:
  Context(StrategySptr strategy_sptr)
      : strategy_sptr_(std::move(strategy_sptr)) {
    std::cout << "Context ctor..." << std::endl;
  }
  ~Context() { std::cout << "Context dtor..." << std::endl; }

  void DoAction() {
    std::cout << "Context DoAction..." << std::endl;
    this->strategy_sptr_->AlgrithmInterface();
  }

 private:
  StrategySptr strategy_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "strategy///////////////////////////////////////////////////////"
               "///////////////////"
            << std::endl;
  std::cout << std::endl;

  auto ctx = std::make_shared<Context>(std::make_shared<ConcreteStrategy1>());
  ctx->DoAction();

  ctx = std::make_shared<Context>(std::make_shared<ConcreteStrategy2>());
  ctx->DoAction();
}

}  // namespace strategy

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_STRATEGY_STRATEGY_H_
