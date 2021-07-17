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

# collect source
CollectSourceFiles(
	${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE_SOURCES
	# Exclude
	${CMAKE_CURRENT_SOURCE_DIR}/pch
	)

# collect include
CollectIncludeDirectories(
	${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC_INCLUDES
	# Exclude
	${CMAKE_CURRENT_SOURCE_DIR}/pch
	)

# source tree
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

# library common
add_library(common
	${PRIVATE_SOURCES}
	)

# include
target_include_directories(common
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
    ${CMAKE_BINARY_DIR}
	)

# link
target_link_libraries(common
  PRIVATE
    typhoon-core-interface
	PUBLIC
    threads
    fmt
    sfmt
    rapidjson
    utfcpp
    asio
    valgrind
	)

# export
add_definitions(-DTPN_API_EXPORT_COMMON)

# dependencies
add_dependencies(common revision_data.h)

# ide folder
set_target_properties(common
	PROPERTIES
		FOLDER
			"libs"
	)

# shared library
if(BUILD_SHARED_LIBS)
  install(TARGETS
    common
    LIBRARY
      DESTINATION
        ${LIBS_DIR}
    )
endif()

# pch
if(USE_COREPCH)
	target_precompile_headers(common
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_common.h
		)
endif()
