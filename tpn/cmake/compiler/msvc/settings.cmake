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

# set up output paths for executable binaries (.exe-files, and .dll-files on DLL-capable platforms)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

set(MSVC_EXPECTED_VERSION 19.29)
set(MSVC_EXPECTED_VERSION_STRING "Microsoft Visual Studio 2019 16.10")

if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS MSVC_EXPECTED_VERSION)
  message(FATAL_ERROR "
		MSVC: typhoon requires version ${MSVC_EXPECTED_VERSION} (${MSVC_EXPECTED_VERSION_STRING})
		to build but found ${CMAKE_CXX_COMPILER_VERSION}
		")
endif()

# utf-8 without bom
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4819")

# CMake sets warning flags by default, however we manage it manually
# for different core and dependency targets
# Search twice, once for space after /W argument,
# once for end of line as CMake regex has no \b
string(REGEX REPLACE "/W[0-4] " "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/W[0-4]$" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
string(REGEX REPLACE "/W[0-4] " "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
string(REGEX REPLACE "/W[0-4]$" "" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")

target_compile_options(typhoon-warning-interface
  INTERFACE
    /W3
	)

if(PLATFORM EQUAL 64)
  target_compile_definitions(typhoon-compile-option-interface
    INTERFACE
      -D_WIN64
  	)
  message(STATUS "MSVC: 64-bit platform, enforced -D_WIN64 parameter")
else()
	# mark 32 bit executables large address aware so they can use > 2GB address space
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
  message(STATUS "MSVC: Enabled large address awareness")

  target_compile_options(typhoon-compile-option-interface
    INTERFACE
      /arch:SSE2
  	)
  message(STATUS "MSVC: Enabled SSE2 support")

  set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} /SAFESEH:NO")
  message(STATUS "MSVC: Disabled Safe Exception Handlers for debug builds")
endif()

# Set build-directive (used in core to tell which buildtype we used)
# msbuild/devenv don't set CMAKE_MAKE_PROGRAM, you can choose build type from a 
# dropdown after generating projects
if("${CMAKE_MAKE_PROGRAM}" MATCHES "MSBuild")
  target_compile_definitions(typhoon-compile-option-interface
    INTERFACE
      -D_BUILD_DIRECTIVE="$(ConfigurationName)"
  	)
else()
	# while all make-like generators do (nmake, ninja)
	target_compile_definitions(typhoon-compile-option-interface
		INTERFACE
			-D_BUILD_DIRECTIVE="$<CONFIG>"
		)
endif()

# multithreaded compiling on VS
target_compile_options(typhoon-compile-option-interface
  INTERFACE
    /MP
	)

if((PLATFORM EQUAL 64) OR
	 (NOT CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0) OR
	 BUILD_SHARED_LIBS)
	target_compile_options(typhoon-compile-option-interface
		INTERFACE
			/bigobj
		)
	message(STATUS "MSVC: Enabled increased number of sections in object files")
endif()

# /Zc:throwingNew.
# When you specify Zc:throwingNew on the command line, it instructs the compiler
# to assume that the program will eventually be linked with a conforming operator
# new implementation, and can omit all of these extra null checks from your program.
# http://blogs.msdn.com/b/vcblog/archive/2015/08/06/new-in-vs-2015-zc-throwingnew.aspx
if(NOT (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 19.0.23026.0))
	# makes this flag a requirement to build TC at all
  target_compile_options(typhoon-compile-option-interface
    INTERFACE
      /Zc:throwingNew
      /Zc:preprocessor
  )
endif()

# Define _CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES - eliminates the warning by
# changing the strcpy call to strcpy_s, which prevents buffer overruns
target_compile_definitions(typhoon-compile-option-interface
  INTERFACE
    -D_CRT_SECURE_CPP_OVERLOAD_STANDARD_NAMES
	)
message(STATUS "MSVC: Overloaded standard names")

# Ignore warnings about older, less secure functions
target_compile_definitions(typhoon-compile-option-interface
  INTERFACE
    -D_CRT_SECURE_NO_WARNINGS
	)
message(STATUS "MSVC: Disabled NON-SECURE warnings")

# Ignore warnings about POSIX deprecation
target_compile_definitions(typhoon-compile-option-interface
  INTERFACE
    -D_CRT_NONSTDC_NO_WARNINGS
	)
message(STATUS "MSVC: Disabled POSIX warnings")

# Ignore specific warnings
# C4351: new behavior: elements of array 'x' will be default initialized
# C4091: 'typedef ': ignored on left of '' when no variable is declared
target_compile_options(typhoon-compile-option-interface
  INTERFACE
    /wd4091
    /wd4351
	)

if(WITH_WARNINGS)
  target_compile_options(typhoon-compile-option-interface
    INTERFACE
      /wd4244
      /wd4267
      /wd4355
      /wd4512
      /wd4619
      /wd4985
      /wd4996
		)
  message(STATUS "MSVC: Disabled generic compiletime warnings")
endif()

if (BUILD_SHARED_LIBS)
  # C4251: needs to have dll-interface to be used by clients of class '...'
  # C4275: non dll-interface class ...' used as base for dll-interface class '...'
  target_compile_options(typhoon-compile-option-interface
    INTERFACE
      /wd4251
      /wd4275
  	)
  message(STATUS "MSVC: Enabled shared linking")
endif()

# Enable and treat as errors the following warnings to easily detect virtual function signature failures:
# 'function' : member function does not override any base class virtual member function
# 'virtual_function' : no override available for virtual member function from base 'class'; function is hidden
target_compile_options(typhoon-compile-option-interface
  INTERFACE
    /we4263
    /we4264
  )

# Disable incremental linking in debug builds.
# To prevent linking getting stuck (which might be fixed in a later VS version).
macro(DisableIncrementalLinking variable)
  string(REGEX REPLACE "/INCREMENTAL *" "" ${variable} "${${variable}}")
  set(${variable} "${${variable}} /INCREMENTAL:NO")
endmacro()

DisableIncrementalLinking(CMAKE_EXE_LINKER_FLAGS_DEBUG)
DisableIncrementalLinking(CMAKE_EXE_LINKER_FLAGS_RELWITHDEBINFO)
DisableIncrementalLinking(CMAKE_SHARED_LINKER_FLAGS_DEBUG)
DisableIncrementalLinking(CMAKE_SHARED_LINKER_FLAGS_RELWITHDEBINFO)