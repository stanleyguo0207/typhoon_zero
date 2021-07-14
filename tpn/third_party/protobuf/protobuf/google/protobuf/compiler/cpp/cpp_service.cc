// Protocol Buffers - Google's data interchange format
// Copyright 2008 Google Inc.  All rights reserved.
// https://developers.google.com/protocol-buffers/
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
// notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
// copyright notice, this list of conditions and the following disclaimer
// in the documentation and/or other materials provided with the
// distribution.
//     * Neither the name of Google Inc. nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Author: kenton@google.com (Kenton Varda)
//  Based on original Protocol Buffers design by
//  Sanjay Ghemawat, Jeff Dean, and others.

#include <google/protobuf/compiler/cpp/cpp_service.h>
#include <google/protobuf/compiler/cpp/cpp_helpers.h>
#include <google/protobuf/compiler/cpp/cpp_options.h>
#include <google/protobuf/io/printer.h>
#include <google/protobuf/stubs/strutil.h>

#include <pgt_custom/pgt_custom_options.pb.h>

namespace google {
namespace protobuf {
namespace compiler {
namespace cpp {

ServiceGenerator::ServiceGenerator(
    const ServiceDescriptor *descriptor,
    const std::map<std::string, std::string> &vars, const Options &options)
    : descriptor_(descriptor), vars_(vars) {
  vars_["classname"] = descriptor_->name();
  vars_["full_name"] = descriptor_->full_name();
  if (options.dllexport_decl.empty()) {
    vars_["dllexport_decl"] = "";
  } else {
    vars_["dllexport_decl"] = options.dllexport_decl + " ";
  }

  if (descriptor_->options().HasExtension(tpn::protocol::service_options)) {
    vars_["service_hash"] =
        "  using ServiceHash = std::integral_constant<uint32_t, 0x" +
        ToUpper(HashToHex(
            HashServiceName(descriptor_->options()
                                .GetExtension(tpn::protocol::service_options)
                                .descriptor_name()))) +
        "u>;";
  } else {
    vars_["service_hash"] =
        "  using ServiceHash = std::integral_constant<uint32_t, 0x" +
        ToUpper(HashToHex(HashServiceName(descriptor_->full_name()))) + "u>;";
  }
}

ServiceGenerator::~ServiceGenerator() = default;

void ServiceGenerator::GenerateDeclarations(io::Printer *printer) {
  GenerateInterface(printer);
}

void ServiceGenerator::GenerateInterface(io::Printer *printer) {
  printer->Print(vars_,
                 "class $dllexport_decl$$classname$ : public ServiceBase {\n"
                 " public:\n"
                 "\n"
                 "  $classname$();\n"
                 "  virtual ~$classname$();\n"
                 "\n"
                 "$service_hash$\n");

  printer->Indent();

  printer->Print(
      vars_,
      "\n"
      "static const ::google::protobuf::ServiceDescriptor *descriptor();\n"
      "\n");

  // client
  if (!descriptor_->options().HasExtension(tpn::protocol::service_options) ||
      descriptor_->options()
          .GetExtension(tpn::protocol::service_options)
          .inbound()) {
    printer->Print(vars_,
                   "// inbound methods "
                   "--------------------------------------------------\n");

    GenerateClientMethodSignatures(printer);

    printer->Print("\n");
  }

  printer->Print(
      "void CallServerMethod(uint32_t token, uint32_t method_id, MessageBuffer "
      "buffer) final;");

  // server
  if (!descriptor_->options().HasExtension(tpn::protocol::service_options) ||
      descriptor_->options()
          .GetExtension(tpn::protocol::service_options)
          .outbound()) {
    printer->Outdent();

    printer->Print(
        "\n"
        " protected:\n");

    printer->Indent();

    printer->Print(
        "// outbound methods "
        "--------------------------------------------------\n");

    GenerateServerMethodSignatures(printer);
  }

  printer->Outdent();

  printer->Print(vars_,
                 "\n"
                 " private:\n"
                 "  uint32_t service_hash_{0};\n"
                 "\n"
                 "  GOOGLE_DISALLOW_EVIL_CONSTRUCTORS($classname$);\n"
                 "};\n");
}

void ServiceGenerator::GenerateImplementation(io::Printer *printer) {
  printer->Print(
      vars_,
      "$classname$::$classname$()\n"
      "  : service_hash_(ServiceHash::value) {}\n"
      "\n"
      "$classname$::~$classname$() {}\n"
      "\n"
      "const ::google::protobuf::ServiceDescriptor *$classname$::descriptor() "
      "{\n"
      "  ::google::protobuf::internal::AssignDescriptors(&$desc_table$);\n"
      "  return $file_level_service_descriptors$[$index_in_metadata$];\n"
      "}\n"
      "\n");

  if (!descriptor_->options().HasExtension(tpn::protocol::service_options) ||
      descriptor_->options()
          .GetExtension(tpn::protocol::service_options)
          .inbound()) {
    GenerateClientMethodImplementations(printer);
  }
  if (!descriptor_->options().HasExtension(tpn::protocol::service_options) ||
      descriptor_->options()
          .GetExtension(tpn::protocol::service_options)
          .outbound()) {
    GenerateServerCallMethod(printer);
    GenerateServerImplementations(printer);
  } else {
    printer->Print(vars_,
                   "void $classname$::CallServerMethod(uint32_t token, "
                   "uint32_t method_id, MessageBuffer /*buffer*/) {\n"
                   "  LOG_ERROR(\"{} Server tried to call server method {}\", "
                   "GetCallerInfo(), method_id);\n"
                   "}\n"
                   "\n");
  }
}

void ServiceGenerator::GenerateClientMethodSignatures(io::Printer *printer) {
  for (int i = 0; i < descriptor_->method_count(); ++i) {
    const MethodDescriptor *method = descriptor_->method(i);
    if (!method->options().HasExtension(tpn::protocol::method_options)) {
      continue;
    }

    std::map<std::string, std::string> sub_vars;
    sub_vars["name"]      = method->name();
    sub_vars["full_name"] = descriptor_->name() + "." + method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(tpn::protocol::method_options).id());
    sub_vars["input_type"]      = ClassName(method->input_type(), true);
    sub_vars["output_type"]     = ClassName(method->output_type(), true);
    sub_vars["input_type_name"] = method->input_type()->full_name();

    if ("NoResponse" != method->output_type()->name()) {
      printer->Print(
          sub_vars,
          "void $name$(const $input_type$ *request, "
          "std::function<void(const $output_type$ *)> response_callback, bool "
          "client = false, bool server = false);\n");
    } else {
      printer->Print(sub_vars,
                     "void $name$(const $input_type$ *request, bool client = "
                     "false, bool server = false);\n");
    }
  }
}

void ServiceGenerator::GenerateServerMethodSignatures(io::Printer *printer) {
  for (int i = 0; i < descriptor_->method_count(); ++i) {
    const MethodDescriptor *method = descriptor_->method(i);
    if (!method->options().HasExtension(tpn::protocol::method_options)) {
      continue;
    }

    std::map<std::string, std::string> sub_vars;
    sub_vars["name"]        = method->name();
    sub_vars["input_type"]  = ClassName(method->input_type(), true);
    sub_vars["output_type"] = ClassName(method->output_type(), true);

    if ("NoResponse" != method->output_type()->name()) {
      printer->Print(
          sub_vars,
          "virtual ::tpn::protocol::ErrorCode Handle$name$(const $input_type$ "
          "*request, "
          "$output_type$ *response, std::function<void(ServiceBase*, "
          "::tpn::protocol::ErrorCode, "
          "const ::google::protobuf::Message *)> &continuation);\n");
    } else {
      printer->Print(sub_vars,
                     "virtual ::tpn::protocol::ErrorCode Handle$name$(const "
                     "$input_type$ *request);\n");
    }
  }
}

void ServiceGenerator::GenerateClientMethodImplementations(
    io::Printer *printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor *method = descriptor_->method(i);
    if (!method->options().HasExtension(tpn::protocol::method_options)) {
      continue;
    }

    std::map<std::string, std::string> sub_vars;
    sub_vars["classname"] = vars_["classname"];
    sub_vars["name"]      = method->name();
    sub_vars["full_name"] = descriptor_->name() + "." + method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(tpn::protocol::method_options).id());
    sub_vars["input_type"]      = ClassName(method->input_type(), true);
    sub_vars["output_type"]     = ClassName(method->output_type(), true);
    sub_vars["input_type_name"] = method->input_type()->full_name();

    if ("NoResponse" != method->output_type()->name()) {
      printer->Print(
          sub_vars,
          "void $classname$::$name$(const $input_type$ *request, "
          "std::function<void(const $output_type$ *)> response_callback, bool "
          "client /*= false*/, bool server /*= false*/) {\n"
          "  LOG_DEBUG(\"{} Server called client method "
          "$full_name$($input_type_name${{ {} }})\", GetCallerInfo(), "
          "request->ShortDebugString());\n"
          "  std::function<void(MessageBuffer)> callback = "
          "[response_callback](MessageBuffer buffer) -> void {\n"
          "    $output_type$ response;\n"
          "    if (response.ParseFromArray(buffer.GetReadPointer(), "
          "buffer.GetActiveSize()))\n"
          "      response_callback(&response);\n"
          "  };\n"
          "  SendRequest(service_hash_, $method_id$ | (client ? 0x40000000 : "
          "0) | (server ? 0x80000000 : 0), request, std::move(callback));\n"
          "}\n"
          "\n");
    } else {
      printer->Print(
          sub_vars,
          "void $classname$::$name$(const $input_type$ *request, bool client "
          "/*= false*/, bool server /*= false*/) {\n"
          "  LOG_DEBUG(\"{} Server called client "
          "method $full_name$($input_type_name${{ {}  }})\", GetCallerInfo(), "
          "request->ShortDebugString());\n"
          "  SendRequest(service_hash_, $method_id$ | (client ? 0x40000000 : "
          "0) | (server ? 0x80000000 : 0), request);\n"
          "}\n"
          "\n");
    }
  }
}

void ServiceGenerator::GenerateServerCallMethod(io::Printer *printer) {
  printer->Print(vars_,
                 "void $classname$::CallServerMethod(uint32_t token, uint32_t "
                 "method_id, MessageBuffer buffer) {\n"
                 "  switch(method_id & 0x3FFFFFFF) {\n");

  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor *method = descriptor_->method(i);
    if (!method->options().HasExtension(tpn::protocol::method_options)) {
      continue;
    }
    std::map<std::string, std::string> sub_vars;
    sub_vars["classname"] = vars_["classname"];
    sub_vars["name"]      = method->name();
    sub_vars["full_name"] = descriptor_->name() + "." + method->name();
    sub_vars["method_id"] = SimpleItoa(
        method->options().GetExtension(tpn::protocol::method_options).id());
    sub_vars["input_type"]       = ClassName(method->input_type(), true);
    sub_vars["output_type"]      = ClassName(method->output_type(), true);
    sub_vars["input_type_name"]  = method->input_type()->full_name();
    sub_vars["output_type_name"] = method->output_type()->full_name();

    printer->Print(sub_vars,
                   "    case $method_id$: {\n"
                   "      $input_type$ request;\n"
                   "      if (!request.ParseFromArray(buffer.GetReadPointer(), "
                   "buffer.GetActiveSize())) {\n"
                   "        LOG_DEBUG(\"{} Failed to parse request for "
                   "$full_name$ server method call.\", GetCallerInfo());\n"
                   "        SendResponse(service_hash_, method_id, token, "
                   "kErrorCodeMalformedRequest);\n"
                   "        return;\n"
                   "      }\n");

    if ("NoResponse" != method->output_type()->name()) {
      printer->Print(
          sub_vars,
          "      LOG_DEBUG(\"{} Client called server method "
          "$full_name$($input_type_name${{ {} }}).\", GetCallerInfo(), "
          "request.ShortDebugString());\n"
          "      std::function<void(ServiceBase*, ::tpn::protocol::ErrorCode, "
          "const ::google::protobuf::Message *)> continuation = [token, "
          "method_id](ServiceBase *service, ::tpn::protocol::ErrorCode status, "
          "const ::google::protobuf::Message *response) {\n"
          "        TPN_ASSERT(response->GetDescriptor() == "
          "$output_type$::descriptor(), \"response descriptor error {} != "
          "{}\", response->GetDescriptor()->DebugString(), "
          "$output_type$::descriptor()->DebugString());\n"
          "        $classname$* self = static_cast<$classname$*>(service);\n"
          "        LOG_DEBUG(\"{} Client called server method $full_name$() "
          "returned $output_type_name${{ {} }} status {}.\", "
          "self->GetCallerInfo(), response->ShortDebugString(), status);\n"
          "        if (kErrorCodeOk == status)\n"
          "          self->SendResponse(self->service_hash_, method_id, token, "
          "response);\n"
          "        else\n"
          "          self->SendResponse(self->service_hash_, method_id, token, "
          "status);\n"
          "      };\n"
          "      $output_type$ response;\n"
          "      ::tpn::protocol::ErrorCode status = Handle$name$(&request, "
          "&response, continuation);\n"
          "      if (continuation)\n"
          "        continuation(this, status, &response);\n");
    } else {
      printer->Print(
          sub_vars,
          "      ::tpn::protocol::ErrorCode status = Handle$name$(&request);\n"
          "      LOG_DEBUG(\"{} Client called server "
          "method $full_name$($input_type_name${{ {} }}) status {}.\", "
          "GetCallerInfo(), request.ShortDebugString(), status);\n"
          "      if (kErrorCodeOk != status)\n"
          "        SendResponse(service_hash_, method_id, token, status);\n");
    }

    printer->Print(sub_vars,
                   "      break;\n"
                   "    }\n");
  }

  printer->Print(vars_,
                 "    default:\n"
                 "      LOG_ERROR(\"Bad method id {}.\", method_id);\n"
                 "      SendResponse(service_hash_, method_id, token, "
                 "kErrorCodeInvalidMethod);\n"
                 "      break;\n"
                 "    }\n"
                 "}\n"
                 "\n");
}

void ServiceGenerator::GenerateServerImplementations(io::Printer *printer) {
  for (int i = 0; i < descriptor_->method_count(); i++) {
    const MethodDescriptor *method = descriptor_->method(i);
    if (!method->options().HasExtension(tpn::protocol::method_options)) {
      continue;
    }

    std::map<std::string, std::string> sub_vars;
    sub_vars["classname"]   = vars_["classname"];
    sub_vars["name"]        = method->name();
    sub_vars["full_name"]   = descriptor_->name() + "." + method->name();
    sub_vars["input_type"]  = ClassName(method->input_type(), true);
    sub_vars["output_type"] = ClassName(method->output_type(), true);

    if ("NoResponse" != method->output_type()->name()) {
      printer->Print(
          sub_vars,
          "::tpn::protocol::ErrorCode $classname$::Handle$name$(const "
          "$input_type$ *request, "
          "$output_type$ *response, std::function<void(ServiceBase*, "
          "::tpn::protocol::ErrorCode, "
          "const ::google::protobuf::Message *)>& continuation) {\n"
          "  LOG_ERROR(\"{} Client tried to call not implemented method "
          "$full_name$({{ {} }})\", GetCallerInfo(), "
          "request->ShortDebugString());\n"
          "  return kErrorCodeNotImplemented;\n"
          "}\n"
          "\n");
    } else {
      printer->Print(
          sub_vars,
          "::tpn::protocol::ErrorCode $classname$::Handle$name$(const "
          "$input_type$ *request) {\n"
          "  LOG_ERROR(\"{} Client tried to call not implemented method "
          "$full_name$({{ {} }})\", GetCallerInfo(), "
          "request->ShortDebugString());\n"
          "  return kErrorCodeNotImplemented;\n"
          "}\n"
          "\n");
    }
  }
}

std::string ServiceGenerator::HashToHex(uint64_t num) {
  if (0 == num) {
    return std::string("0");
  }

  char buf[16];
  char *bufptr = buf + 16;

  static const char kHexChars[] = "0123456789abcdef";
  while (0 != num) {
    *--bufptr = kHexChars[num & 0xf];
    num >>= 4;
  }

  return std::string(bufptr, buf + 16 - bufptr);
}

std::uint32_t ServiceGenerator::HashServiceName(std::string const &name) {
  std::uint32_t hash = 0x811C9DC5;

  for (auto &&ch : name) {
    hash ^= static_cast<uint32_t>(ch);
    hash *= 0x1000193;
  }

  return hash;
}

}  // namespace cpp
}  // namespace compiler
}  // namespace protobuf
}  // namespace google
