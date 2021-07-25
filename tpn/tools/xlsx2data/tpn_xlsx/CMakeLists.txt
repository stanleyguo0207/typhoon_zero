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

# executable tpn_xlsx
add_executable(tpn_xlsx
	${PRIVATE_SOURCES}
	)

# include
target_include_directories(tpn_xlsx
	PRIVATE
		${CMAKE_CURRENT_BINARY_DIR}
	PUBLIC
		${PUBLIC_INCLUDES}
	)

# link
target_link_libraries(tpn_xlsx
  PRIVATE
    typhoon-core-interface
	PUBLIC
    common
    xlnt
	)
  
# ide folder
set_target_properties(tpn_xlsx
	PROPERTIES
		FOLDER
			"tools"
	)

# install
install(TARGETS
  tpn_xlsx
    DESTINATION
      ${BIN_DIR}/tools
  )

# config
if(NOT WIN32)
  set_property(TARGET
	  tpn_xlsx
	  APPEND
	  PROPERTY
		  COMPILE_DEFINITIONS
		  _TPN_XLSX2DATA_CONFIG="${CONF_DIR}/xlsx2data_config.json"
	  )

  install(FILES
    xlsx2data_config.json 
      DESTINATION
      ${CONF_DIR}
    )
elseif(WIN32)
  add_custom_command(TARGET
    tpn_xlsx
    POST_BUILD
      COMMAND
      ${CMAKE_COMMAND} -E copy
		  ${CMAKE_CURRENT_SOURCE_DIR}/xlsx2data_config.json
		  ${CMAKE_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE}/
    )

  install(FILES
    xlsx2data_config.json 
      DESTINATION
      ${BIN_DIR}/tools
    )
endif()

# pch
if(USE_COREPCH)
	target_precompile_headers(tpn_xlsx
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/pch/pch_tpn_xlsx.h
		)
endif()
