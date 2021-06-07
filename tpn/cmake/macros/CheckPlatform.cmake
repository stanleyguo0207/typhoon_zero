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

# check platform bit size
if(CMAKE_SIZEOF_VOID_P MATCHES 8)
	message(STATUS "Detected that the current platform is 64-bit")
	set(PLATFORM 64)
else()
	message(STATUS "Detected that the current platform is 32-bit")
	set(PLATFORM 32)
endif()

if(WIN32)
	message(STATUS "Start loading Windows platform configuration")
	include("${CMAKE_SOURCE_DIR}/tpn/cmake/platform/win/settings.cmake")
elseif(UNIX)
	message(STATUS "Start loading Unix platform configuration")
	include("${CMAKE_SOURCE_DIR}/tpn/cmake/platform/unix/settings.cmake")
endif()
