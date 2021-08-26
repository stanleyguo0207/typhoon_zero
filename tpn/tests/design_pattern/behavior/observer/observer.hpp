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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_OBSERVER_OBSERVER_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_OBSERVER_OBSERVER_H_

#include "../../../test_include.h"

namespace observer {

class Subject;

class Observer {
 public:
  virtual ~Observer()                       = default;
  virtual void Update(Subject *subject_ptr) = 0;
};

using ObserverSptr = std::shared_ptr<Observer>;

class ConcreteObserver1 : public Observer {
 public:
  ConcreteObserver1() { std::cout << "ConcreteObserver1 ctor..." << std::endl; }
  ~ConcreteObserver1() {
    std::cout << "ConcreteObserver1 dtor..." << std::endl;
  }

  void Update(Subject *subject_ptr) override;

 private:
  std::string state_;
};

class ConcreteObserver2 : public Observer {
 public:
  ConcreteObserver2() { std::cout << "ConcreteObserver2 ctor..." << std::endl; }
  ~ConcreteObserver2() {
    std::cout << "ConcreteObserver2 dtor..." << std::endl;
  }

  void Update(Subject *subject_ptr) override;

 private:
  std::string state_;
};

//////////////////////////////////////////////////////////////////////////

class Subject {
 public:
  virtual ~Subject() = default;

  virtual void Notify() {
    std::cout << "Subject Notify..." << std::endl;
    for (auto &&observer : observer_list_) {
      observer->Update(this);
    }
  }

  virtual void Attach(ObserverSptr observer_sptr) {
    std::cout << "Subject Attach before " << this->observer_list_.size()
              << "..." << std::endl;
    this->observer_list_.emplace_back(std::move(observer_sptr));
    std::cout << "Subject Attach after " << this->observer_list_.size() << "..."
              << std::endl;
  }

  virtual void Detach(ObserverSptr observer_sptr) {
    std::cout << "Subject Detach before " << this->observer_list_.size()
              << "..." << std::endl;
    if (auto iter = std::find(std::begin(this->observer_list_),
                              std::end(this->observer_list_), observer_sptr);
        iter != this->observer_list_.end()) {
      this->observer_list_.erase(iter);
    }
    std::cout << "Subject Detach after " << this->observer_list_.size() << "..."
              << std::endl;
  }

  virtual std::string GetState() {
    std::cout << "Subject GetState..." << std::endl;
    return {this->state_.data(), this->state_.size()};
  }

  virtual void SetState(std::string_view state) {
    std::cout << "Subject SetState before " << this->state_ << "..."
              << std::endl;

    this->state_.assign(state.begin(), state.end());

    std::cout << "Subject SetState after " << this->state_ << "..."
              << std::endl;
  }

 private:
  std::string state_;
  std::list<ObserverSptr> observer_list_;
};

class ConcreteSubject1 : public Subject {
 public:
  ConcreteSubject1() { std::cout << "ConcreteSubject1 ctor..." << std::endl; }
  ~ConcreteSubject1() { std::cout << "ConcreteSubject1 dtor..." << std::endl; }
};

class ConcreteSubject2 : public Subject {
 public:
  ConcreteSubject2() { std::cout << "ConcreteSubject2 ctor..." << std::endl; }
  ~ConcreteSubject2() { std::cout << "ConcreteSubject2 dtor..." << std::endl; }
};

//////////////////////////////////////////////////////////////////////////

void ConcreteObserver1::Update(Subject *subject_ptr) {
  std::cout << "ConcreteObserver1 Update before" << this->state_ << "..."
            << std::endl;
  this->state_ = subject_ptr->GetState();
  std::cout << "ConcreteObserver1 Update after " << this->state_ << "..."
            << std::endl;
}

void ConcreteObserver2::Update(Subject *subject_ptr) {
  std::cout << "ConcreteObserver2 Update before" << this->state_ << "..."
            << std::endl;
  this->state_ = subject_ptr->GetState();
  std::cout << "ConcreteObserver2 Update after " << this->state_ << "..."
            << std::endl;
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "observer///////////////////////////////////////////////////////"
               "///////////////////"
            << std::endl;
  std::cout << std::endl;

  ObserverSptr ob1 = std::make_shared<ConcreteObserver1>();
  ObserverSptr ob2 = std::make_shared<ConcreteObserver2>();
  ObserverSptr ob3 = std::make_shared<ConcreteObserver1>();

  std::shared_ptr<Subject> subject_sptr = std::make_shared<ConcreteSubject1>();
  subject_sptr->Attach(ob1);
  subject_sptr->Attach(ob2);
  subject_sptr->Attach(ob3);

  subject_sptr->SetState("old");
  subject_sptr->Notify();

  subject_sptr->SetState("new");
  subject_sptr->Detach(ob3);
  subject_sptr->Notify();

  std::cout << "ob1 use : " << ob1.use_count() << std::endl;
  std::cout << "ob2 use : " << ob2.use_count() << std::endl;
  std::cout << "ob3 use : " << ob3.use_count() << std::endl;
  std::cout << "subject_sptr use : " << subject_sptr.use_count() << std::endl;
}

}  // namespace observer

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_OBSERVER_OBSERVER_H_
