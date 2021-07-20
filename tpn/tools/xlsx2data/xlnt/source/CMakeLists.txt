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

# Commonly used project directories
set(XLNT_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(XLNT_INCLUDE_DIR ${XLNT_ROOT_DIR}/include)
set(XLNT_SOURCE_DIR ${XLNT_ROOT_DIR}/source)
set(THIRD_PARTY_DIR ${XLNT_ROOT_DIR}/third-party)

# Include libstudxml library
add_subdirectory(${THIRD_PARTY_DIR}/libstudxml
  ${CMAKE_CURRENT_BINARY_DIR}/third-party/libstudxml)

# Non-target-specific compiler settings
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4") # level 4 warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP") # multi-processor compilation
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall") # all warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra") # extra warnings
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unknown-pragmas") # ignore MSVC and Clang pragmas
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-maybe-uninitialized") # GCC diagnostic with lots of false positives
endif()

file(GLOB ROOT_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/*.hpp)
file(GLOB CELL_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/cell/*.hpp)
file(GLOB CELL_SOURCES ${XLNT_SOURCE_DIR}/cell/*.cpp)
file(GLOB DRAWING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB CHARTS_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/charts/*.hpp)
file(GLOB CHARTS_SOURCES ${XLNT_SOURCE_DIR}/charts/*.cpp)
file(GLOB CHARTSHEET_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/chartsheet/*.hpp)
file(GLOB CHARTSHEET_SOURCES ${XLNT_SOURCE_DIR}/chartsheet/*.cpp)
file(GLOB DRAWING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/drawing/*.hpp)
file(GLOB DRAWING_SOURCES ${XLNT_SOURCE_DIR}/drawing/*.cpp)
file(GLOB FORMULA_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/formula/*.hpp)
file(GLOB FORMULA_SOURCES ${XLNT_SOURCE_DIR}/formula/*.cpp)
file(GLOB PACKAGING_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/packaging/*.hpp)
file(GLOB PACKAGING_SOURCES ${XLNT_SOURCE_DIR}/packaging/*.cpp)
file(GLOB STYLES_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/styles/*.hpp)
file(GLOB STYLES_SOURCES ${XLNT_SOURCE_DIR}/styles/*.cpp)
file(GLOB UTILS_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/utils/*.hpp)
file(GLOB UTILS_SOURCES ${XLNT_SOURCE_DIR}/utils/*.cpp)
file(GLOB WORKBOOK_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/workbook/*.hpp)
file(GLOB WORKBOOK_SOURCES ${XLNT_SOURCE_DIR}/workbook/*.cpp)
file(GLOB WORKSHEET_HEADERS ${XLNT_INCLUDE_DIR}/xlnt/worksheet/*.hpp)
file(GLOB WORKSHEET_SOURCES ${XLNT_SOURCE_DIR}/worksheet/*.cpp)
file(GLOB MINIZ_HEADERS ${THIRD_PARTY_DIR}/miniz/*.h)
file(GLOB MINIZ_SOURCES ${THIRD_PARTY_DIR}/miniz/*.c)

file(GLOB DETAIL_ROOT_HEADERS ${XLNT_SOURCE_DIR}/detail/*.hpp)
file(GLOB DETAIL_ROOT_SOURCES ${XLNT_SOURCE_DIR}/detail/*.cpp)
file(GLOB DETAIL_CRYPTOGRAPHY_HEADERS ${XLNT_SOURCE_DIR}/detail/cryptography/*.hpp)
file(GLOB DETAIL_CRYPTOGRAPHY_SOURCES ${XLNT_SOURCE_DIR}/detail/cryptography/*.c*)
file(GLOB DETAIL_EXTERNAL_HEADERS ${XLNT_SOURCE_DIR}/detail/external/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_HEADERS ${XLNT_SOURCE_DIR}/detail/header_footer/*.hpp)
file(GLOB DETAIL_HEADER_FOOTER_SOURCES ${XLNT_SOURCE_DIR}/detail/header_footer/*.cpp)
file(GLOB DETAIL_IMPLEMENTATIONS_HEADERS ${XLNT_SOURCE_DIR}/detail/implementations/*.hpp)
file(GLOB DETAIL_IMPLEMENTATIONS_SOURCES ${XLNT_SOURCE_DIR}/detail/implementations/*.cpp)
file(GLOB DETAIL_NUMBER_FORMAT_HEADERS ${XLNT_SOURCE_DIR}/detail/number_format/*.hpp)
file(GLOB DETAIL_NUMBER_FORMAT_SOURCES ${XLNT_SOURCE_DIR}/detail/number_format/*.cpp)
file(GLOB DETAIL_SERIALIZATION_HEADERS ${XLNT_SOURCE_DIR}/detail/serialization/*.hpp)
file(GLOB DETAIL_SERIALIZATION_SOURCES ${XLNT_SOURCE_DIR}/detail/serialization/*.cpp)


set(DETAIL_HEADERS ${DETAIL_ROOT_HEADERS} ${DETAIL_CRYPTOGRAPHY_HEADERS}
  ${DETAIL_EXTERNAL_HEADERS} ${DETAIL_HEADER_FOOTER_HEADERS}
  ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_NUMBER_FORMAT_HEADERS}
  ${DETAIL_SERIALIZATION_HEADERS})
set(DETAIL_SOURCES ${DETAIL_ROOT_SOURCES} ${DETAIL_CRYPTOGRAPHY_SOURCES}
  ${DETAIL_EXTERNAL_SOURCES} ${DETAIL_HEADER_FOOTER_SOURCES}
  ${DETAIL_IMPLEMENTATIONS_SOURCES} ${DETAIL_NUMBER_FORMAT_SOURCES}
  ${DETAIL_SERIALIZATION_SOURCES})

set(XLNT_HEADERS ${ROOT_HEADERS} ${CELL_HEADERS} ${CHARTS_HEADERS}
  ${CHARTSHEET_HEADERS} ${DRAWING_HEADERS} ${FORMULA_HEADERS}
  ${PACKAGING_HEADERS} ${STYLES_HEADERS} ${UTILS_HEADERS}
  ${WORKBOOK_HEADERS} ${WORKSHEET_HEADERS} ${DETAIL_HEADERS} ${DETAIL_CRYPTO_HEADERS}
  ${DRAWING_HEADERS} ${MINIZ_HEADERS})
set(XLNT_SOURCES ${CELL_SOURCES} ${CHARTS_SOURCES} ${CHARTSHEET_SOURCES}
  ${DRAWING_SOURCES} ${FORMULA_SOURCES} ${PACKAGING_SOURCES}
  ${STYLES_SOURCES} ${UTILS_SOURCES} ${WORKBOOK_SOURCES}
  ${WORKSHEET_SOURCES} ${DETAIL_SOURCES} ${DETAIL_CRYPTO_SOURCES}
  ${DRAWING_SOURCES} ${MINIZ_SOURCES})

# Append "d" to the name of the compiled library
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "Set debug library postfix")

include(GNUInstallDirs)

# Compile shared library
add_library(xlnt
  SHARED
    ${XLNT_HEADERS}
    ${XLNT_SOURCES}
    $<TARGET_OBJECTS:libstudxml>
  )

target_compile_definitions(xlnt
  PRIVATE
    XLNT_SHARED=1
  )
  
# Includes
target_include_directories(xlnt
	PUBLIC
	  $<BUILD_INTERFACE:${XLNT_INCLUDE_DIR}>
	PRIVATE
	  ${XLNT_SOURCE_DIR}
	  ${XLNT_SOURCE_DIR}/../third-party/libstudxml
    ${XLNT_SOURCE_DIR}/../third-party/miniz
	  # ${XLNT_SOURCE_DIR}/../third-party/utfcpp
  )

# link
target_link_libraries(xlnt
  PRIVATE
    typhoon-core-interface
	PUBLIC
    utfcpp
	)

# Platform- and file-specific settings, MSVC
if(MSVC)
  target_compile_definitions(xlnt
    PRIVATE
      _CRT_SECURE_NO_WARNINGS=1
    )

  target_compile_definitions(xlnt
    PRIVATE
      XLNT_EXPORT=1
    )

  set_target_properties(xlnt
    PROPERTIES
      COMPILE_FLAGS
        "/wd\"4251\" /wd\"4275\" /wd\"4068\" /wd\"4005\""
    )

  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
    PROPERTIES
      COMPILE_FLAGS
        "/wd\"4244\" /wd\"4334\" /wd\"4127\""
    )

  set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/cryptography/aes.cpp
    PROPERTIES
      COMPILE_FLAGS
        "/wd\"4996\""
    )
else()
  # Platform- and file-specific settings, GCC
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/detail/serialization/miniz.cpp
	    PROPERTIES
	      COMPILE_FLAGS
          "-Wno-strict-aliasing"
      )
  endif()
endif()

# Group files into pseudo-folders in IDEs
source_group(xlnt FILES ${ROOT_HEADERS})
source_group(cell FILES ${CELL_HEADERS} ${CELL_SOURCES})
source_group(detail FILES ${DETAIL_ROOT_HEADERS} ${DETAIL_ROOT_SOURCES})
source_group(detail\\cryptography FILES ${DETAIL_CRYPTOGRAPHY_HEADERS} ${DETAIL_CRYPTOGRAPHY_SOURCES})
source_group(detail\\external FILES ${DETAIL_EXTERNAL_HEADERS})
source_group(detail\\header_footer FILES ${DETAIL_HEADER_FOOTER_HEADERS} ${DETAIL_HEADER_FOOTER_SOURCES})
source_group(detail\\implementations FILES ${DETAIL_IMPLEMENTATIONS_HEADERS} ${DETAIL_IMPLEMENTATIONS_SOURCES})
source_group(detail\\number_format FILES ${DETAIL_NUMBER_FORMAT_HEADERS} ${DETAIL_NUMBER_FORMAT_SOURCES})
source_group(detail\\serialization FILES ${DETAIL_SERIALIZATION_HEADERS} ${DETAIL_SERIALIZATION_SOURCES})
source_group(drawing FILES ${DRAWING_HEADERS} ${DRAWING_SOURCES})
source_group(packaging FILES ${PACKAGING_HEADERS} ${PACKAGING_SOURCES})
source_group(styles FILES ${STYLES_HEADERS} ${STYLES_SOURCES})
source_group(utils FILES ${UTILS_HEADERS} ${UTILS_SOURCES})
source_group(workbook FILES ${WORKBOOK_HEADERS} ${WORKBOOK_SOURCES})
source_group(worksheet FILES ${WORKSHEET_HEADERS} ${WORKSHEET_SOURCES})
source_group(third-party\\miniz FILES ${MINIZ_HEADERS} ${MINIZ_SOURCES})

set_target_properties(common
	PROPERTIES
		FOLDER
			"libs"
	)
