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

# library proto
add_library(proto
	${PRIVATE_SOURCES}
	)

# include
target_include_directories(proto
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
	)

# link
target_link_libraries(proto
  PRIVATE
    typhoon-core-interface
	PUBLIC
    common
    protobuf
	)

# export
add_definitions(-DTPN_API_EXPORT_PROTO)

# ide folder
set_target_properties(proto
	PROPERTIES
		FOLDER
			"libs"
	)

# shared library
if(BUILD_SHARED_LIBS)
	install(TARGETS
		proto
		LIBRARY
			DESTINATION
				${BIN_DIR}/lib
		)
endif()

# pch
if(USE_COREPCH)
	target_precompile_headers(proto
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_proto.h
		)
endif()
