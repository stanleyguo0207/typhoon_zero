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

#ifndef TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_COMMAND_COMMAND_H_
#define TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_COMMAND_COMMAND_H_

#include "../../../test_include.h"

namespace command {

class Command {
 public:
  virtual ~Command() = default;

  virtual void Execute() = 0;
};

using CommandSptr = std::shared_ptr<Command>;

class Receiver;
using ReceiverSptr = std::shared_ptr<Receiver>;

class ConcreteCommand : public Command {
 public:
  ConcreteCommand(ReceiverSptr receiver_sptr)
      : receiver_sptr_(std::move(receiver_sptr)) {
    std::cout << "ConcreteCommand ctor..." << std::endl;
  }
  ~ConcreteCommand() { std::cout << "ConcreteCommand dtor..." << std::endl; }

  void Execute() override;

 private:
  ReceiverSptr receiver_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

class Invoker {
 public:
  Invoker(CommandSptr cmd_sptr) : cmd_sptr_(std::move(cmd_sptr)) {
    std::cout << "Invoker ctor..." << std::endl;
  }
  ~Invoker() { std::cout << "Invoker dtor..." << std::endl; }

  void Invoke() {
    std::cout << "Invoker Invoke..." << std::endl;
    this->cmd_sptr_->Execute();
  }

 private:
  CommandSptr cmd_sptr_{nullptr};
};

//////////////////////////////////////////////////////////////////////////

class Receiver {
 public:
  Receiver() { std::cout << "Receiver ctor..." << std::endl; }
  ~Receiver() { std::cout << "Receiver dtor..." << std::endl; }

  void Action() { std::cout << "Receiver Action..." << std::endl; }
};

//////////////////////////////////////////////////////////////////////////

void ConcreteCommand::Execute() {
  std::cout << "ConcreteCommand Execute..." << std::endl;
  this->receiver_sptr_->Action();
}

//////////////////////////////////////////////////////////////////////////

void Test() {
  std::cout << std::endl;
  std::cout << "command///////////////////////////////////////////////////////"
               "///////////////////"
            << std::endl;
  std::cout << std::endl;

  auto recvr      = std::make_shared<Receiver>();
  CommandSptr cmd = std::make_shared<ConcreteCommand>(recvr);
  auto invr       = std::make_shared<Invoker>(cmd);
  invr->Invoke();
}

}  // namespace command

#endif  // TYPHOON_ZERO_TPN_TESTS_DESIGN_PATTERN_BEHAVIOR_COMMAND_COMMAND_H_
