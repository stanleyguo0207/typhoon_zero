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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_INTERPRETER_INTERPRETER_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_INTERPRETER_INTERPRETER_H_

#include "../../../test_include.h"

namespace interpreter {

class Context {
 public:
  Context() { std::cout << "Context ctor..." << std::endl; }
  ~Context() { std::cout << "Context dtor..." << std::endl; }
};

class AbstractExpression {
 public:
  virtual ~AbstractExpression() = default;

  virtual void Interpret(const Context &ctx) = 0;
};

using AbstractExpressionSptr = std::shared_ptr<AbstractExpression>;

class TerminalExpression : public AbstractExpression {
 public:
  TerminalExpression(std::string_view statement)
      : statement_{statement.data(), statement.size()} {
    std::cout << "TerminalExpression ctor..." << std::endl;
  }
  ~TerminalExpression() {
    std::cout << "TerminalExpression dtor..." << std::endl;
  }

  void Interpret(const Context &ctx) override {
    std::cout << "TerminalExpression Interpret " << this->statement_ << "..."
              << std::endl;
  }

 private:
  std::string statement_;
};

class NonterminalExpression : public AbstractExpression {
 public:
  NonterminalExpression(AbstractExpressionSptr expression_sptr, int times)
      : expression_sptr_(std::move(expression_sptr)), times_(times) {
    std::cout << "NonterminalExpression ctor..." << std::endl;
  }
  ~NonterminalExpression() {
    std::cout << "NonterminalExpression dtor..." << std::endl;
  }

  void Interpret(const Context &ctx) override {
    std::cout << "NonterminalExpression dtor..." << std::endl;
    for (int i = 0; i < times_; ++i) {
      this->expression_sptr_->Interpret(ctx);
    }
  }

 private:
  AbstractExpressionSptr expression_sptr_{nullptr};
  int times_{0};
};

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "interpreter////////////////////////////////////////////////////"
               "//////////////////////"
            << std::endl;
  std::cout << std::endl;

  auto ctx                   = std::make_shared<Context>();
  AbstractExpressionSptr te  = std::make_shared<TerminalExpression>("hello");
  AbstractExpressionSptr nte = std::make_shared<NonterminalExpression>(te, 2);

  nte->Interpret(*ctx);
}

}  // namespace interpreter

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_INTERPRETER_INTERPRETER_H_
