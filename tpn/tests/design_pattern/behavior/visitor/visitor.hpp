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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_VISITOR_VISITOR_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_VISITOR_VISITOR_H_

#include "../../../test_include.h"

namespace visitor {

class Element;
using ElementSptr = std::shared_ptr<Element>;
class ConcreteElement1;
class ConcreteElement2;

class Visitor {
 public:
  virtual ~Visitor() = default;

  virtual void VisitConcreteElement1(ElementSptr element_sptr) = 0;
  virtual void VisitConcreteElement2(ElementSptr element_sptr) = 0;
};

using VisitorSptr = std::shared_ptr<Visitor>;

class ConcreteVisitor1 : public Visitor {
 public:
  ConcreteVisitor1() { std::cout << "ConcreteVisitor1 ctor..." << std::endl; }
  ~ConcreteVisitor1() { std::cout << "ConcreteVisitor1 dtor..." << std::endl; }

  void VisitConcreteElement1(ElementSptr element_sptr) override;

  void VisitConcreteElement2(ElementSptr element_sptr) override;
};

class ConcreteVisitor2 : public Visitor {
 public:
  ConcreteVisitor2() { std::cout << "ConcreteVisitor2 ctor..." << std::endl; }
  ~ConcreteVisitor2() { std::cout << "ConcreteVisitor2 dtor..." << std::endl; }

  void VisitConcreteElement1(ElementSptr element_sptr) override;

  void VisitConcreteElement2(ElementSptr element_sptr) override;
};

//////////////////////////////////////////////////////////////////////////

class Element : public std::enable_shared_from_this<Element> {
 public:
  virtual ~Element() = default;

  virtual void Accept(VisitorSptr visitor_sptr) = 0;
};

class ConcreteElement1 : public Element {
 public:
  ConcreteElement1() { std::cout << "ConcreteElement1 ctor..." << std::endl; }
  ~ConcreteElement1() { std::cout << "ConcreteElement1 dtor..." << std::endl; }

  void Accept(VisitorSptr visitor_sptr) override {
    std::cout << "ConcreteElement1 Accept..." << std::endl;
    visitor_sptr->VisitConcreteElement1(shared_from_this());
  }
};

class ConcreteElement2 : public Element {
 public:
  ConcreteElement2() { std::cout << "ConcreteElement2 ctor..." << std::endl; }
  ~ConcreteElement2() { std::cout << "ConcreteElement2 dtor..." << std::endl; }

  void Accept(VisitorSptr visitor_sptr) override {
    std::cout << "ConcreteElement2 Accept..." << std::endl;
    visitor_sptr->VisitConcreteElement2(shared_from_this());
  }
};

//////////////////////////////////////////////////////////////////////////

void ConcreteVisitor1::VisitConcreteElement1(ElementSptr element_sptr) {
  std::cout << "ConcreteVisitor1 VisitConcreteElement1..." << std::endl;
}

void ConcreteVisitor1::VisitConcreteElement2(ElementSptr element_sptr) {
  std::cout << "ConcreteVisitor1 VisitConcreteElement2..." << std::endl;
}

void ConcreteVisitor2::VisitConcreteElement1(ElementSptr element_sptr) {
  std::cout << "ConcreteVisitor2 VisitConcreteElement1..." << std::endl;
}

void ConcreteVisitor2::VisitConcreteElement2(ElementSptr element_sptr) {
  std::cout << "ConcreteVisitor2 VisitConcreteElement2..." << std::endl;
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "visitor//////////////////////////////////////////////////////"
               "///////////////////"
            << std::endl;
  std::cout << std::endl;

  VisitorSptr vis1 = std::make_shared<ConcreteVisitor1>();
  VisitorSptr vis2 = std::make_shared<ConcreteVisitor2>();
  ElementSptr elm1 = std::make_shared<ConcreteElement1>();
  ElementSptr elm2 = std::make_shared<ConcreteElement2>();

  elm1->Accept(vis1);
  elm1->Accept(vis2);

  elm2->Accept(vis1);
  elm2->Accept(vis2);
}

}  // namespace visitor

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_VISITOR_VISITOR_H_
