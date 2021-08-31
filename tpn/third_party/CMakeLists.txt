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

add_subdirectory(threads)

if(SERVERS OR TOOLS)
	add_subdirectory(fmt)
	add_subdirectory(sfmt)
	add_subdirectory(utfcpp)
	add_subdirectory(asio)
	add_subdirectory(zlib)
	add_subdirectory(valgrind)
  add_subdirectory(g3dlite)
endif()

if(SERVERS)
	add_subdirectory(rapidjson)
	add_subdirectory(protobuf)
	add_subdirectory(readline)
endif()

if(TOOLS)
endif()
