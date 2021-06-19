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

if(WIN32)
  # Use source code on windows

  # srouce collect
  set(ZLIB_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/adler32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/compress.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/crc32.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/crc32.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/deflate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/deflate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/gzclose.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/gzguts.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/gzlib.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/gzread.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/gzwrite.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/infback.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inffast.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inffast.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inffixed.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inflate.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inflate.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inftrees.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/inftrees.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/trees.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/trees.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/uncompr.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zconf.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zlib.h
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zutil.c
    ${CMAKE_CURRENT_SOURCE_DIR}/zlib/zutil.h
    )

  # libray
  add_library(zlib
    STATIC
      ${ZLIB_SOURCES}
    )

  # include
  target_include_directories(zlib
    PUBLIC
      ${CMAKE_CURRENT_SOURCE_DIR}/zlib
    )

  # link libray
  target_link_libraries(zlib
    PRIVATE
      typhoon-dependency-interface
  )

  #ide folder
  set_target_properties(zlib
    PROPERTIES
      FOLDER
        "third_party"
    )

else()
  # Look for an installed zlib on unix
  find_package(ZLIB REQUIRED)

  add_library(zlib
    SHARED
      IMPORTED GLOBAL
    )

  set_target_properties(zlib
    PROPERTIES
      IMPORTED_LOCATION
        "${ZLIB_LIBRARIES}"
      INTERFACE_INCLUDE_DIRECTORIES
        "${ZLIB_INCLUDE_DIRS}"
    )
endif()

