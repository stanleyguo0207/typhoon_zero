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

if(NOT CONF_DIR)
	set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/config)
  message(STATUS "UNIX: Using default configuration directory ${CONF_DIR}")
endif()

if(NOT LIB_DIR)
	set(LIB_DIR ${CMAKE_INSTALL_PREFIX}/lib)
  message(STATUS "UNIX: Using default library directory ${LIB_DIR}")
endif()

configure_file(
	"${CMAKE_SOURCE_DIR}/tpn/cmake/platform/unix/cmake_uninstall.in.cmake"
	"${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
	@ONLY
	)
message(STATUS "UNIX: Configuring uninstall target")

add_custom_target(uninstall
	"${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake"
	)
message(STATUS "UNIX: Created uninstall target")

message(STATUS "UNIX: Detected compiler: ${CMAKE_C_COMPILER}")
if(CMAKE_C_COMPILER MATCHES "gcc" OR CMAKE_C_COMPILER_ID STREQUAL "GNU")
	include(${CMAKE_SOURCE_DIR}/tpn/cmake/compiler/gcc/settings.cmake)
else()
	target_compile_definitions(typhoon-compile-option-interface
		INTERFACE
			-D_BUILD_DIRECTIVE="${CMAKE_BUILD_TYPE}"
		)
	message(WARNING "UNIX: Compiler not use gnu")
endif()
