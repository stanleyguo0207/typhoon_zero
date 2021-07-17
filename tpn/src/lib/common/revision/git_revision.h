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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_COMMON_REVISION_GIT_REVISION_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_COMMON_REVISION_GIT_REVISION_H_

#include "define.h"

namespace tpn {

namespace git {

TPN_COMMON_API const char *GetHash();
TPN_COMMON_API const char *GetDate();
TPN_COMMON_API const char *GetBranch();
TPN_COMMON_API const char *GetCMakeCommand();
TPN_COMMON_API const char *GetCMakeVersion();
TPN_COMMON_API const char *GetHostOSVersion();
TPN_COMMON_API const char *GetBuildDirectory();
TPN_COMMON_API const char *GetSourceDirectory();
TPN_COMMON_API const char *GetMySQLExecutable();
TPN_COMMON_API const char *GetFullDatabase();
TPN_COMMON_API const char *GetHotfixesDatabase();
TPN_COMMON_API const char *GetFullVersion();
TPN_COMMON_API const char *GetCompanyNameStr();
TPN_COMMON_API const char *GetLegalCopyrightStr();
TPN_COMMON_API const char *GetFileVersionStr();
TPN_COMMON_API const char *GetProductVersionStr();

}  // namespace git

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_COMMON_REVISION_GIT_REVISION_H_
