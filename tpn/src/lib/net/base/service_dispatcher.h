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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_DISPATCHER_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_DISPATCHER_H_

#include <memory>
#include <functional>
#include <unordered_map>

#include "log.h"
#include "net_common.h"
#include "byte_buffer.h"

namespace tpn {

namespace net {

/// 服务分发器
///  @tparam  SessionType     会话类型
template <typename SessionType>
class ServiceDispatcher {
 public:
  /// 服务分发
  /// 本接口会把数据发送到对应的服务去
  ///  @param[in]   session_sptr    会话
  ///  @param[in]   service_hash    服务key
  ///  @param[in]   token           令牌
  ///  @param[in]   method_id       对应服务中的方法编号
  ///  @param[in]   buffer          RPC数据
  void Dispatch(std::shared_ptr<SessionType> session_sptr,
                uint32_t service_hash, uint32_t token, uint32_t method_id,
                ByteBuffer buffer) {
    auto iter = dispatcher_.find(service_hash);
    if (dispatcher_.end() != iter) {
      iter->second(session_sptr, token, method_id, std::move(buffer));
    } else {
      NET_DEBUG("{} tried to call invalid service {:#x}",
                session_sptr->GetCallerInfo(), service_hash);
    }
  }

 protected:
  /// 添加服务到分发器中 需要在子类的构造函数中添加对应的服务
  ///  @tparam  ServiceType     服务类型
  template <typename ServiceType>
  void AddService() {
    dispatcher_[ServiceType::ServiceHash::value] =
        &ServiceDispatcher::Dispatch<ServiceType>;
  }

  /// 服务分发
  ///  @tparam  ServiceType     服务类型
  ///  @param[in]   token           令牌
  ///  @param[in]   method_id       对应服务中的方法编号
  ///  @param[in]   buffer          RPC数据
  template <typename ServiceType>
  static void Dispatch(std::shared_ptr<SessionType> session_sptr,
                       uint32_t token, uint32_t method_id, ByteBuffer buffer) {
    ServiceType(session_sptr)
        .CallServerMethod(token, method_id, std::move(buffer));
  }

  // using ServiceMethod = std::function(void(std::shared_ptr<SessionType>,
  //                                          uint32_t, uint32_t, ByteBuffer));
  using ServiceMethod = void (*)(std::shared_ptr<SessionType>, uint32_t,
                                 uint32_t, ByteBuffer);
  std::unordered_map<uint32_t, ServiceMethod> dispatcher_;  ///< 服务分发器
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_DISPATCHER_H_
