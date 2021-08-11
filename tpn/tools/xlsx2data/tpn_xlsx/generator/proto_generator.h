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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_PROTO_GENERATOR_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_PROTO_GENERATOR_H_

#include "xlsx2data_common.h"
#include "helper.h"

namespace tpn {

class FileHelper;

namespace xlsx {

///// proto文件生成器
//class TPN_XLSX2DATA_API ProtoGenerator {
// public:
//  /// 构造函数
//  ///  @param[in]   workbook    要转换的工作簿
//  ///  @param[in]   out_file    输出文件句柄
//  ProtoGenerator(xlnt::workbook &workbook, FileHelper &out_file);
//
//  ~ProtoGenerator();
//
//  /// 生成proto文件
//  ///  @return 生成成功返回true
//  bool Generate();
//
// private:
//  /// 生成sheet表的proto结构
//  ///  @param[in]   worksheet   工作表
//  ///  @return 生成成功返回true
//  bool GenerateSheetInfo(xlnt::worksheet &worksheet);
//
// private:
//  xlnt::workbook &workbook_;  ///< 当前处理的工作簿
//  FileHelper &out_file_;      ///< 输出文件句柄
//  Printer printer_;           ///< 打印器
//};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_PROTO_GENERATOR_H_
