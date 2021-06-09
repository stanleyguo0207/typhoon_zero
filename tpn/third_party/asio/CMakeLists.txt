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

# libray
add_library(asio
	INTERFACE
	)

# definitions
target_compile_definitions(asio
	INTERFACE
		ASIO_STANDALONE
	)

# include
target_include_directories(asio
	INTERFACE
		${CMAKE_CURRENT_SOURCE_DIR}
	)

# link
target_link_libraries(asio
	INTERFACE
		threads
	)

# ide folder
set_target_properties(asio
	PROPERTIES
		FOLDER
			"third_party"
	)
