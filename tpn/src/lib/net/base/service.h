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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_H_

#include <memory>
#include <functional>

#include "net_common.h"

namespace tpn {

namespace protocol {}
using namespace tpn::protocol;

namespace net {

/// 自定义服务类
/// 继承自rpc的服务类型 注册到dispatcher用
///  @tparam  SessionType     会话类型
///  @tparam  ServiceType     服务类型
/// @note SessionType需要实现以下接口
/// @code
/// void SendRequest(uint32_t service_hash, uint32_t method_id,
///                  const google::protobuf::Message *request,
///                  std::function<void(MessageBuffer)> callback)
/// void SendRequest(uint32_t service_hash, uint32_t method_id,
///                  const google::protobuf::Message *request)
/// void SendResponse(uint32_t service_hash, uint32_t method_id,
///                   uint32_t token, uint32_t status)
/// void SendResponse(uint32_t service_hash, uint32_t method_id,
///                   uint32_t token, const google::protobuf::Message *response)
/// std::string GetCallerInfo() const
/// @endcode
template <typename SessionType, typename ServiceType>
class Service : public ServiceType {
 public:
  /// 构造函数
  ///  @param[in]   session_sptr    会话，用来调用响应的服务
  Service(std::shared_ptr<SessionType> session_sptr)
      : ServiceType(), session_sptr_(session_sptr) {}

  ~Service() = default;

 protected:
  /// 请求
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   request         请求数据
  ///  @param[in]   callback        回应回调
  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request,
                   std::function<void(MessageBuffer)> callback) override {
    session_sptr_->SendRequest(service_hash, method_id, request,
                               std::move(callback));
  }

  /// 请求
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   request         请求数据
  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request) override {
    session_sptr_->SendRequest(service_hash, method_id, request);
  }

  /// 回应
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   token           令牌
  ///  @param[in]   status          状态
  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    uint32_t status) override {
    session_sptr_->SendResponse(service_hash, method_id, token, status);
  }

  /// 回应
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   method_id       服务中对应的方法编号
  ///  @param[in]   token           令牌
  ///  @param[in]   response        回应数据
  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    const google::protobuf::Message *response) override {
    session_sptr_->SendResponse(service_hash, method_id, token, response);
  }

  /// 获取调用者信息
  ///  @return 调用者信息
  std::string GetCallerInfo() const override {
    return session_sptr_->GetCallerInfo();
  }

 protected:
  std::shared_ptr<SessionType> session_sptr_;  ///< 服务会话
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_H_
