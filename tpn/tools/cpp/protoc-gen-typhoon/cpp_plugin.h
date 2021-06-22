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

#ifndef TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PLUGIN_H_
#define TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PLUGIN_H_

#include <string>

#include <google/protobuf/stubs/common.h>

namespace google {

namespace protobuf {

class FileDescriptor;

namespace compiler {

class GeneratorContext;

}  // namespace compiler

}  // namespace protobuf

}  // namespace google

namespace pb  = google::protobuf;
namespace pbc = pb::compiler;

namespace tpn {

namespace protoc {

class CppRpcGenerator : public pbc::CodeGenerator {
 public:
  CppRpcGenerator()          = default;
  virtual ~CppRpcGenerator() = default;

  bool Generate(const pb::FileDescriptor *file, const std::string &parameter,
                pbc::GeneratorContext *generator_context,
                std::string *error) const override {
    return true;
  }
};

}  // namespace protoc

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PLUGIN_H_
