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

#include <cstdio>

#include "banner.h"

#include "fmt_wrap.h"
#include "git_revision.h"

namespace tpn {

namespace {

static const char *s_logo_str = R"logo(
<Ctrl-C> to stop.
 ______   __  __     ______   __  __     ______     ______     __   __
/\__  _\ /\ \_\ \   /\  == \ /\ \_\ \   /\  __ \   /\  __ \   /\ "-.\ \
\/_/\ \/ \ \____ \  \ \  _-/ \ \  __ \  \ \ \/\ \  \ \ \/\ \  \ \ \-.  \
   \ \_\  \/\_____\  \ \_\    \ \_\ \_\  \ \_____\  \ \_____\  \ \_\\"\_\
    \/_/   \/_____/   \/_/     \/_/\/_/   \/_____/   \/_____/   \/_/ \/_/
                              Z E R O
                        stanleyguo0207@163.com
                    https://gitee.com/stanleyguo0207
                    https://github.com/stanleyguo0207

)logo";

}  // namespace

void BannerShow(const char *app_name, std::function<void()> extra) {
  printf("%s\n", "{} ({})\n{}"_format(git::GetFullVersion(),
                                      app_name ? app_name : "", s_logo_str)
                     .c_str());

  if (extra) {
    extra();
  }
}

}  // namespace tpn
