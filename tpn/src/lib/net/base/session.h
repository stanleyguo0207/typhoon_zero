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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_

#include "net_common.h"

namespace tpn {

namespace net {

class SessionBase {
 public:
  SessionBase();

  ~SessionBase();

  /// 请求
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   request         请求数据
  ///  @param[in]   callback        回应回调
  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request,
                   std::function<void(ByteBuffer)> callback);

  /// 请求
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   request         请求数据
  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request);

  /// 回应
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   token           令牌
  ///  @param[in]   status          状态
  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    uint32_t status);

  /// 回应
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   token           令牌
  ///  @param[in]   response        回应数据
  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    const google::protobuf::Message *response);

  /// 获取调用者信息
  ///  @return 调用者信息
  std::string GetCallerInfo() const;

 protected:
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SESSION_H_
