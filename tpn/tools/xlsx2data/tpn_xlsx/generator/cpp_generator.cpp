///
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

#include "cpp_generator.h"

#include "log.h"
#include "config.h"
#include "debug_hub.h"
#include "fmt_wrap.h"
#include "utils.h"
#include "helper.h"
#include "generator_hub.h"

namespace tpn {

namespace xlsx {

CppGenerator::CppGenerator() {}

CppGenerator::~CppGenerator() {}

bool CppGenerator::Load(std::string &error) {
  auto cpp_file_path = g_config->GetStringDefault("xlsx_cpp_dir", "cpp");

  std::string cpp_file_head_path = fmt::format(
      "{}/{}.h", cpp_file_path, g_xlsx2data_generator->GetFilePrefix());
  std::string cpp_file_src_path = fmt::format(
      "{}/{}.cpp", cpp_file_path, g_xlsx2data_generator->GetFilePrefix());
  try {
    cpp_file_head_.Open(cpp_file_head_path, true);
    cpp_file_src_.Open(cpp_file_src_path, true);
  } catch (FileException &ex) {
    error = std::string{ex.what()};
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()};
    return false;
  }

  printer_.Reset();

  // license
  auto license = GetLicense();
  printer_.Print(license);

  cpp_file_head_.Write(printer_.GetBuf());
  cpp_file_src_.Write(printer_.GetBuf());

  GenerateHeadGuardBegin();
  GenerateHeadInclude();
  GenerateHeadNamespaceBegin();
  GenerateHeadClassBegin();

  GenerateSourceInclude();
  GenerateSourceNamespaceBegin();

  return true;
}

bool CppGenerator::Analyze(xlnt::worksheet &worksheet) {
  LOG_INFO("cpp generator start analyze worksheet : {}", worksheet.title());

  auto &&ranges = worksheet.rows();
  if (ranges.length() > 0) {
    std::string title_raw = GetSheetTitle(worksheet.title());
    TPN_ASSERT(!title_raw.empty(), "sheet title error, title : {}",
               worksheet.title());

    std::string title = CapitalizeFirstLetter(title_raw);

    // 头文件
    printer_.Reset();
    printer_.Println("");
    printer_.Println(" public:");
    printer_.Indent();
    if (!g_xlsx2data_generator->GetAnalyst().GenerateCppHeadData(printer_,
                                                                 title_raw)) {
      LOG_ERROR("cpp generator cpp head data error, title: {}", title_raw);
      return false;
    }
    printer_.Outdent();

    printer_.Println(" private:");
    printer_.Indent();
    printer_.Println(fmt::format("std::map<std::string, {}> {}_map_;",
                                 GetProto3MessageName(title),
                                 LowercaseString(title)));
    printer_.Outdent();
    cpp_file_head_.Write(printer_.GetBuf());

    // 源文件
    printer_.Reset();
    if (!g_xlsx2data_generator->GetAnalyst().GenerateCppSourceData(printer_,
                                                                   title_raw)) {
      LOG_ERROR("cpp generator cpp srouce data error, title: {}", title_raw);
      return false;
    }
    printer_.Println("");
    cpp_file_src_.Write(printer_.GetBuf());
  }

  LOG_INFO("cpp generator start analyze worksheet : {}", worksheet.title());

  return true;
}

bool CppGenerator::GenerateTail() {
  GenerateHeadClassEnd();
  GenerateHeadNamespaceEnd();
  GenerateHeadInstance();
  GenerateHeadGuardEnd();

  GenerateSourceNamespaceEnd();
  return true;
}

void CppGenerator::GenerateHeadInclude() {
  printer_.Reset();
  printer_.Println("#include <map>");
  printer_.Println("#include <tuple>");
  printer_.Println("#include <string>");
  printer_.Println("#include <string_view>");
  printer_.Println("");
  printer_.Println("#include \"define.h\"");
  printer_.Println("#include \"data_hub.pb.h\"");
  printer_.Println("");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadGuardBegin() {
  printer_.Reset();
  printer_.Println("#ifndef __TYPHOON_DATA_HUB_H__");
  printer_.Println("#define __TYPHOON_DATA_HUB_H__");
  printer_.Println("");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadGuardEnd() {
  printer_.Reset();
  printer_.Println("#endif  // __TYPHOON_DATA_HUB_H__");
  printer_.Println("");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadNamespaceBegin() {
  printer_.Reset();
  printer_.Println("namespace tpn {");
  printer_.Println("");
  printer_.Println("namespace data {");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadNamespaceEnd() {
  printer_.Reset();
  printer_.Println("");
  printer_.Println("}  // namespace data");
  printer_.Println("");
  printer_.Println("}  // namespace tpn");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadClassBegin() {
  printer_.Reset();
  printer_.Println("");
  printer_.Println(
      fmt::format("class TPN_DATA_API {} {{", GetCppDataHubMgrName()));
  cpp_file_head_.Write(printer_.GetBuf());
  GenerateHeadMethodBegin();
}

void CppGenerator::GenerateHeadClassEnd() {
  GenerateHeadMethodEnd();
  GenerateHeadSingleton();
  printer_.Reset();
  printer_.Println("};");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadMethodBegin() {
  printer_.Reset();
  printer_.Println(" public:");
  printer_.Indent();
  GenerateHeadMethodLoad();
  GenerateHeadMethodReLoad();
  GenerateHeadMethodGetPath();

  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadMethodLoad() {
  printer_.Println(
      "bool Load(std::string_view path, std::string &error, bool reload = "
      "false);");
}

void CppGenerator::GenerateHeadMethodReLoad() {
  printer_.Println("bool Reload(std::string &error);");
}

void CppGenerator::GenerateHeadMethodGetPath() {
  printer_.Println("std::string_view GetPath();");
}

void CppGenerator::GenerateHeadMethodEnd() {
  printer_.Reset();
  printer_.Println("");
  printer_.Println(" private:");
  printer_.Indent();
  printer_.Println("std::string path_;");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadSingleton() {
  printer_.Reset();
  printer_.Println("");
  printer_.Indent();
  printer_.Println(
      fmt::format("TPN_SINGLETON_DECL({})", GetCppDataHubMgrName()));
  printer_.Outdent();
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateHeadInstance() {
  printer_.Reset();
  printer_.Println("");
  printer_.Println("/// global data manager instance");
  printer_.Println(fmt::format("#define g_data_hub tpn::data::{}::Instance()",
                               GetCppDataHubMgrName()));
  printer_.Println("");
  cpp_file_head_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateSourceInclude() {
  printer_.Reset();
  printer_.Println("#include \"data_hub.h\"");
  printer_.Println("");
  printer_.Println("#include <filesystem>");
  printer_.Println("");
  printer_.Println("#include \"utils.h\"");
  printer_.Println("#include \"debug_hub.h\"");
  printer_.Println("");
  cpp_file_src_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateSourceNamespaceBegin() {
  printer_.Reset();
  printer_.Println("namespace tpn {");
  printer_.Println("");
  printer_.Println("namespace data {");
  printer_.Println("");
  cpp_file_src_.Write(printer_.GetBuf());
  GenerateSourceMethod();
}

void CppGenerator::GenerateSourceNamespaceEnd() {
  printer_.Reset();
  printer_.Println("}  // namespace data");
  printer_.Println("");
  printer_.Println("}  // namespace tpn");
  printer_.Println("");
  cpp_file_src_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateSourceMethod() {
  printer_.Reset();
  GenerateSourceMethodLoad();
  GenerateSourceMethodReLoad();
  GenerateSourceMethodGetPath();
  cpp_file_src_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateSourceMethodLoad() {
  printer_.Println(
      "bool Load(std::string_view path, std::string &error, bool reload /*  = "
      "false */) {");
  printer_.Println("}");
  printer_.Println("");
}

void CppGenerator::GenerateSourceMethodReLoad() {
  printer_.Println("bool Reload(std::string &error) {");
  printer_.Indent();

  printer_.Println("return Load({}, error, true);");

  printer_.Outdent();
  printer_.Println("}");
  printer_.Println("");
}

void CppGenerator::GenerateSourceMethodGetPath() {
  printer_.Println("std::string_view GetPath() {");
  printer_.Indent();

  printer_.Println("return path_;");

  printer_.Outdent();
  printer_.Println("}");
  printer_.Println("");
}

void CppGenerator::GenerateSourceSingleton() {
  printer_.Reset();
  printer_.Println(
      fmt::format("TPN_SINGLETON_IMPL({})", GetCppDataHubMgrName()));
  cpp_file_src_.Write(printer_.GetBuf());
}

}  // namespace xlsx

}  // namespace tpn
