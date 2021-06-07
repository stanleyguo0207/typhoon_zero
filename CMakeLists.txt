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

# minimum
cmake_minimum_required(VERSION 3.20.0)

# disable in-source build
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

# project typhoon_zero
project(typhoon_zero C CXX)

# cmake compatibility
if(POLICY CMP0005)
	# Preprocessor definition values are now escaped automatically.
	cmake_policy(SET CMP0005 NEW)
endif()
if(POLICY CMP0043)
	# Ignore COMPILE_DEFINITIONS_<Config> properties
	cmake_policy(SET CMP0043 NEW)
endif()
if(POLICY CMP0054)
	# only interpret if() arguments as variables or keywords when unquoted
	cmake_policy(SET CMP0054 NEW)
endif()
if(POLICY CMP0074)
	# find_package() uses <PackageName>_ROOT variables.
	cmake_policy(SET CMP0074 NEW)
endif()

# c standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# c++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# install path
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/install" CACHE PATH "default install path" FORCE)
endif()

# project directory
add_subdirectory(tpn)
