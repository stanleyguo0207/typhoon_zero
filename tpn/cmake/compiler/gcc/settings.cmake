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

target_compile_definitions(typhoon-compile-option-interface
	INTERFACE
		-D_BUILD_DIRECTIVE="$<CONFIG>"
	)

set(GCC_EXPECTED_VERSION 10.0.0)
if(CMAKE_CXX_COMPILER_VERSION VERSION_LESS GCC_EXPECTED_VERSION)
	message(FATAL_ERROR "
		GCC: typhoon requires version ${GCC_EXPECTED_VERSION} to build
		but found ${CMAKE_CXX_COMPILER_VERSION}
		")
endif()

if(PLATFORM EQUAL 32)
	target_compile_options(typhoon-compile-option-interface
		INTERFACE
			-DHAVE_SSE2
			-D__SSE2__
		)
endif()
message(STATUS "GCC: SFMT enabled, SSE2 flags forced")

if(WITH_WARNINGS)
	target_compile_options(typhoon-warning-interface
		INTERFACE
			-W
			-Wall
			-Wextra
			-Winit-self
			-Winvalid-pch
			-Wfatal-errors
			-Woverloaded-virtual
		)
	message(STATUS "GCC: All warings enabled")
endif()

if(WITH_COREDEBUG)
	target_compile_options(typhoon-compile-option-interface
		INTERFACE
			-g3
		)
	message(STATUS "GCC: Debug-flags set (-g3)")
endif()

if(BUILD_SHARED_LIBS)
	target_compile_options(typhoon-compile-option-interface
		INTERFACE
			-fPIC
			-Wno-attributes
		)

	target_compile_options(typhoon-hidden-symbols-interface
		INTERFACE
			-fvisibility=hidden
		)

	# Should break the build when there are TYPHOON_*_API macros missing
  # but it complains about missing references in precompiled headers.
  # set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wl,--no-undefined")
  # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--no-undefined")

	message(STATUS "GCC: Enabled shared linking")
endif()
