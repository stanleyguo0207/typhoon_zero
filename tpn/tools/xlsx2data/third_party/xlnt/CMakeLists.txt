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
	${CMAKE_CURRENT_SOURCE_DIR}/python
	)

# collect include
CollectIncludeDirectories(
	${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC_INCLUDES
	# Exclude
	${CMAKE_CURRENT_SOURCE_DIR}/python
	)

# source tree
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

# library xlnt
add_library(xlnt
  ${PRIVATE_SOURCES}
	)

# include
target_include_directories(xlnt
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
	)

# link
target_link_libraries(xlnt
  PRIVATE
    typhoon-core-interface
	PUBLIC
    libstudxml
    miniz
    utfcpp_xlnt
	)
  
# export
add_definitions(-DXLNT_STATIC)
# add_definitions(-DXLNT_EXPORT)

# ide folder
set_target_properties(xlnt
	PROPERTIES
		FOLDER
			"tools"
	)

# shared library
if(BUILD_SHARED_LIBS)
  install(TARGETS
    xlnt
    LIBRARY
      DESTINATION
        ${LIBS_DIR}
    )
endif()
