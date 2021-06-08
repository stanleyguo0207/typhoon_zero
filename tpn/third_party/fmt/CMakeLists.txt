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

# check symbol
include(CheckSymbolExists)
if(WIN32)
	check_symbol_exists(open io.h HAVE_OPEN)
else()
	check_symbol_exists(open fcntl.h HAVE_OPEN)
endif()

# source collect
set(FMT_SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/chrono.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/color.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/compile.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/core.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/format.cc
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/format.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/format-inl.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/locale.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/ostream.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/printf.h
	${CMAKE_CURRENT_SOURCE_DIR}/fmt/ranges.h
	)

if(HAVE_OPEN)
	set(FMT_SOURCES
		${FMT_SOURCES}
		${CMAKE_CURRENT_SOURCE_DIR}/fmt/os.cc
		${CMAKE_CURRENT_SOURCE_DIR}/fmt/os.h
		${CMAKE_CURRENT_SOURCE_DIR}/fmt/posix.h
		)
endif()

# library
add_library(fmt
	STATIC
		${FMT_SOURCES}
	)

# include
target_include_directories(fmt
	PUBLIC
		${CMAKE_CURRENT_SOURCE_DIR}
	)

# features
target_compile_features(fmt
	PUBLIC
		cxx_override
		cxx_rvalue_references
		cxx_deleted_functions
	)

# link library
target_link_libraries(fmt
	PRIVATE
		typhoon-dependency-interface
	)

# ide folder
set_target_properties(fmt
	PROPERTIES
		FOLDER
			"third_party"
	)
