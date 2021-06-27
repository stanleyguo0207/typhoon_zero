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

# executable test
add_executable(test
  ${PRIVATE_SOURCES}
  )

# include
target_include_directories(test
  PRIVATE
    ${CMAKE_CURRENT_BINARY_DIR}
  PUBLIC
    ${PUBLIC_INCLUDES}
  )

# link
target_link_libraries(test
  PRIVATE
    typhoon-core-interface
  PUBLIC
    common
    proto
    net
  )

# ide folder
set_target_properties(test
  PROPERTIES
    FOLDER
      "server"
  )

# config
set_property(TARGET
  test	
	APPEND
	PROPERTY
		COMPILE_DEFINITIONS
		_TPN_SVR_TEST_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/test_config.json"
	)

# config win32
if(WIN32)
  add_custom_command(TARGET
		test
    POST_BUILD
      COMMAND
			${CMAKE_COMMAND} -E copy
			${CMAKE_CURRENT_SOURCE_DIR}/test_config.json
			${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
  )
endif()

# install
install(TARGETS
  test
    DESTINATION
      ${BIN_DIR}
  )
install(FILES
  test_config.json 
    DESTINATION
    ${CONF_DIR}
  )

# pch
if(USE_COREPCH)
  target_precompile_headers(test
    PRIVATE
      ${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_test.h
    )
endif()
