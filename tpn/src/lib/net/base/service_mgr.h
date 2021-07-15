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

#ifndef TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_MGR_H_
#define TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_MGR_H_

#include <functional>
#include <unordered_map>

#include "message_buffer.h"
#include "net_common.h"

namespace tpn {

namespace net {

/// 服务管理基类
/// 各服务器需要继承本类，打开AddService接口
///  @tapram  SessionType     会话类型
template <typename SessionType>
class ServiceMgr {
 public:
  ServiceMgr()  = default;
  ~ServiceMgr() = default;

  /// 注册服务
  ///  @tparam  ServiceType     服务类型
  template <typename ServiceType>
  TPN_INLINE void AddService() {
    dispatchers_[ServiceType::ServiceHash::value] =
        &ServiceMgr::Dispatch<ServiceType>;
  }

  /// 分发协议
  /// 如何确定调用的是对端的哪个方法呢？
  /// 1. 通过service_hash找到对应的服务，对端需要注册对应的服务
  /// 2. 通过method_id找到对应服务中的方法，不同服务中的相同方法id没有影响
  ///  @param[in]   session_sptr    会话
  ///  @param[in]   service_hash    服务索引
  ///  @param[in]   token           对端的令牌
  ///  @param[in]   method_id       服务内的方法id
  ///  @param[in]   buffer          协议数据
  TPN_INLINE void Dispatch(std::shared_ptr<SessionType> session_sptr,
                           uint32_t service_hash, uint32_t token,
                           uint32_t method_id, MessageBuffer buffer) {
    auto iter = dispatchers_.find(service_hash);
    if (dispatchers_.end() != iter) {
      iter->second(session_sptr, token, method_id, std::move(buffer));
    } else {
      NET_DEBUG("{} tried to call invalid service {:#x}",
                session_sptr->GetCallerInfo(), service_hash);
    }
  }

 protected:
  /// 内部分发
  ///  @tparam      ServiceType     服务类型
  ///  @param[in]   session_sptr    会话
  ///  @param[in]   service_hash    服务索引
  ///  @param[in]   token           对端的令牌
  ///  @param[in]   method_id       服务内的方法id
  ///  @param[in]   buffer          协议数据
  template <typename ServiceType>
  static void Dispatch(std::shared_ptr<SessionType> session_sptr,
                       uint32_t token, uint32_t method_id,
                       MessageBuffer buffer) {
    ServiceType(session_sptr)
        .CallServerMethod(token, method_id, std::move(buffer));
  }

 protected:
  /// 服务方法签名
  using ServiceMethod = void (*)(std::shared_ptr<SessionType>, uint32_t,
                                 uint32_t, MessageBuffer);

  std::unordered_map<uint32_t, ServiceMethod> dispatchers_;  ///< 服务分发器
};

}  // namespace net

}  // namespace tpn

#endif  // TYPHOON_ZERO_TPN_SRC_LIB_NET_BASE_SERVICE_MGR_H_
