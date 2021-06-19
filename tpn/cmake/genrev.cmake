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

if(NOT BUILDDIR)
	# Workaround for funny MSVC behaviour
	# this segment is only used when using cmake gui
	set(BUILDDIR ${CMAKE_BINARY_DIR})
endif()

# Revision generate
if(WITHOUT_GIT)
	set(rev_date "1970-01-01 00:00:00 +0000")
	set(rev_hash "unknown")
	set(rev_branch "Archived")
else()
	if(GIT_EXECUTABLE)
		# Create a revision-string
		execute_process(COMMAND
			"${GIT_EXECUTABLE}" describe --long --match init --dirty=+ --abbrev=12
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			OUTPUT_VARIABLE rev_info
			OUTPUT_STRIP_TRAILING_WHITESPACE
			ERROR_QUIET
			)

		# And grab the commits timestamp
		execute_process(COMMAND
			"${GIT_EXECUTABLE}" show -s --format=%ci
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			OUTPUT_VARIABLE rev_date
			OUTPUT_STRIP_TRAILING_WHITESPACE
			ERROR_QUIET
			)

		# Also retrieve branch name
		execute_process(COMMAND
			"${GIT_EXECUTABLE}" rev-parse --abbrev-ref HEAD
			WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}"
			OUTPUT_VARIABLE rev_branch
			OUTPUT_STRIP_TRAILING_WHITESPACE
			ERROR_QUIET
			)
	endif()

	# Last minute check - ensure that we have a proper revision
	# If everything above fails (means the user has erased the git revision control
	# directory or removed the origin/HEAD tag)
	if(NOT rev_info)
		# No valid ways available to find/set the revision/hash, so let's force some defaults
		message(STATUS "genrev.cmake\n"
			"   Could not find a proper repository signature (hash)"
			"- you may need to pull tags with git fetch -t\n"
			"   Continuing anyway"
			"- note that the version string will be set to \"unknown 1970-01-01 00:00:00 (Archived)\""
			"\n")
		if(NOT rev_date)
			set(rev_date "1970-01-01 00:00:00 +0000")
		endif()
		set(rev_hash "unknown")
		if(NOT rev_branch)
			set(rev_branch "Archived")
		endif()
	else()
		# Extract information required to build a proper versionstring
		string(REGEX REPLACE init-|[0-9]+-g "" rev_hash ${rev_info})
	endif()
endif()

# Create the actual revision_data.h file from the above params
if(NOT "${rev_hash_cached}" MATCHES "${rev_hash}" OR
	 NOT "${rev_branch_cached}" MATCHES "${rev_branch}" OR
	 NOT EXISTS "${BUILDDIR}/revision_data.h")
	configure_file(
		"${CMAKE_SOURCE_DIR}/tpn/cmake/revision/revision_data.h.in.cmake"
		"${BUILDDIR}/revision_data.h"
		@ONLY
		)
	set(rev_hash_cached "${rev_hash}" CACHE INTERNAL "Cached commit-hash")
	set(rev_branch_cached "${rev_branch}" CACHE INTERNAL "Cached branch name")
endif()
