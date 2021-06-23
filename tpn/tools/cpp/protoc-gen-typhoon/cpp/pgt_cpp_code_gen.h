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

#ifndef TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_CODE_GEN_H_
#define TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_CODE_GEN_H_

#include "pgt_config.h"
#include "pgt_fwd.h"

namespace pb  = google::protobuf;
namespace pbc = pb::compiler;

namespace tpn {

namespace protoc {

/// protobuf gen typhoon 代码生成器
class PGTCppCodeGenerator : public pbc::CodeGenerator {
 public:
  PGTCppCodeGenerator();
  virtual ~PGTCppCodeGenerator();

  // Generates code for the given proto file, generating one or more files in
  // the given output directory.
  //
  // A parameter to be passed to the generator can be specified on the command
  // line. This is intended to be used to pass generator specific parameters.
  // It is empty if no parameter was given. ParseGeneratorParameter (below),
  // can be used to accept multiple parameters within the single parameter
  // command line flag.
  //
  // Returns true if successful.  Otherwise, sets *error to a description of
  // the problem (e.g. "invalid parameter") and returns false.
  bool Generate(const pb::FileDescriptor *file, const std::string &parameter,
                pbc::GeneratorContext *generator_context,
                std::string *error) const override;
};

}  // namespace protoc

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_CODE_GEN_H_
