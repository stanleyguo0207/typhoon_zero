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

project(test_logger CXX)

add_executable(test_logger
	"../../test_include.h"
	"../../test_main.cpp"
	"test_logger.cpp"
	)

set_property(TARGET
	test_logger	
	APPEND
	PROPERTY
		COMPILE_DEFINITIONS
		_TPN_LOGGER_CONFIG_TEST_FILE="${CMAKE_CURRENT_SOURCE_DIR}/config_logger_test.json"
	)

target_link_libraries(test_logger
	Catch2::Catch2
	common
	)
	
install(TARGETS test_logger DESTINATION ${BIN_DIR}/tests)
include(CTest)
include(Catch)
catch_discover_tests(test_logger)

if(WIN32)
  add_custom_command(TARGET
		test_logger
    POST_BUILD
      COMMAND
			${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_SOURCE_DIR}/config_logger_test.json
			${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
  )
endif()
