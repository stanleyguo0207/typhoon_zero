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

#include "net_error.h"

namespace tpn {

namespace net {

namespace {

thread_local static std::error_code s_ec_last;

}  // namespace

void SetLastError(int ec) { s_ec_last.assign(ec, std::system_category()); }

void SetLastError(int ec, const std::error_category &ecat) {
  s_ec_last.assign(ec, ecat);
}

void SetLastError(const std::error_code &ec) { s_ec_last = ec; }

void SetLastError(const std::system_error &e) { s_ec_last = e.code(); }

void ClearLastError() { s_ec_last.clear(); }

std::error_code &GetLastError() { return s_ec_last; }

int GetLastErrorVal() { return s_ec_last.value(); }

std::string GetLastErrorMsg() { return s_ec_last.message(); }

}  // namespace net

}  // namespace tpn
