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

set(LIBSTUDXML_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(LIBSTUDXML_INCLUDE_DIR ${LIBSTUDXML_ROOT_DIR})

set(LIBSTUDXML
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/parser.cxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/qname.cxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/serializer.cxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/value-traits.cxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/content.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/exception.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/forward.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/parser.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/qname.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/serializer.hxx
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/value-traits.hxx
  )

set(GENX
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/genx/char-props.c
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/genx/genx.c
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/genx/genx.h
  )

set(EXPAT
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmlparse.c
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmlrole.c
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmltok.c
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/ascii.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/asciitab.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/config.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/expat_external.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/expat.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/iasciitab.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/internal.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/latin1tab.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/nametab.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/utf8tab.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmlrole.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmltok_impl.h
  ${LIBSTUDXML_ROOT_DIR}/libstudxml/details/expat/xmltok.h
  )

add_library(libstudxml
  OBJECT
    ${LIBSTUDXML}
    ${GENX}
    ${EXPAT}
  )

target_compile_definitions(libstudxml
  PUBLIC
    LIBSTUDXML_STATIC_LIB=1
  )

target_include_directories(libstudxml
  PUBLIC
    ${LIBSTUDXML_ROOT_DIR}
  PUBLIC
    ${EXPAT_INCLUDE_DIRS}
  )

if(MSVC)
  target_compile_definitions(libstudxml
    PRIVATE
      _CRT_SECURE_NO_WARNINGS=1
    )
endif()

set_target_properties(libstudxml
  PROPERTIES
    POSITION_INDEPENDENT_CODE 1
  )
