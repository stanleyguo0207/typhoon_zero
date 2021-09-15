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

# executable cell
add_executable(cell
	${PRIVATE_SOURCES}
	)

# include
target_include_directories(cell
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
	)

# link
target_link_libraries(cell
  PRIVATE
    typhoon-core-interface
	PUBLIC
    common
	)
  
# ide folder
set_target_properties(cell
	PROPERTIES
		FOLDER
			"server"
	)

# install
install(TARGETS
  cell
    DESTINATION
      ${BIN_DIR}
  )

# config
if(NOT WIN32)
  set_property(TARGET
	  cell
	  APPEND
	  PROPERTY
		  COMPILE_DEFINITIONS
      _TPN_SVR_CELL_CONFIG="${CONF_DIR}/cell_config.json"
	  )

  install(FILES
    cell_config.json 
      DESTINATION
        ${CONF_DIR}
    )
elseif(WIN32)
  add_custom_command(TARGET
    cell
    POST_BUILD
      COMMAND
      ${CMAKE_COMMAND} -E copy
		  ${CMAKE_CURRENT_SOURCE_DIR}/cell_config.json
		  ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
    )

  install(FILES
    cell_config.json 
      DESTINATION
        ${BIN_DIR}
    )
endif()

# pch
if(USE_COREPCH)
	target_precompile_headers(cell
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_cell.h
		)
endif()
