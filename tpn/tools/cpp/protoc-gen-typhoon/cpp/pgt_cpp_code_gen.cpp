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

#include <memory>
#include <utility>
#include <vector>

#include <google/protobuf/stubs/strutil.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/descriptor.pb.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/io/zero_copy_stream.h>

#include "pgt_cpp_file_gen.h"

namespace pb    = google::protobuf;
namespace pbc   = pb::compiler;
namespace pbcpp = pbc::cpp;

namespace tpn {

namespace protoc {

bool PGTCppCodeGenerator::Generate(const pb::FileDescriptor *file,
                                   const std::string &parameter,
                                   pbc::GeneratorContext *generator_context,
                                   std::string *error) const {
  std::vector<std::pair<std::string, std::string> > options;
  pbc::ParseGeneratorParameter(parameter, &options);

  // 选项处理
  pbcpp::Options file_options;
  for (auto &&[key, val] : options) {
    if ("dllexport_decl" == key) {
      file_options.dllexport_decl = val;
    } else if ("safe_boundary_check" == key) {
      file_options.safe_boundary_check = true;
    } else if ("speed" == key) {
      file_options.enforce_mode = pbcpp::EnforceOptimizeMode::kSpeed;
    } else if ("code_size" == key) {
      file_options.enforce_mode = pbcpp::EnforceOptimizeMode::kCodeSize;
    } else if ("lite" == key) {
      file_options.enforce_mode = pbcpp::EnforceOptimizeMode::kLiteRuntime;
    } else {
      *error = "Unknown value for experimental_tail_call_table_mode: " + val;
      return false;
    }
  }

  std::string basename = pbcpp::StripProto(file->name());
  basename.append(".pb");

  // 文件生成器
  PGTCppFileGenerator file_generator(file, file_options);

  // 头文件
  {
    std::unique_ptr<pb::io::ZeroCopyOutputStream> output(
        generator_context->Open(basename + ".h"));
    pb::io::Printer printer(output.get(), '$');
    file_generator.GenerateHeader(&printer);
  }

  // 源文件
  {
    std::unique_ptr<pb::io::ZeroCopyOutputStream> output(
        generator_context->Open(basename + ".cc"));
    pb::io::Printer printer(output.get(), '$');
    file_generator.GenerateSource(&printer);
  }

  return true;
}

}  // namespace protoc

}  // namespace tpn
