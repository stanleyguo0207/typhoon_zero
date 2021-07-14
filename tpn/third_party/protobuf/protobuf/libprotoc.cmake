set(PROTOC_SOURCES_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/code_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/command_line_interface.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_enum.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_enum_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_extension.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_file.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_helpers.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_map_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_message_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_padding_optimizer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_parse_function_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_primitive_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_service.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_string_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/plugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/plugin.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/subprocess.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/zip_writer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/pgt_custom/pgt_custom_options.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/main.cc
  )

set(PROTOC_SOURCES_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_enum.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_enum_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_extension.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_file.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_helpers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_map_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_message_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_message_layout_helper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_names.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_options.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_padding_optimizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_parse_function_generator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_primitive_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_service.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/cpp/cpp_string_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/scc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/subprocess.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/zip_writer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/pgt_custom/pgt_custom_options.pb.h
  )

# exec protc
add_executable(protoc
  ${PROTOC_SOURCES_FILES}
  ${PROTOC_SOURCES_INCLUDES}
  )

target_include_directories(protoc
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf
  )

# link library
target_link_libraries(protoc
  protobuf
  )

# install
install(TARGETS protoc DESTINATION ${BIN_DIR})
