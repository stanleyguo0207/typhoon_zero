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

#include "pgt_cpp_code_gen.h"

namespace pb    = google::protobuf;
namespace pbc   = pb::compiler;
namespace pbcpp = pbc::cpp;

namespace tpn {

namespace protoc {

PGTCppCodeGenerator::PGTCppCodeGenerator() {}
PGTCppCodeGenerator::~PGTCppCodeGenerator() {}

bool PGTCppCodeGenerator::Generate(const pb::FileDescriptor *file,
                                   const std::string &parameter,
                                   pbc::GeneratorContext *generator_context,
                                   std::string *error) const {
  std::vector<std::pair<std::string, std::string> > options;
  pbc::ParseGeneratorParameter(parameter, &options);

  return true;
}

}  // namespace protoc

}  // namespace tpn
