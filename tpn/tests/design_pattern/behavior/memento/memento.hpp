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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEMENTO_MEMENTO_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEMENTO_MEMENTO_H_

#include "../../../test_include.h"

namespace memento {

class Memento {
 private:
  friend class Originator;

  Memento(std::string_view state) : state_{state.data(), state.size()} {
    std::cout << "Memento ctor..." << std::endl;
  }
  ~Memento() { std::cout << "Memento dtor..." << std::endl; }

  void SetState(std::string_view state) {
    std::cout << "Memento SetState " << state << "..." << std::endl;
    this->state_.assign(state.data(), state.size());
  }
  std::string_view GetState() { return this->state_; }

 private:
  std::string state_;
};

class Originator {
 public:
  Originator() { std::cout << "Originator ctor..." << std::endl; }
  Originator(std::string_view state) : state_{state.data(), state.size()} {
    std::cout << "Originator ctor " << state << "..." << std::endl;
  }
  ~Originator() { std::cout << "Originator dtor..." << std::endl; }

  void RestoreToMemento(Memento *memento_ptr) {
    std::cout << "Originator RestoreToMemento..." << std::endl;
    this->state_ = memento_ptr->GetState();
  }
  Memento *CreateMemento() {
    std::cout << "Originator CreateMemento..." << std::endl;
    return new Memento(this->state_);
  }

  void SetState(std::string_view state) {
    std::cout << "Originator SetState " << state << "..." << std::endl;
    this->state_.assign(state.data(), state.size());
  }
  std::string_view GetState() { return this->state_; }

  void Show() {
    std::cout << "Originator Show " << state_ << "..." << std::endl;
  }

 private:
  std::string state_;
};

class Caretaker {
 public:
  Caretaker() { std::cout << "Caretaker ctor..." << std::endl; }
  ~Caretaker() { std::cout << "Caretaker dtor..." << std::endl; }

  void SetMemento(Memento *memento_ptr) {
    std::cout << "Caretaker SetMemento..." << std::endl;
    this->memento_ptr_ = memento_ptr;
  }
  Memento *GetMemento() { return this->memento_ptr_; }

 private:
  Memento *memento_ptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

/// !!! error 这种模式有内存泄漏
/// 必须开放 Memento的构造函数和析构函数
void Test() {
  std::cout << std::endl;
  std::cout << "memento////////////////////////////////////////////////////////"
               "//////////////////"
            << std::endl;
  std::cout << std::endl;

  auto orig = std::make_shared<Originator>("old");
  orig->Show();

  auto taker = std::make_shared<Caretaker>();
  taker->SetMemento(orig->CreateMemento());

  orig->SetState("New");
  orig->Show();

  orig->RestoreToMemento(taker->GetMemento());
  orig->Show();
}

}  // namespace memento

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEMENTO_MEMENTO_H_
