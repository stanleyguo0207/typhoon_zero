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

#ifndef TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_CPP_GENERATOR_H_
#define TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_CPP_GENERATOR_H_

#include "file_helper.h"
#include "xlsx2data_common.h"
#include "helper.h"

namespace tpn {

namespace xlsx {

/// cpp文件生成器
class TPN_XLSX2DATA_API CppGenerator {
 public:
  CppGenerator();
  ~CppGenerator();

  /// 加载配置
  ///  @param[out]  error     读取数据错误信息
  ///  @return 加载成功返回true
  bool Load(std::string &error);

  /// 分析数据
  ///  @param[in]   worksheet         工作表
  ///  @return 成功返回true
  bool Analyze(xlnt::worksheet &worksheet);

  /// 生成尾部数据
  ///  @return 成功返回true
  bool GenerateTail();

 private:
  /// 头文件包含
  void GenerateHeadInclude();

  /// 头文件守护开始
  void GenerateHeadGuardBegin();

  /// 头文件守护结束
  void GenerateHeadGuardEnd();

  /// 头文件命名空间开始
  void GenerateHeadNamespaceBegin();

  /// 头文件命名空间结束
  void GenerateHeadNamespaceEnd();

  /// 头文件类开始
  void GenerateHeadClassBegin();

  /// 头文件类结束
  void GenerateHeadClassEnd();

  /// 头文件方法开始
  void GenerateHeadMethodBegin();

  /// 头文方法 加载
  void GenerateHeadMethodLoad();

  /// 头文方法 重新加载
  void GenerateHeadMethodReLoad();

  /// 头文方法 初始化
  void GenerateHeadMethodInit();

  /// 头文方法 获取路径
  void GenerateHeadMethodGetPath();

  /// 头文件方法结束
  void GenerateHeadMethodEnd();

  /// 头文件类单例
  void GenerateHeadSingleton();

  /// 头文件类实例声明
  void GenerateHeadInstance();

 private:
  /// 源文件包含
  void GenerateSourceInclude();

  /// 源文件命名空间开始
  void GenerateSourceNamespaceBegin();

  /// 源文件命名空间结束
  void GenerateSourceNamespaceEnd();

  /// 源文方法
  void GenerateSourceMethod();

  /// 源文方法 加载
  void GenerateSourceMethodLoad();

  /// 源文方法 重新加载
  void GenerateSourceMethodReLoad();

  /// 源文方法 初始化
  void GenerateSourceMethodInit();

  /// 源文方法 获取路径
  void GenerateSourceMethodGetPath();

  /// 源文件类单例
  void GenerateSourceSingleton();

 private:
  FileHelper cpp_file_head_;  ///< data_hub .h 文件
  FileHelper cpp_file_src_;   ///< data_hub .cpp 文件
  Printer printer_;           ///< 打印器
  bool init_flag_{true};      ///< init函数标记
  Printer init_printer_;      ///< init函数打印器
};

}  // namespace xlsx

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_TOOLS_XLSX2DATA_TPN_XLSX_GENERATOR_CPP_GENERATOR_H_
