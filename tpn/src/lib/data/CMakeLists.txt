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
  ${CMAKE_CURRENT_SOURCE_DIR}/raw
	)

# collect include
CollectIncludeDirectories(
	${CMAKE_CURRENT_SOURCE_DIR}
	PUBLIC_INCLUDES
	# Exclude
	${CMAKE_CURRENT_SOURCE_DIR}/pch
  ${CMAKE_CURRENT_SOURCE_DIR}/raw
	)

# source tree
GroupSources(${CMAKE_CURRENT_SOURCE_DIR})

# library data
add_library(data
	${PRIVATE_SOURCES}
	)

# include
target_include_directories(data
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
    ${CMAKE_BINARY_DIR}
	)

# link
target_link_libraries(data
  PRIVATE
    typhoon-core-interface
	PUBLIC
    common
    protobuf
	)

# export
add_definitions(-DTPN_API_EXPORT_DATA)

# ide folder
set_target_properties(data
	PROPERTIES
		FOLDER
			"libs"
	)

# data
set_property(TARGET
  data	
	APPEND
	PROPERTY
		COMPILE_DEFINITIONS
		_TPN_DATA_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}/data/data_hub.bin"
	)

# data win32
if(WIN32)
  add_custom_command(TARGET
		data
    POST_BUILD
      COMMAND
      ${CMAKE_COMMAND} -E copy_directory 
		  ${CMAKE_CURRENT_SOURCE_DIR}/data
		  ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/data
  )
endif()

install(FILES
  data/data_hub.bin
    DESTINATION
    ${CONF_DIR}
  )

# shared library
if(BUILD_SHARED_LIBS)
  install(TARGETS
    data
    LIBRARY
      DESTINATION
        ${LIBS_DIR}
    )
endif()

# pch
if(USE_COREPCH)
	target_precompile_headers(data
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_data.h
		)
endif()
