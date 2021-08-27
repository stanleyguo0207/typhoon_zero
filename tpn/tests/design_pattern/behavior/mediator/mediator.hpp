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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEDIATOR_MEDIATOR_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEDIATOR_MEDIATOR_H_

#include "../../../test_include.h"

namespace mediator {

class Mediator;
using MediatorSptr = std::shared_ptr<Mediator>;

class Colleage : public std::enable_shared_from_this<Colleage> {
 public:
  virtual ~Colleage() = default;

  virtual void SetMediator(MediatorSptr mediator_sptr) {
    std::cout << "Colleage SetMediator..." << std::endl;
    this->mediator_sptr_ = std::move(mediator_sptr);
  }

  virtual void SendMsg(std::string_view msg) = 0;

  virtual void ReceiveMsg(std::string_view msg) = 0;

 protected:
  Colleage(MediatorSptr mediator_sptr)
      : mediator_sptr_(std::move(mediator_sptr)) {
    std::cout << "Colleage ctor..." << std::endl;
  }
  MediatorSptr mediator_sptr_{nullptr};
};

using ColleageSptr = std::shared_ptr<Colleage>;

class ConcreteColleage1 : public Colleage {
 public:
  ConcreteColleage1(MediatorSptr mediator_sptr)
      : Colleage(std::move(mediator_sptr)) {
    std::cout << "ConcreteColleage1 ctor..." << std::endl;
  }

  ~ConcreteColleage1() {
    std::cout << "ConcreteColleage1 dtor..." << std::endl;
  }

  void SendMsg(std::string_view msg);

  void ReceiveMsg(std::string_view msg) override {
    std::cout << "ConcreteColleage1 ReceiveMsg " << msg << "..." << std::endl;
  }
};

class ConcreteColleage2 : public Colleage {
 public:
  ConcreteColleage2(MediatorSptr mediator_sptr)
      : Colleage(std::move(mediator_sptr)) {
    std::cout << "ConcreteColleage2 ctor..." << std::endl;
  }

  ~ConcreteColleage2() {
    std::cout << "ConcreteColleage2 dtor..." << std::endl;
  }

  void SendMsg(std::string_view msg);

  void ReceiveMsg(std::string_view msg) override {
    std::cout << "ConcreteColleage2 ReceiveMsg " << msg << "..." << std::endl;
  }
};

//////////////////////////////////////////////////////////////////////////

class Mediator {
 public:
  virtual ~Mediator() = default;

  virtual void SendMsg(std::string_view msg, ColleageSptr colleage_sptr) = 0;
};

class ConcreteMediator : public Mediator {
 public:
  ConcreteMediator() { std::cout << "ConcreteMediator ctor..." << std::endl; }
  ~ConcreteMediator() { std::cout << "ConcreteMediator dtor..." << std::endl; }

  void SetColleage1(ColleageSptr colleage_sptr) {
    std::cout << "ConcreteMediator SetColleage1..." << std::endl;
    this->colleage1_sptr_ = std::move(colleage_sptr);
  }

  void SetColleage2(ColleageSptr colleage_sptr) {
    std::cout << "ConcreteMediator ConcreteColleage2..." << std::endl;
    this->colleage2_sptr_ = std::move(colleage_sptr);
  }

  void SendMsg(std::string_view msg, ColleageSptr colleage_sptr) override {
    std::cout << "ConcreteMediator SendMsg..." << std::endl;
    if (colleage_sptr == this->colleage1_sptr_) {
      this->colleage1_sptr_->ReceiveMsg(msg);
    } else if (colleage_sptr == this->colleage2_sptr_) {
      this->colleage2_sptr_->ReceiveMsg(msg);
    } else {
      std::cout << "ConcreteMediator SendMsg " << msg << " error..."
                << std::endl;
    }
  }

 private:
  ColleageSptr colleage1_sptr_{nullptr};
  ColleageSptr colleage2_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

void ConcreteColleage1::SendMsg(std::string_view msg) {
  std::cout << "ConcreteColleage1 SendMsg..." << std::endl;
  this->mediator_sptr_->SendMsg(msg, shared_from_this());
}

void ConcreteColleage2::SendMsg(std::string_view msg) {
  std::cout << "ConcreteColleage2 SendMsg..." << std::endl;
  this->mediator_sptr_->SendMsg(msg, shared_from_this());
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "mediator///////////////////////////////////////////////////////"
               "///////////////////"
            << std::endl;
  std::cout << std::endl;

  auto mediator = std::make_shared<ConcreteMediator>();

  ColleageSptr cog1 = std::make_shared<ConcreteColleage1>(mediator);
  ColleageSptr cog2 = std::make_shared<ConcreteColleage2>(mediator);

  mediator->SetColleage1(cog1);
  mediator->SetColleage2(cog2);

  cog1->SendMsg("hello");
  cog2->SendMsg("world");
}

}  // namespace mediator

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_MEDIATOR_MEDIATOR_H_
