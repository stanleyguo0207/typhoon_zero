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

if(NOT EXISTS "@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt")
  MESSAGE(FATAL_ERROR "
		Cannot find install manifest: \"@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt\"
		")
endif()

file(READ "@CMAKE_CURRENT_BINARY_DIR@/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")
foreach(file ${files})
	message(STATUS "Uninstalling \"${file}\"")
	if(EXISTS "${file}")
		exec_program("@CMAKE_COMMAND@"
			ARGS "-E remove \"${file}\""
      OUTPUT_VARIABLE rm_out
      RETURN_VALUE rm_retval
			)
		if(${rm_retval} STREQUAL 0)
		else()
			message(FATAL_ERROR "Problem when removing \"${file}\"")
		endif()
	else()
		message(STATUS "File \"${file}\" does not exist.")
	endif()
endforeach()
