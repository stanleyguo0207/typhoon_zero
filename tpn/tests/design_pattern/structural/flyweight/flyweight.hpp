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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FLYWEIGHT_FLYWEIGHT_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FLYWEIGHT_FLYWEIGHT_H_

namespace flyweight {

class Flyweight {
 public:
  virtual ~Flyweight() = default;

  virtual void Operation(std::string_view extrinsic_state) = 0;

  std::string_view GetIntrinsicState() {
    std::cout << "Flyweight GetIntrinsicState..." << std::endl;
    return this->intrinsic_state_;
  }

 protected:
  Flyweight(std::string_view intrinsic_state) {
    std::cout << "Flyweight " << intrinsic_state << "..." << std::endl;
    this->intrinsic_state_.assign(intrinsic_state.data(),
                                  intrinsic_state.size());
  }

 private:
  std::string intrinsic_state_;
};

using FlyweightSptr = std::shared_ptr<Flyweight>;

class ConcreteFlyweight : public Flyweight {
 public:
  ConcreteFlyweight(std::string_view intrinsic_state)
      : Flyweight(intrinsic_state) {
    std::cout << "ConcreteFlyweight ctor..." << std::endl;
  }
  ~ConcreteFlyweight() {
    std::cout << "ConcreteFlyweight dtor..." << std::endl;
  }

  void Operation(std::string_view extrinsic_state) override {
    std::cout << "ConcreteFlyweight Operation..." << std::endl;
    std::cout << "intrinsic :" << this->GetIntrinsicState() << std::endl;
    std::cout << "extrinsic :" << extrinsic_state << std::endl;
  }
};

class UnsharedConcreteFlyweight : public Flyweight {
 public:
  UnsharedConcreteFlyweight(std::string_view intrinsic_state)
      : Flyweight(intrinsic_state) {
    std::cout << "UnsharedConcreteFlyweight ctor..." << std::endl;
  }
  ~UnsharedConcreteFlyweight() {
    std::cout << "UnsharedConcreteFlyweight dtor..." << std::endl;
  }

  void Operation(std::string_view extrinsic_state) override {
    std::cout << "UnsharedConcreteFlyweight Operation..." << std::endl;
    std::cout << "extrinsic :" << extrinsic_state << std::endl;
  }
};

class FlyweightFactory {
 public:
  FlyweightFactory() { std::cout << "FlyweightFactory ctor..." << std::endl; }
  ~FlyweightFactory() { std::cout << "FlyweightFactory dtor..." << std::endl; }

  FlyweightSptr GetFlyweight(std::string_view key) {
    std::cout << "FlyweightFactory GetFlyweight..." << std::endl;
    if (auto iter = std::find_if(
            std::begin(this->flyweight_sptr_vec_),
            std::end(this->flyweight_sptr_vec_),
            [&](const auto &_la) { return _la->GetIntrinsicState() == key; });
        this->flyweight_sptr_vec_.end() != iter) {
      return *iter;
    }

    auto fly = std::make_shared<ConcreteFlyweight>(key);
    this->flyweight_sptr_vec_.emplace_back(fly);
    return fly;
  }

  void GetFlyweightCount() {
    std::cout << "FlyweightFactory GetFlyweightCount "
              << this->flyweight_sptr_vec_.size() << "..." << std::endl;
  }

 private:
  std::vector<FlyweightSptr> flyweight_sptr_vec_;
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "flyweight//////////////////////////////////////////////////////"
               "////////////////////"
            << std::endl;
  std::cout << std::endl;

  std::string extrinsic_state = "ext";

  auto fc   = std::make_shared<FlyweightFactory>();
  auto fly1 = fc->GetFlyweight("hello");
  auto fly2 = fc->GetFlyweight("hello");

  fly1->Operation(extrinsic_state);
  fc->GetFlyweightCount();
}

}  // namespace flyweight

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_FLYWEIGHT_FLYWEIGHT_H_
