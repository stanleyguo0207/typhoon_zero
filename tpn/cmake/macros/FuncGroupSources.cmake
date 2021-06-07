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

# no
# flat
# hierarchical (default)
# hierarchical-folders
macro(GroupSources dir)
	# WITH_SOURCE_TREE 
  if(NOT ${WITH_SOURCE_TREE} STREQUAL "")
    # include all header files and c/c++ source files
    file(GLOB_RECURSE elements RELATIVE ${dir} *.h *.hpp *.c *.cpp *.cc)

    foreach(element ${elements})
			# extract filename and directories
      get_filename_component(element_name ${element} NAME)
      get_filename_component(element_dir ${element} DIRECTORY)

      if(NOT ${element_dir} STREQUAL "")
        if(${WITH_SOURCE_TREE} STREQUAL "flat")
          string(FIND ${element_dir} "/" delemiter_pos)
          if(NOT ${delemiter_pos} EQUAL -1)
            string(SUBSTRING ${element_dir} 0 ${delemiter_pos} group_name)
            source_group("${group_name}" FILES ${dir}/${element})
          else()
            source_group("${element_dir}" FILES ${dir}/${element})
          endif()
        else()
          string(REPLACE "/" "\\" group_name ${element_dir})
          source_group("${group_name}" FILES ${dir}/${element})
        endif()
      else()
        source_group("\\" FILES ${dir}/${element})
      endif()
    endforeach()
  endif()
endmacro()

# https://cmake.org/cmake/help/v3.20/prop_gbl/USE_FOLDERS.html
if(WITH_SOURCE_TREE STREQUAL "hierarchical-folders")
	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
endif()
