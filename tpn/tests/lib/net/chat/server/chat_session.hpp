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

#ifndef TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SESSION_H_
#define TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SESSION_H_

#include <deque>
#include <string>

#include "net.h"

#include "chat_room.h"
#include "chat_service.h"
#include "chat_service_dispather.h"

using namespace tpn;

namespace test {

class TcpChatSession
    : public net::TcpSessionBase<TcpChatSession, net::TemplateArgsTcpSession> {
 public:
  using Super =
      net::TcpSessionBase<TcpChatSession, net::TemplateArgsTcpSession>;
  using Self = TcpChatSession;

  using Super::TcpSessionBase;

  explicit TcpChatSession(ChatRoom &room, net::IoHandle &io_handle,
                          net::SessionMgr<Self> &session_mgr, size_t buffer_max,
                          size_t buffer_prepare)
      : Super(io_handle, session_mgr, buffer_max, buffer_prepare),
        room_(room) {}

  ~TcpChatSession() = default;

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request,
                   std::function<void(MessageBuffer)> callback) {
    NET_DEBUG("TcpServiceSessionBase SendRequest callback");

    // 先忽略
    std::ignore = callback;

    SendRequest(service_hash, method_id, request);
  }

  void SendRequest(uint32_t service_hash, uint32_t method_id,
                   const google::protobuf::Message *request) {
    NET_DEBUG("TcpServiceSessionBase SendRequest");

    protocol::Header header;
    header.set_service_id(0);
    header.set_service_hash(service_hash);
    header.set_method_id(method_id);
    header.set_token(request_token_++);
    header.set_size(request->ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                         request->GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet.GetWritePointer();
    packet.WriteCompleted(request->GetCachedSize());
    request->SerializeToArray(ptr, request->GetCachedSize());

    Send(std::move(packet));
  }

  void SendResponse(uint32_t service_hash, uint32_t method_id, uint32_t token,
                    uint32_t status) {
    NET_DEBUG("TcpServiceSessionBase SendResponse status");

    protocol::Header header;
    header.set_service_id(0xFE);
    header.set_service_hash(service_hash);
    header.set_method_id(method_id);
    header.set_token(token);
    header.set_status(status);

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());

    Send(std::move(packet));
  }

  void SendResponse(uint32_t service_hash, uint32_t method_id,
                    protocol::ErrorCode token,
                    const google::protobuf::Message *response) {
    NET_DEBUG("TcpServiceSessionBase SendResponse response");

    protocol::Header header;
    header.set_service_id(0);
    header.set_service_hash(service_hash);
    header.set_method_id(method_id);
    header.set_token(token);
    header.set_status(kErrorCodeOk);
    header.set_size(response->ByteSizeLong());

    uint16_t header_size = (uint16_t)header.ByteSizeLong();
    EndianRefMakeLittle(header_size);

    MessageBuffer packet(sizeof(header_size) + header.GetCachedSize() +
                         response->GetCachedSize());
    packet.Write(&header_size, sizeof(header_size));
    uint8_t *ptr = packet.GetWritePointer();
    packet.WriteCompleted(header.GetCachedSize());
    header.SerializePartialToArray(ptr, header.GetCachedSize());
    ptr = packet.GetWritePointer();
    packet.WriteCompleted(response->GetCachedSize());
    response->SerializeToArray(ptr, response->GetCachedSize());

    Send(std::move(packet));
  }

  std::string GetCallerInfo() const { return name_; }

  void SetName(std::string name) { name_.assign(name); }

  void Deliver(const std::string &msg) {
    protocol::TChatNtf ntf;
    ntf.add_message_list(msg);

    net::Service<Self, protocol::TChatListener>(this->GetSelfSptr())
        .ChatNtf(&ntf);
  }

  void Chat(const std::string &msg) { room_.Deliver(msg); }

  ChatRoom &GetRoom() const { return room_; }

  /// 处理断开连接
  /// 本函数重写了disconnect模块的HandleDisconnect的方法
  ///  @param[in]   ec          错误码
  ///  @param[in]   this_ptr    延长生命周期的智能指针
  void HandleDisconnect(const std::error_code &ec,
                        std::shared_ptr<Self> this_ptr) {
    IgnoreUnused(ec, this_ptr);

    room_.Leave(this->GetSelfSptr());
    Super::HandleDisconnect(ec, std::move(this_ptr));
  }

  void FireRecv(std::shared_ptr<Self> &this_ptr, protocol::Header &&header,
                MessageBuffer &&packet) {
    std::ignore = this_ptr;

    LOG_INFO("FireRecv recv data");

    chat_dispather->Dispatch(this->GetSelfSptr(), header.service_hash(),
                             header.token(), header.method_id(),
                             std::move(packet));
  }

 private:
  ChatRoom &room_;             ///< 房间
  std::string name_;           ///< 聊天用户名
  uint32_t request_token_{0};  ///< 令牌
};

}  // namespace test

#endif  // TYPHOON_ZERO_TPN_TESTS_LIB_NET_CHAT_SERVER_CHAT_SESSION_H_
