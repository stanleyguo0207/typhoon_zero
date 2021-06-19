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

# #define _WIN32_WINNT_WIN7                   0x0601 // Windows 7
# #define _WIN32_WINNT_WIN8                   0x0602 // Windows 8
# #define _WIN32_WINNT_WINBLUE                0x0603 // Windows 8.1
# #define _WIN32_WINNT_WINTHRESHOLD           0x0A00 // Windows 10
# #define _WIN32_WINNT_WIN10                  0x0A00 // Windows 10
add_definitions(-D_WIN32_WINNT=0x0601)  # >= win7

add_definitions(-DWIN32_LEAN_AND_MEAN)

add_definitions(-DNOMINMAX)

if(NOT BIN_DIR)
	set(BIN_DIR ${CMAKE_INSTALL_PREFIX}/bin)
  message(STATUS "WINDOWS: Using default binary directory ${BIN_DIR}")
endif()

if(NOT CONF_DIR)
	set(CONF_DIR ${CMAKE_INSTALL_PREFIX}/config)
  message(STATUS "WINDOWS: Using default configuration directory ${CONF_DIR}")
endif()

if(NOT LIBS_DIR)
	set(LIBS_DIR ${CMAKE_INSTALL_PREFIX}/lib)
	message(STATUS "WINDOWS: Using default library directory ${LIBS_DIR}")
endif()

message(STATUS "WINDOWS: Detected compiler: ${CMAKE_C_COMPILER}")
if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  message(STATUS "Begin loading MSVC configuration")
  include(${CMAKE_SOURCE_DIR}/tpn/cmake/compiler/msvc/settings.cmake)
else()
  message(FATAL_ERROR "WINDOWS: Only use MSVC on Windows")
endif()