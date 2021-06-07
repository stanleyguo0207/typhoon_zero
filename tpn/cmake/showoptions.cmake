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

message(STATUS "All options info.")
message(STATUS "--------------------------------------------------------------------------")
message(STATUS "typhoon revision                                   : ${rev_hash} ${rev_date} (${rev_branch} branch)")
message(STATUS "CMAKE_BUILD_TYPE                                   : ${CMAKE_BUILD_TYPE}")

# c standard
message(STATUS "CMAKE_C_STANDARD                                   : ${CMAKE_C_STANDARD}")
message(STATUS "CMAKE_C_STANDARD_REQUIRED                          : ${CMAKE_C_STANDARD_REQUIRED}")
message(STATUS "CMAKE_C_EXTENSIONS                                 : ${CMAKE_C_EXTENSIONS}")
message(STATUS "CFLAGS                                             : $ENV{CFLAGS}")
message(STATUS "CMAKE_C_FLAGS                                      : ${CMAKE_C_FLAGS}")

# c++ standard
message(STATUS "CMAKE_CXX_STANDARD                                 : ${CMAKE_CXX_STANDARD}")
message(STATUS "CMAKE_CXX_STANDARD_REQUIRED                        : ${CMAKE_CXX_STANDARD_REQUIRED}")
message(STATUS "CMAKE_CXX_EXTENSIONS                               : ${CMAKE_CXX_EXTENSIONS}")
message(STATUS "CXXFLAGS                                           : $ENV{CXXFLAGS}")
message(STATUS "CMAKE_CXX_FLAGS                                    : ${CMAKE_CXX_FLAGS}")

# configure dir
message(STATUS "CMAKE_SOURCE_DIR                                   : ${CMAKE_SOURCE_DIR}")
message(STATUS "CMAKE_BINARY_DIR                                   : ${CMAKE_BINARY_DIR}")
message(STATUS "CMAKE_INSTALL_PREFIX                               : ${CMAKE_INSTALL_PREFIX}")
message(STATUS "CMAKE_INSTALL_RPATH                                : ${CMAKE_INSTALL_RPATH}")
message(STATUS "CONF_DIR                                           : ${CONF_DIR}")
message(STATUS "LIBS_DIR                                           : ${LIBS_DIR}")

# rpath
if(CMAKE_SKIP_BUILD_RPATH)
  message(STATUS "CMAKE_SKIP_BUILD_RPATH                             : ON")
else()
  message(STATUS "CMAKE_SKIP_BUILD_RPATH                             : OFF")
endif()
if(CMAKE_BUILD_WITH_INSTALL_RPATH)
  message(STATUS "CMAKE_BUILD_WITH_INSTALL_RPATH                     : ON")
else()
  message(STATUS "CMAKE_BUILD_WITH_INSTALL_RPATH                     : OFF")
endif()
if(CMAKE_INSTALL_RPATH_USE_LINK_PATH)
  message(STATUS "CMAKE_INSTALL_RPATH_USE_LINK_PATH                  : ON")
else()
  message(STATUS "CMAKE_INSTALL_RPATH_USE_LINK_PATH                  : OFF")
endif()

# compile_commands.json
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS                      : ON  (default)")
else()
  message(STATUS "CMAKE_EXPORT_COMPILE_COMMANDS                      : OFF")
endif()

# servers
if(SERVERS)
  message(STATUS "Build servers                                      : ON  (default)")
else()
  message(STATUS "Build servers                                      : OFF")
endif()

# tools
if(TOOLS)
  message(STATUS "Build tools                                        : ON")
else()
  message(STATUS "Build tools                                        : OFF (default)")
endif()

# precompiled headers
if(USE_COREPCH)
  message(STATUS "Use precompiled headers when compiling servers     : ON  (default)")
else()
  message(STATUS "Use precompiled headers when compiling servers     : OFF")
endif()
if(USE_SCRIPTPCH)
  message(STATUS "Use precompiled headers when compiling scripts     : ON  (default)")
else()
  message(STATUS "Use precompiled headers when compiling scripts     : OFF")
endif()

# exception
if(USE_EXCEPTION)
  message(STATUS "Use exception not abort                            : ON  (default)")
else()
  message(STATUS "Use exception not abort                            : OFF")
endif()

# endian
if(PLATFORM_USE_BIG_ENDIAN)
  message(STATUS "Platform use big endian                            : ON")
else()
	message(STATUS "Platform use big endian                            : OFF")
endif()

# dynamic links
if(BUILD_SHARED_LIBS)
  message(STATUS "Will link against shared libraries!                : ON")
  if(WITH_DYNAMIC_LINKING_FORCED)
    message(STATUS "Dynamic linking was enforced through a dynamic script module!")
  endif()
  WarnAboutSpacesInBuildPath()
else()
  message(STATUS "Will link against shared libraries!                : OFF (default)")
endif()

# test
if(BUILD_TESTS)
  message(STATUS "Build tests                                        : ON")
else()
  message(STATUS "Build tests                                        : OFF (default)")
endif()

# warnings
if(WITH_WARNINGS)
  message(STATUS "Show all warnings during compile                   : ON")
else()
  message(STATUS "Show all warnings during compile                   : OFF (default)")
endif()

# debug
if(WITH_COREDEBUG)
  message(STATUS "Include additional debug-code in core              : ON")
else()
  message(STATUS "Include additional debug-code in core              : OFF (default)")
endif()

# openssl
if(WITH_SSL)
  message(STATUS "Use ssl in network                                 : ON")
else()
  message(STATUS "Use ssl in network                                 : OFF (default)")
endif()

# source tree
if(NOT ${WITH_SOURCE_TREE} STREQUAL "")
  message(STATUS "Build the source tree for IDE's.                   : ${WITH_SOURCE_TREE}")
else()
  message(STATUS "Build the source tree for IDE's.                   : OFF (default)")
endif()

message(STATUS "--------------------------------------------------------------------------")
