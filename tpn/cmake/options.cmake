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

# build type
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "RelWithDebInfo")
endif()

# config run path
set(CMAKE_SKIP_BUILD_RPATH OFF)
set(CMAKE_BUILD_WITH_INSTALL_RPATH OFF)
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH ON)

# compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# module switch
option(SERVERS "Build servers" ON)
option(TOOLS "Build tools" OFF)

# PCH
if(NOPCH)
	option(USE_COREPCH "Use precompiled headers when compiling servers" OFF)
	option(USE_SCRIPTPCH "Use precompiled headers when compiling scripts" OFF)
else()
	option(USE_COREPCH "Use precompiled headers when compiling servers" ON)
	option(USE_SCRIPTPCH "Use precompiled headers when compiling scripts" ON)
endif()

# exception
if(NOEXCEPTION)
	opetion(USE_EXCEPTION "Use exception not abort" OFF)
else()
	opetion(USE_EXCEPTION "Use exception not abort" ON)
endif()
if(NOT USE_EXCEPTION)
	add_definitions(-DTPN_NO_EXCEPTION)
endif()

# dynamic library
option(WITH_DYNAMIC_LINKING "Enable dynamic library linking" OFF)

# warnings
option(WITH_WARNINGS "Show all warings during complie" OFF)

# debug
option(WITH_COREDEBUG "Include additional debug-code in core" ON)

# srouce tree
set(WITH_SOURCE_TREE "hierarchical" CACHE STRING "Build the source tree for IDE's")
set_property(CACHE WITH_SOURCE_TREE PROPERTY STRINGS no flat hierarchical hierarchical-folders)

# git
option(WITHOUT_GIT "Disable the GIT testsing routines" OFF)
