#
#           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
#            │ └┬┘├─┘├─┤│ ││ ││││
#            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
#
# This file is part of the typhoon Project.
# Copyright (C) 2021 stanley0207@163.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

cmake_minimum_required(VERSION 3.20.0)

project(test_common CXX)

add_executable(test_common
	"../../test_include.h"
	"../../test_main.cpp"
	"test_common.cpp"
)

set_property(TARGET
	test_common
	APPEND
	PROPERTY
		COMPILE_DEFINITIONS
		_TPN_COMMON_CONFIG_TEST_FILE="${CMAKE_CURRENT_SOURCE_DIR}/config_common_test.json"
	)

target_link_libraries(test_common
	Catch2::Catch2
	common
)

install(TARGETS test_common DESTINATION ${BIN_DIR}/tests)
include(CTest)
include(Catch)
catch_discover_tests(test_common)
