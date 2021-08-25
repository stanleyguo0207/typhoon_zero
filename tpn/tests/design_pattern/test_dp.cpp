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

#include "../test_include.h"

#include "create/factory_method/factory_method.hpp"
#include "create/factory_abstact/factory_abstact.hpp"
#include "create/builder/builder.hpp"
#include "create/prototype/prototype.hpp"

TEST_CASE("factory method", "[desgin_pattern]") { factory_method::Test(); }
TEST_CASE("factory abstact", "[desgin_pattern]") { factory_abstact::Test(); }
TEST_CASE("singleton", "[desgin_pattern]") {}
TEST_CASE("builder", "[desgin_pattern]") { builder::Test(); }
TEST_CASE("prototype", "[desgin_pattern]") { prototype::Test(); }

//////////////////////////////////////////////////////////////////////////

#include "structural/bridge/bridge.hpp"
#include "structural/adapter/adapter.hpp"
#include "structural/decorator/decorator.hpp"

TEST_CASE("bridge", "[desgin_pattern]") { bridge::Test(); }
TEST_CASE("adapter", "[desgin_pattern]") { adapter::Test(); }
TEST_CASE("decorator", "[desgin_pattern]") { decorator::Test(); }
TEST_CASE("composite", "[desgin_pattern]") {}
TEST_CASE("flyweight", "[desgin_pattern]") {}
TEST_CASE("facade", "[desgin_pattern]") {}
TEST_CASE("proxy", "[desgin_pattern]") {}

//////////////////////////////////////////////////////////////////////////

TEST_CASE("template", "[desgin_pattern]") {}
TEST_CASE("strategy", "[desgin_pattern]") {}
TEST_CASE("state", "[desgin_pattern]") {}
TEST_CASE("observer", "[desgin_pattern]") {}
TEST_CASE("memento", "[desgin_pattern]") {}
TEST_CASE("mediator", "[desgin_pattern]") {}
TEST_CASE("command", "[desgin_pattern]") {}
TEST_CASE("visitor", "[desgin_pattern]") {}
TEST_CASE("chain of responsibility", "[desgin_pattern]") {}
TEST_CASE("iterator", "[desgin_pattern]") {}
TEST_CASE("interpreter", "[desgin_pattern]") {}
