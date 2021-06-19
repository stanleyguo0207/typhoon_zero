set(PROTOBUF_SOURCES_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/any.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/any.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/api.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/importer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/parser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor_database.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/duration.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/dynamic_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/empty.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/extension_set_heavy.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/field_mask.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_reflection.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_table_driven.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/gzip_stream.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/printer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/tokenizer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/map_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/reflection_ops.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/service.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/source_context.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/struct.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/substitute.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/text_format.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/timestamp.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/type.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/unknown_field_set.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/delimited_message_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/field_comparator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/field_mask_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/datapiece.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/default_value_objectwriter.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/error_listener.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/field_mask_utility.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_escaping.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_objectwriter.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_stream_parser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/object_writer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/proto_writer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/protostream_objectsource.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/protostream_objectwriter.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/type_info.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/type_info_test_helper.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/utility.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/json_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/message_differencer.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/time_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/type_resolver_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wire_format.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wrappers.pb.cc
  )

set(PROTOBUF_SOURCES_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/any.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/any.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/api.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/importer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/parser.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/descriptor_database.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/duration.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/dynamic_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/empty.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/field_access_listener.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/field_mask.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_reflection.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/gzip_stream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/printer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/tokenizer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/map_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/reflection_ops.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/service.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/source_context.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/struct.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/substitute.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/text_format.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/timestamp.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/type.pb.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/unknown_field_set.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/delimited_message_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/field_comparator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/field_mask_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/datapiece.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/default_value_objectwriter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/error_listener.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/field_mask_utility.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_escaping.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_objectwriter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/json_stream_parser.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/object_writer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/proto_writer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/protostream_objectsource.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/protostream_objectwriter.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/type_info.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/type_info_test_helper.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/internal/utility.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/json_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/message_differencer.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/time_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/util/type_resolver_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wire_format.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wrappers.pb.h
  )

add_library(protobuf
  STATIC
    ${PROTOBUF_LITE_SOURCES_FILES}
    ${PROTOBUF_SOURCES_FILES}
    ${PROTOBUF_SOURCES_INCLUDES}
  )

target_include_directories(protobuf
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf
  )

target_link_libraries(protobuf
  PRIVATE
    typhoon-default-interface
    typhoon-no-warning-interface
  PUBLIC
    threads
    zlib
  )

target_compile_definitions(protobuf
  PUBLIC
    -DHAVE_ZLIB
)

if(MSVC)
  target_compile_options(protobuf
    INTERFACE
      /wd4018 # 'expression' : signed/unsigned mismatch
      /wd4065 # switch statement contains 'default' but no 'case' labels
      /wd4146 # unary minus operator applied to unsigned type, result still unsigned
      /wd4244 # 'conversion' conversion from 'type1' to 'type2', possible loss of data
      /wd4251 # 'identifier' : class 'type' needs to have dll-interface to be used by clients of class 'type2'
      /wd4267 # 'var' : conversion from 'size_t' to 'type', possible loss of data
      /wd4305 # 'identifier' : truncation from 'type1' to 'type2'
      /wd4307 # 'operator' : integral constant overflow
      /wd4309 # 'conversion' : truncation of constant value
      /wd4334 # 'operator' : result of 32-bit shift implicitly converted to 64 bits (was 64-bit shift intended?)
      /wd4355 # 'this' : used in base member initializer list
      /wd4506 # no definition for inline function 'function'
      /wd4800 # 'type' : forcing value to bool 'true' or 'false' (performance warning)
      /wd4996 # The compiler encountered a deprecated declaration.
    )

  target_compile_definitions(protobuf
    INTERFACE
      -D_SCL_SECURE_NO_WARNINGS
    )
endif()

set_target_properties(protobuf
  PROPERTIES
    FOLDER
      "third_party"
  )
