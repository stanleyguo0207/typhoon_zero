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

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tpn/cmake/macros")

include(CheckCXXSourceRuns)
include(CheckIncludeFiles)

include(ConfigureScripts)
include(FuncAutoCollect)

include(CheckEndian)

include(${CMAKE_SOURCE_DIR}/tpn/cmake/banner.cmake)

include(${CMAKE_SOURCE_DIR}/tpn/cmake/options.cmake)

include(ConfigureBaseTargets)

include(CheckPlatform)

include(CheckBuildDir)

include(FuncGroupSources)

if(NOT WITHOUT_GIT)
	find_package(Git)
endif()

include(${CMAKE_SOURCE_DIR}/tpn/cmake/showoptions.cmake)

if(BUILD_TESTS)
	enable_testing()
endif()

add_subdirectory(third_party)

add_subdirectory(src)

if(BUILD_TESTS)
	add_subdirectory(tests)
endif()
