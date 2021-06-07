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

# An interface library to make the target com available to other targets
add_library(typhoon-compile-option-interface INTERFACE)

# Use -std=c++11 instead of -std=gnu++11
set(CXX_EXTENSIONS OFF)

# An interface library to make the target features available to other targets
add_library(typhoon-feature-interface INTERFACE)

target_compile_features(typhoon-feature-interface
	INTERFACE
		cxx_std_17
		cxx_alias_templates
		cxx_auto_type
		cxx_constexpr
		cxx_decltype
		cxx_decltype_auto
		cxx_final
		cxx_lambdas
		cxx_generic_lambdas
		cxx_variadic_templates
		cxx_defaulted_functions
		cxx_nullptr
		cxx_trailing_return_types
		cxx_return_type_deduction
		cxx_override
		cxx_alignof
		cxx_alignas
		cxx_attribute_deprecated
	)

# An interface used for all other interfaces
add_library(typhoon-default-interface INTERFACE)
target_link_libraries(typhoon-default-interface
	INTERFACE
		typhoon-compile-option-interface
		typhoon-feature-interface
	)

# An interface library to make the warnings level available to other targets
# This interface taget is set-up through the platform specific script
add_library(typhoon-warning-interface INTERFACE)

# An interface used for silencing all warnings
add_library(typhoon-no-warning-interface INTERFACE)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  target_compile_options(typhoon-no-warning-interface
    INTERFACE
      /W0
		)
else()
  target_compile_options(typhoon-no-warning-interface
    INTERFACE
      -w
		)
endif()

# An interface library to change the default behaviour
# to hide symbols automatically.
add_library(typhoon-hidden-symbols-interface INTERFACE)

# An interface amalgamation which provides the flags and definitions
# used by the dependency targets.
add_library(typhoon-dependency-interface INTERFACE)
target_link_libraries(typhoon-dependency-interface
  INTERFACE
    typhoon-default-interface
    typhoon-no-warning-interface
    typhoon-hidden-symbols-interface
	)

# An interface amalgamation which provides the flags and definitions
# used by the core targets.
add_library(typhoon-core-interface INTERFACE)
target_link_libraries(typhoon-core-interface
  INTERFACE
    typhoon-default-interface
    typhoon-warning-interface
	)
