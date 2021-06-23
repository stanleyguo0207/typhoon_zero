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

#ifndef TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_SERVICE_GEN_H_
#define TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_SERVICE_GEN_H_

#include <map>

#include <google/protobuf/stubs/common.h>

#include "pgt_fwd.h"

namespace pb    = google::protobuf;
namespace pbcpp = pb::compiler::cpp;

namespace tpn {

namespace protoc {

class PGTCppServiceGenerator {
 public:
  PGTCppServiceGenerator(const pb::ServiceDescriptor *service,
                         const pbcpp::Options &options);
  ~PGTCppServiceGenerator();

 private:
  const pb::ServiceDescriptor *service_;  ///< 服务
  std::map<std::string, std::string> vars_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(PGTCppServiceGenerator);
};

}  // namespace protoc

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_SERVICE_GEN_H_
