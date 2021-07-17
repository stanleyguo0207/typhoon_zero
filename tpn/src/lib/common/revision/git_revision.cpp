//
//           ┌┬┐┬ ┬┌─┐┬ ┬┌─┐┌─┐┌┐┌
//            │ └┬┘├─┘├─┤│ ││ ││││
//            ┴  ┴ ┴  ┴ ┴└─┘└─┘┘└┘
//
// This file is part of the typhoon Project.
// Copyright (C) 2021 stanley0207@163.com
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "git_revision.h"

#include "revision_data.h"
#include "fmt_wrap.h"

namespace tpn {

namespace git {

const char *GetHash() { return _HASH; }

const char *GetDate() { return _DATE; }

const char *GetBranch() { return _BRANCH; }

const char *GetCMakeCommand() { return _CMAKE_COMMAND; }

const char *GetCMakeVersion() { return _CMAKE_VERSION; }

const char *GetHostOSVersion() { return _CMAKE_HOST_SYSTEM; }

const char *GetBuildDirectory() { return _BUILD_DIRECTORY; }

const char *GetSourceDirectory() { return _SOURCE_DIRECTORY; }

const char *GetMySQLExecutable() { return _MYSQL_EXECUTABLE; }

const char *GetFullDatabase() { return _FULL_DATABASE; }

const char *GetHotfixesDatabase() { return _HOTFIXES_DATABASE; }

#if (TPN_PLATFORM == TPN_PLATFORM_WIN)
#  if defined(_WIN64)
#    define TPN_PLATFORM_STR "Win64"
#  else
#    define TPN_PLATFORM_STR "Win32"
#  endif
#else
#  define TPN_PLATFORM_STR "Unix"
#endif

#if defined(TPN_API_USE_DYNAMIC_LINKING)
#  define TPN_LINKING_TYPE_STR "Dynamic"
#else
#  define TPN_LINKING_TYPE_STR "Static"
#endif

const char *GetFullVersion() {
  return "Typhoon rev. " VER_PRODUCTVERSION_STR "\n(" TPN_PLATFORM_STR
         ", " _BUILD_DIRECTIVE ", " TPN_LINKING_TYPE_STR ")";
}

const char *GetCompanyNameStr() { return VER_COMPANYNAME_STR; }

const char *GetLegalCopyrightStr() { return VER_LEGALCOPYRIGHT_STR; }

const char *GetFileVersionStr() { return VER_FILEVERSION_STR; }

const char *GetProductVersionStr() { return VER_PRODUCTVERSION_STR; }

}  // namespace git

}  // namespace tpn
