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

if(NOT Catch2)
	set(Catch2_DIR "${CMAKE_SOURCE_DIR}/tpn/third_party/catch2/lib/cmake/Catch2")
	find_package(Catch2 REQUIRED)
	if(Catch2_FOUND)
		message(STATUS "TEST: found Catch2: ${Catch2_VERSION}")
		list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/tpn/third_party/catch2/lib/cmake")
	endif()
endif()

add_subdirectory(third_party)
add_subdirectory(lib)
