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
  auto cpp_file_path =
      g_config->GetStringDefault("xlsx_cpp_dir", "xlsx2data/cpp");

  std::string cpp_file_head_path = fmt::format(
      "{}/{}.h", cpp_file_path, g_xlsx2data_generator->GetFilePrefix());
  std::string cpp_file_src_path = fmt::format(
      "{}/{}.cpp", cpp_file_path, g_xlsx2data_generator->GetFilePrefix());

  std::string bin_file_src_path = fmt::format(
      "{}/bin_generator.cpp",
      g_config->GetStringDefault("xlsx_generator_dir", "xlsx2data/generator"));
  try {
    cpp_file_head_.Open(cpp_file_head_path, true);
    cpp_file_src_.Open(cpp_file_src_path, true);
    bin_file_src_.Open(bin_file_src_path, true);
  } catch (FileException &ex) {
    error = std::string{ex.what()};
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()};
    return false;
  }

  printer_.Reset();
  init_flag_ = true;
  init_printer_.Reset();
  bin_printer_.Reset();

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
    printer_.Println(fmt::format(
        "std::map<std::string, {}::{}> {}_map_;", GetProto3MessageName(title),
        CapitalizeFirstLetter(title), LowercaseString(title)));
    printer_.Outdent();
    cpp_file_head_.Write(printer_.GetBuf());

    // 源文件
    printer_.Reset();
    if (!g_xlsx2data_generator->GetAnalyst().GenerateCppSourceData(
            printer_, init_printer_, init_flag_, title_raw)) {
      LOG_ERROR("cpp generator cpp srouce data error, title: {}", title_raw);
      return false;
    }
    init_flag_ = false;
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

  GenerateSourceMethodInit();
  GenerateSourceNamespaceEnd();

  GenerateBinGenerator();

  cpp_file_head_.Flush();
  cpp_file_head_.Close();

  cpp_file_src_.Flush();
  cpp_file_src_.Close();

  bin_file_src_.Flush();
  bin_file_src_.Close();
  return true;
}

void CppGenerator::GenerateHeadInclude() {
  printer_.Reset();
  printer_.Println("#include <map>");
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
  GenerateHeadMethodInit();
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

void CppGenerator::GenerateHeadMethodInit() {
  printer_.Println("bool Init(DataHubMap &data_map);");
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
  printer_.Println("#include <fstream>");
  printer_.Println("#include <filesystem>");
  printer_.Println("");
  printer_.Println("#include \"log.h\"");
  printer_.Println("#include \"utils.h\"");
  printer_.Println("#include \"config.h\"");
  printer_.Println("#include \"debug_hub.h\"");
  printer_.Println("");
  printer_.Println("namespace fs = std::filesystem;");
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
      fmt::format("bool {}Load(std::string_view path, std::string &error, bool "
                  "reload /*  = false */) {{",
                  GetCppDataHubMgrNameWithArea()));
  printer_.Println(
      "  if (!reload) {\n"
      "    path_ = {path.data(), path.size()};\n"
      "  }"
      "  auto data_path = fs::path(path_);\n"
      "  data_path.make_preferred();\n"
      "  auto data_file = fs::absolute(data_path);\n"
      "  DataHubMap data_map;\n"
      "  try {\n"
      "    std::fstream input(data_file, std::fstream::in | "
      "std::fstream::binary);\n"
      "    if (!input) {\n"
      "      LOG_ERROR(\"file: {} not found.\", data_file);\n"
      "      return false;\n"
      "    } else if (data_map.ParseFromIstream(&input) {\n"
      "      LOG_ERROR(\"file: {} parse failed.\", data_file);\n"
      "      return false;\n"
      "    }\n"
      "  } catch (fs::filesystem_error &e) {\n"
      "    error = std::string{e.what()} + \" (\" + path_ + \") \";\n"
      "    return false;\n"
      "  } catch (const std::exception &ex) {\n"
      "    error = std::string{ex.what()} + \" (\" + path_ + \") \";\n"
      "    return false;\n"
      "  }\n"
      "\n"
      "  if (!Init(data_map)) {\n"
      "    error = \"Init error.\";\n"
      "    return false;\n"
      "  }\n"
      "\n"
      "  return true;\n"
      "}\n");
}

void CppGenerator::GenerateSourceMethodReLoad() {
  printer_.Println(fmt::format("bool {}Reload(std::string &error) {{",
                               GetCppDataHubMgrNameWithArea()));
  printer_.Indent();

  printer_.Println("return Load({}, error, true);");

  printer_.Outdent();
  printer_.Println("}");
  printer_.Println("");
}

void CppGenerator::GenerateSourceMethodInit() {
  printer_.Reset();
  printer_.Println(fmt::format("bool {}Init(DataHubMap &data_map) {{",
                               GetCppDataHubMgrNameWithArea()));
  printer_.Println("  for (auto &&[key, val] : data_map.datas()) {");
  cpp_file_src_.Write(printer_.GetBuf());

  cpp_file_src_.Write(init_printer_.GetBuf());
  init_printer_.Reset();

  printer_.Reset();
  printer_.Println("  }");
  printer_.Println("  return true;");
  printer_.Println("}");
  printer_.Println("");
  cpp_file_src_.Write(printer_.GetBuf());
}

void CppGenerator::GenerateSourceMethodGetPath() {
  printer_.Println(fmt::format("std::string_view {}GetPath() {{",
                               GetCppDataHubMgrNameWithArea()));
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

void CppGenerator::GenerateBinGenerator() {
  bin_printer_.Reset();
  bin_printer_.Println(fmt::format(
      "{license}"
      "\n"
      "#include \"bin_generator.h\"\n"
      "\n"
      "#include <fstream>\n"
      "#include <filesystem>\n"
      "\n"
      "#include <google/protobuf/stubs/common.h>\n"
      "#include <google/protobuf/util/json_util.h>\n"
      "#include <rapidjson/document.h>\n"
      "#include <rapidjson/writer.h>\n"
      "#include <rapidjson/filereadstream.h>\n"
      "\n"
      "#include \"log.h\"\n"
      "#include \"config.h\"\n"
      "#include \"data_hub.pb.h\"\n"
      "#include \"generator_hub.h\"\n"
      "\n"
      "namespace fs = std::filesystem;\n"
      "\n"
      "namespace tpn {{\n"
      "\n"
      "namespace xlsx {{\n"
      "\n"
      "BinGenerator::BinGenerator() {{}}\n"
      "\n"
      "BinGenerator::~BinGenerator() {{}}\n"
      "\n"
      "bool BinGenerator::Generate() {{\n"
      "  GOOGLE_PROTOBUF_VERIFY_VERSION;\n"
      "  std::shared_ptr<void> protobuf_handle(\n"
      "    nullptr, [](void *) {{ "
      "google::protobuf::ShutdownProtobufLibrary(); }});\n"
      "\n"
      "  std::string json_file_path = fmt::format(\n"
      "    \"{{}}/{{}}.json\", "
      "g_config->GetStringDefault(\"xlsx_json_dir\", \"xlsx2data/json\"),\n"
      "    g_xlsx2data_generator->GetFilePrefix());\n"
      "\n"
      "  std::string bin_file_path = fmt::format(\n"
      "    \"{{}}/{{}}.bin\", g_config->GetStringDefault(\"xlsx_bin_dir\", "
      "\"xlsx2data/bin\"),\n"
      "    g_xlsx2data_generator->GetFilePrefix());\n"
      "\n"
      "  auto json_path = fs::path(json_file_path);\n"
      "  json_path.make_preferred();\n"
      "  auto json_file = fs::absolute(json_path);\n"
      "\n"
      "  auto bin_path = fs::path(bin_file_path);\n"
      "  bin_path.make_preferred();\n"
      "  auto bin_file = fs::absolute(bin_path);\n"
      "  if (!fs::exists(bin_file.parent_path())) {{\n"
      "    fs::create_directories(bin_file.parent_path());\n"
      "  }}\n"
      "  try {{\n"
      "#if (TPN_COMPILER == TPN_COMPILER_MSVC)\n"
      "    const char *mask = \"rb\";\n"
      "#else\n"
      "    const char *mask = \"r\";\n"
      "#endif\n"
      "    auto *fp  = std::fopen(json_file.generic_string().c_str(), mask);\n"
      "    auto len  = fs::file_size(json_path);\n"
      "    char *buf = new char[len + 1];\n"
      "    rapidjson::FileReadStream is(fp, buf, len + 1);\n"
      "    rapidjson::Document d;\n"
      "    d.ParseStream<rapidjson::kParseCommentsFlag>(is);\n"
      "    if (d.HasParseError()) {{\n"
      "      LOG_ERROR(\"parse json error, {{}}\", d.GetParseError());\n"
      "      return false;\n"
      "    }}\n"
      "\n"
      "    rapidjson::StringBuffer buffer;\n"
      "    rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);\n"
      "    d.Accept(writer);\n"
      "\n"
      "    data::DataHubMap data_map;\n"
      "    auto status = "
      "::google::protobuf::util::JsonStringToMessage(buffer.GetString(), "
      "&data_map);\n"
      "    if (!status.ok()) {{\n"
      "      LOG_ERROR(\"Failed to write data_hub protobuf bin.error: {{}}\", "
      "status.message());\n"
      "      return false;\n"
      "    }}\n"
      "\n"
      "    std::fstream output(bin_file_path, std::fstream::out | "
      "std::fstream::trunc | std::fstream::binary);\n"
      "    if (!data_map.SerializeToOstream(&output)) {{\n"
      "      LOG_ERROR(\"Failed to write data_hub protobuf bin.\");\n"
      "      return false;\n"
      "    }}\n"
      "  }} catch (fs::filesystem_error &e) {{\n"
      "    LOG_ERROR(\"{{}},error: {{}}\", json_file_path, e.what());\n"
      "    return false;\n"
      "  }} catch (const std::exception &ex) {{\n"
      "    LOG_ERROR(\"{{}},error: {{}}\", json_file_path, ex.what());\n"
      "    return false;\n"
      "  }}\n"
      "  return true;\n"
      "}}\n"
      "\n"
      "}}  // namespace xlsx\n"
      "\n"
      "}}  // namespace tpn\n",
      "license"_a = GetLicense()));
  bin_file_src_.Write(bin_printer_.GetBuf());
}

}  // namespace xlsx

}  // namespace tpn
