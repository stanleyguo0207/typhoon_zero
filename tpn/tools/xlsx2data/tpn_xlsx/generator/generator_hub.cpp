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

#include "generator_hub.h"

#include <filesystem>

#include "log.h"
#include "config.h"
#include "debug_hub.h"
#include "helper.h"

namespace fs = std::filesystem;

namespace tpn {

namespace xlsx {

bool GeneratorHub::Load(std::string_view path, std::string &error,
                        bool reload /* = false */) {
  if (!reload) {
    path_ = {path.data(), path.size()};
  }

  xlsx_file_paths_.clear();

  auto xlsx_path = fs::path(path);
  xlsx_path.make_preferred();

  try {
    for (auto &&iter : fs::directory_iterator(xlsx_path)) {
      auto file_path = iter.path();
      if (WorkBookIsOutput(file_path.filename().generic_string())) {
        xlsx_file_paths_.emplace_back(file_path.generic_string());
      }
    }

    if (xlsx_file_paths_.empty()) {
      error = std::string("coudn't find xlsx files in") + " (" + path_ + ") ";
      return false;
    }

    file_prefix_ = g_config->GetStringDefault("xlsx_file_prefix", "data_hub");
  } catch (fs::filesystem_error &e) {
    error = std::string{e.what()} + " (" + path_ + ") ";
    return false;
  } catch (const std::exception &ex) {
    error = std::string{ex.what()} + " (" + path_ + ") ";
    return false;
  }

  if (!json_gen_.Load(error)) {
    return false;
  }

  if (!proto_gen_.Load(error)) {
    return false;
  }

  if (!cpp_gen_.Load(error)) {
    return false;
  }

  return true;
}

bool GeneratorHub::Reload(std::string &error) { return Load({}, error, true); }

bool GeneratorHub::Generate() {
  LOG_INFO("xlsx generator start generate");

  for (auto &&path : xlsx_file_paths_) {
    LOG_INFO("xlsx generator start generate path: {}", path);

    xlnt::workbook wb;
    try {
      wb.load(path);
    } catch (xlnt::exception &e) {
      LOG_ERROR("xlsx generator load workbook {} exception error : {}", path,
                e.what());
      return false;
    } catch (...) {
      LOG_ERROR("xlsx generator load workbook {} exception ...", path);
      return false;
    }

    for (auto &&sheet : wb) {
      if (!SheetTitleIsOutput(sheet.title())) {
        continue;
      }

      LOG_INFO("xlsx generator start generate anlayst path: {}, sheet: {}",
               path, sheet.title());
      // 分析器
      if (!GenerateAnlayst(sheet)) {
        LOG_ERROR("xlsx generator generate anlayst error path: {}, sheet: {}",
                  path, sheet.title());
        return false;
      }
      LOG_INFO("xlsx generator finish generate anlayst path: {}, sheet: {}",
               path, sheet.title());

      LOG_INFO("xlsx generator start generate json path: {}, sheet: {}", path,
               sheet.title());
      // 原生数据
      if (!GenerateJson(sheet)) {
        LOG_ERROR("xlsx generator generate json error path: {}, sheet: {}",
                  path, sheet.title());
        return false;
      }
      LOG_INFO("xlsx generator finish generate json path: {}, sheet: {}", path,
               sheet.title());

      LOG_INFO("xlsx generator start generate proto path: {}, sheet: {}", path,
               sheet.title());
      // proto描述文件
      if (!GenerateProto(sheet)) {
        LOG_ERROR("xlsx generator generate proto error path: {}, sheet: {}",
                  path, sheet.title());
        return false;
      }
      LOG_INFO("xlsx generator finish generate proto path: {}, sheet: {}", path,
               sheet.title());

      LOG_INFO("xlsx generator start generate cpp path: {}, sheet: {}", path,
               sheet.title());
      // cpp文件
      if (!GenerateCpp(sheet)) {
        LOG_ERROR("xlsx generator generate cpp error path: {}, sheet: {}",
                  path, sheet.title());
        return false;
      }
      LOG_INFO("xlsx generator finish generate cpp path: {}, sheet: {}", path,
               sheet.title());
    }

    LOG_INFO("xlsx generator finish generate path: {}", path);
  }

  LOG_INFO("xlsx generator start generate json file");
  // json 文件生成
  if (!GenerateJsonFile()) {
    LOG_ERROR("xlsx generator generate json file error");
    return false;
  }
  LOG_INFO("xlsx generator finish generate json file");

  LOG_INFO("xlsx generator start generate cpp tail");
  // cpp 尾部内容生成
  if (!GenerateCppTail()) {
    LOG_ERROR("xlsx generator generate cpp tail error");
    return false;
  }
  LOG_INFO("xlsx generator finish generate cpp tail");

  LOG_INFO("xlsx generator finish generate");

  return true;
}

std::string_view GeneratorHub::GetFilePrefix() const { return file_prefix_; }

Analyst &GeneratorHub::GetAnalyst() { return analyst_; }

bool GeneratorHub::GenerateAnlayst(xlnt::worksheet &worksheet) {
  return analyst_.Analyze(worksheet);
}

bool GeneratorHub::GenerateJson(xlnt::worksheet &worksheet) {
  return json_gen_.Analyze(worksheet);
}

bool GeneratorHub::GenerateProto(xlnt::worksheet &worksheet) {
  return proto_gen_.Analyze(worksheet);
}

bool GeneratorHub::GenerateCpp(xlnt::worksheet &worksheet) {
  return cpp_gen_.Analyze(worksheet);
}

bool GeneratorHub::GenerateJsonFile() { return json_gen_.Generate(); }

bool GeneratorHub::GenerateCppTail() { return cpp_gen_.GenerateTail(); }

TPN_SINGLETON_IMPL(GeneratorHub)

}  // namespace xlsx

}  // namespace tpn
