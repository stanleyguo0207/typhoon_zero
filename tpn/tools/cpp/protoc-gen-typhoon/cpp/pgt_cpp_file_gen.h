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

#ifndef TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_FILE_GEN_H_
#define TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_FILE_GEN_H_

#include <algorithm>
#include <memory>
#include <set>
#include <string>
#include <vector>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/compiler/cpp/cpp_field.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>

#include "pgt_config.h"
#include "pgt_fwd.h"

namespace pb    = google::protobuf;
namespace pbcpp = pb::compiler::cpp;

namespace tpn {

namespace protoc {

class PGTCppServiceGenerator;

class PGTCppFileGenerator {
 public:
  PGTCppFileGenerator(const pb::FileDescriptor *file,
                      const pbcpp::Options &options);

  ~PGTCppFileGenerator();

  /// 产生头文件
  ///  @param[in]   printer     打印器
  void GenerateHeader(pb::io::Printer *printer);

  /// 产生源文件
  ///  @param[in]   printer     打印器
  void GenerateSource(pb::io::Printer *printer);

 private:
  void GenerateBuildDescriptors(pb::io::Printer *printer);

  void GenerateNamespaceOpeners(pb::io::Printer *printer);
  void GenerateNamespaceClosers(pb::io::Printer *printer);

  const pb::FileDescriptor *file_;  ///<  文件句柄
  const pbcpp::Options options_;    ///< 选项

  std::vector<std::unique_ptr<pbcpp::MessageGenerator>> message_generators_;
  std::vector<std::unique_ptr<pbcpp::EnumGenerator>> enum_generators_;
  std::vector<std::unique_ptr<PGTCppServiceGenerator>> service_generators_;
  std::vector<std::unique_ptr<pbcpp::ExtensionGenerator>> extension_generators_;

  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS(PGTCppFileGenerator);
};

}  // namespace protoc

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_CPP_PROTOC_GEN_TYPHOON_CPP_PGT_CPP_FILE_GEN_H_
