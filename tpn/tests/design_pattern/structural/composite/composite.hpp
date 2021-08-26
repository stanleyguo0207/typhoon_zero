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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_COMPOSITE_COMPOSITE_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_COMPOSITE_COMPOSITE_H_

#include "../../../test_include.h"

namespace composite {

class Component;
using ComponentSptr = std::shared_ptr<Component>;

class Component {
 public:
  virtual ~Component()     = default;
  virtual void Operation() = 0;

  virtual void Add(ComponentSptr comp_sptr) {
    std::cout << "Component Add..." << std::endl;
  }

  virtual void Remove(ComponentSptr comp_sptr) {
    std::cout << "Component Remove..." << std::endl;
  }

  virtual ComponentSptr GetChild(int index) {
    std::cout << "Component GetChild " << index << "..." << std::endl;
    return nullptr;
  }
};

class Leaf : public Component {
 public:
  Leaf() { std::cout << "Leaf ctor..." << std::endl; }
  ~Leaf() { std::cout << "Leaf dtor..." << std::endl; }

  void Operation() override { std::cout << "Leaf Operation..." << std::endl; }
};

class Composite : public Component {
 public:
  Composite() { std::cout << "Composite ctor..." << std::endl; }
  ~Composite() { std::cout << "Composite dtor..." << std::endl; }

  void Operation() override {
    std::cout << "Composite Operation..." << std::endl;
    for (auto &&comp_sptr : comp_sptr_vec_) {
      comp_sptr->Operation();
    }
  }

  void Add(ComponentSptr comp_sptr) override {
    std::cout << "Composite Add..." << std::endl;
    this->comp_sptr_vec_.emplace_back(std::move(comp_sptr));
  }

  void Remove(ComponentSptr comp_sptr) override {
    std::cout << "Composite Remove..." << std::endl;
    this->comp_sptr_vec_.erase(
        std::remove_if(std::begin(this->comp_sptr_vec_),
                       std::end(this->comp_sptr_vec_),
                       [&](const auto &_la) { return _la == comp_sptr; }),
        std::end(this->comp_sptr_vec_));
  }

  ComponentSptr GetChild(int index) override {
    std::cout << "Composite GetChild " << index << "..." << std::endl;
    if (index < 0 || index > this->comp_sptr_vec_.size()) {
      return nullptr;
    }
    return this->comp_sptr_vec_[index];
  }

 private:
  std::vector<ComponentSptr> comp_sptr_vec_;
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "composite//////////////////////////////////////////////////////"
               "////////////////////"
            << std::endl;
  std::cout << std::endl;

  auto root = std::make_shared<Composite>();
  root->Add(std::make_shared<Leaf>());

  auto leaf1 = std::make_shared<Leaf>();
  auto leaf2 = std::make_shared<Leaf>();

  leaf1->Add(leaf2);
  leaf1->Remove(leaf2);
  leaf1->Operation();

  auto comp = std::make_shared<Composite>();
  comp->Add(leaf1);
  comp->Add(leaf2);
  comp->Operation();

  root->Add(comp);
  root->Operation();

  std::cout << "root use: " << root.use_count() << std::endl;
  std::cout << "leaf1 use: " << leaf1.use_count() << std::endl;
  std::cout << "leaf2 use: " << leaf2.use_count() << std::endl;
  std::cout << "comp use: " << comp.use_count() << std::endl;
}

}  // namespace composite

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_STRUCTURAL_COMPOSITE_COMPOSITE_H_
