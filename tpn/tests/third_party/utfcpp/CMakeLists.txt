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

project(test_utfcpp CXX)

add_executable(test_utfcpp
	"../../test_include.h"
	"../../test_main.cpp"
	"test_utfcpp.cpp"
	)

target_link_libraries(test_utfcpp
	Catch2::Catch2
	utfcpp
	)

install(TARGETS test_utfcpp DESTINATION ${BIN_DIR}/tests)
include(CTest)
include(Catch)
catch_discover_tests(test_utfcpp)
