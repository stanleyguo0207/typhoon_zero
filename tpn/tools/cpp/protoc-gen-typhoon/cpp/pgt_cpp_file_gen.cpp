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
// but WITHOUT ANY WARRANTY without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "pgt_cpp_file_gen.h"

#include <memory>
#include <string>
#include <vector>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>
#include <google/protobuf/compiler/cpp/cpp_extension.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/compiler/cpp/cpp_message.h>
#include <google/protobuf/compiler/cpp/cpp_file.h>
#include <google/protobuf/descriptor.pb.h>

#include "pgt_cpp_code_gen.h"
#include "pgt_cpp_service_gen.h"

namespace tpn {

namespace protoc {

PGTCppFileGenerator::PGTCppFileGenerator(const pb::FileDescriptor *file,
                                         const pbcpp::Options &options)
    : file_(file), options_(options), scc_analyzer_(options) {
  pbcpp::SetCommonVars(options, &variables_);
  variables_["dllexport_decl"] = options.dllexport_decl;

  std::vector<const pb::Descriptor *> msgs = pbcpp::FlattenMessagesInFile(file);
  for (int i = 0; i < msgs.size(); ++i) {
    pbcpp::MessageGenerator *msg_gen = new pbcpp::MessageGenerator(
        msgs[i], variables_, i, options, &scc_analyzer_);
    message_generators_.emplace_back(msg_gen);
    msg_gen->AddGenerators(&enum_generators_, &extension_generators_);
  }

  for (int i = 0; i < file->enum_type_count(); ++i) {
    enum_generators_.emplace_back(
        new pbcpp::EnumGenerator(file->enum_type(i), variables_, options));
  }

  for (int i = 0; i < file->service_count(); ++i) {
    service_generators_.emplace_back(
        new PGTCppServiceGenerator(file->service(i), options));
  }

  for (int i = 0; i < file->extension_count(); ++i) {
    extension_generators_.emplace_back(
        new pbcpp::ExtensionGenerator(file->extension(i), options));
  }

  pb::SplitStringUsing(file->package(), ".", &package_parts_);
}

PGTCppFileGenerator::~PGTCppFileGenerator() {}

void PGTCppFileGenerator::GenerateHeader(pb::io::Printer *printer) {
  printer->Print("hello world\n");
}

void PGTCppFileGenerator::GenerateSource(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateBuildDescriptors(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateNamespaceOpeners(pb::io::Printer *printer) {
  if (package_parts_.size() > 0) {
    printer->Print("\n");
  }

  for (auto &&part : package_parts_) {
    printer->Print("namespace $part$ {\n", "part", part);
  }
}

void PGTCppFileGenerator::GenerateNamespaceClosers(pb::io::Printer *printer) {
  if (package_parts_.size() > 0) {
    printer->Print("\n");
  }

  for (auto &&part : package_parts_) {
    printer->Print("}  //  namespace $part$\n", "part", part);
  }
}

}  // namespace protoc

}  // namespace tpn
