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

#include "pgt_cpp_code_gen.h"
#include "pgt_cpp_service_gen.h"

namespace tpn {

namespace protoc {

PGTCppFileGenerator::PGTCppFileGenerator(const pb::FileDescriptor *file,
                                         const pbcpp::Options &options)
    : file_(file), options_(options) {}

PGTCppFileGenerator::~PGTCppFileGenerator() {}

void PGTCppFileGenerator::GenerateHeader(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateSource(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateBuildDescriptors(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateNamespaceOpeners(pb::io::Printer *printer) {}

void PGTCppFileGenerator::GenerateNamespaceClosers(pb::io::Printer *printer) {}

}  // namespace protoc

}  // namespace tpn
