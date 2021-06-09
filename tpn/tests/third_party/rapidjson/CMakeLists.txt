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

project(test_rapidjson CXX)

add_executable(test_rapidjson
	"../../test_include.h"
	"../../test_main.cpp"
	"test_rapidjson.cpp"
)

set_property(TARGET
	test_rapidjson
	APPEND
	PROPERTY
		COMPILE_DEFINITIONS
		_TPN_RAPIDJSON_TEST_FILE="${CMAKE_CURRENT_SOURCE_DIR}/configtest.json"
	)

target_link_libraries(test_rapidjson
	Catch2::Catch2
	rapidjson
)

install(TARGETS test_rapidjson DESTINATION ${BIN_DIR}/tests)
include(CTest)
include(Catch)
catch_discover_tests(test_rapidjson)
