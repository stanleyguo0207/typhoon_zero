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

#include "proto_generator.h"

#include "log.h"
#include "utils.h"
#include "file_helper.h"
#include "debug_hub.h"

namespace tpn {

namespace xlsx {

//namespace {
//
///// 内部类型转换为proto3类型
//static std::string GetProto3Type(std::string_view type_name) {
//  std::string ret;
//  auto type = GetTypeByTypeName(type_name);
//  switch (type) {
//    case XlsxDataType::kXlsxDataTypeDouble:
//    case XlsxDataType::kXlsxDataTypeFloat: {
//      ret.assign(type_name.data(), type_name.size());
//    } break;
//    case XlsxDataType::kXlsxDataTypeI8:
//    case XlsxDataType::kXlsxDataTypeI16: {
//      ret.assign("sint32");
//    } break;
//    case XlsxDataType::kXlsxDataTypeI32: {
//      ret.assign("sfixed32");
//    } break;
//    case XlsxDataType::kXlsxDataTypeI64: {
//      ret.assign("sint64");
//    } break;
//    case XlsxDataType::kXlsxDataTypeU8:
//    case XlsxDataType::kXlsxDataTypeU16: {
//      ret.assign("uint32");
//    } break;
//    case XlsxDataType::kXlsxDataTypeU32: {
//      ret.assign("fixed32");
//    } break;
//    case XlsxDataType::kXlsxDataTypeU64: {
//      ret.assign("uint64");
//    } break;
//    case XlsxDataType::kXlsxDataTypeStr: {
//      ret.assign("string");
//    } break;
//    default: {
//      TPN_ASSERT(false, "cound't transform to proto3 type, type_name : {}",
//                 type_name);
//    } break;
//  }
//
//  return std::move(ret);
//}
//
//static constexpr size_t s_key_type_max_size = 8;  ///< key的最大长度为64位
//
///// 获取指定类型的长度，已c++类型为标准
//static const size_t GetTypeSizeOf(std::string_view type_name) {
//  size_t ret = 0;
//  auto type  = GetTypeByTypeName(type_name);
//
//  switch (type) {
//    case XlsxDataType::kXlsxDataTypeI8:
//    case XlsxDataType::kXlsxDataTypeU8: {
//      ret = 1;
//    } break;
//    case XlsxDataType::kXlsxDataTypeI16:
//    case XlsxDataType::kXlsxDataTypeU16: {
//      ret = 2;
//    } break;
//    case XlsxDataType::kXlsxDataTypeI32:
//    case XlsxDataType::kXlsxDataTypeU32: {
//      ret = 4;
//    } break;
//    case XlsxDataType::kXlsxDataTypeI64:
//    case XlsxDataType::kXlsxDataTypeU64: {
//      ret = 8;
//    } break;
//
//    default: {
//      TPN_ASSERT(
//          false,
//          "cound't get size of type only support integer, type_name : {}",
//          type_name);
//    } break;
//  }
//
//  return ret;
//}
//
//}  // namespace
//
//ProtoGenerator::ProtoGenerator(xlnt::workbook &workbook, FileHelper &out_file)
//    : workbook_(workbook), out_file_(out_file) {}
//
//ProtoGenerator::~ProtoGenerator() {}
//
//bool ProtoGenerator::Generate() {
//  LOG_INFO("proto generator start generate");
//
//  printer_.Reset();
//
//  for (auto &&sheet : workbook_) {
//    LOG_INFO("proto generator load sheet: {}", sheet.title());
//    if (!tpn::xlsx::SheetTitleIsOutput(sheet.title())) {
//      continue;
//    }
//
//    GenerateSheetInfo(sheet);
//  }
//
//  LOG_INFO("proto generator finish generate");
//
//  return true;
//}
//
//bool ProtoGenerator::GenerateSheetInfo(xlnt::worksheet &worksheet) {
//  auto &&ranges = worksheet.rows();
//  if (ranges.length() > 1) {  // 第一行为此列字段的标签定义
//    printer_.Reset();
//
//    std::string title_str = worksheet.title().substr(1);
//
//    printer_.Println(
//        fmt::format("message {} {{", GetDataHubItemName(title_str)));
//    printer_.Indent();
//
//    auto row = ranges[0];
//
//    for (auto &&cell : row) {
//      printer_.Println(fmt::format("{}", cell.to_string()));
//    }
//
//    printer_.Outdent();
//    printer_.Println("}");
//    printer_.Println("");
//
//    printer_.Println(
//        fmt::format("message {} {{", GetDataHubMapName(title_str)));
//    printer_.Indent();
//
//    printer_.Println(fmt::format("map<{}, {}> map_data = 1;", "fixed32",
//                                 GetDataHubItemName(title_str)));
//
//    printer_.Outdent();
//    printer_.Println("}");
//    printer_.Println("");
//
//    out_file_.Write(printer_.GetBuf());
//  }
//
//  return true;
//}

}  // namespace xlsx

}  // namespace tpn
