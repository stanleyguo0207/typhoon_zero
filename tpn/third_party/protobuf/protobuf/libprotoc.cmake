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
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_doc_comment.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_enum.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_enum_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_field_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_helpers.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_map_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_message_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_primitive_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_reflection_class.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_enum_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_message_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_primitive_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_source_generator_base.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_wrapper_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_context.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_doc_comment.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_field_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_extension.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_extension_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_file.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_generator_factory.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_helpers.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_kotlin_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_map_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_map_field_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_builder.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_builder_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_field_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_name_resolver.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_primitive_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_primitive_field_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_service.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_shared_code_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_string_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_string_field_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/js/js_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/js/well_known_types_embed.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_enum.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_enum_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_extension.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_file.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_helpers.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_map_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_message_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_oneof.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_primitive_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/php/php_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/plugin.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/plugin.pb.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/python/python_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/ruby/ruby_generator.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/subprocess.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/zip_writer.cc
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
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_doc_comment.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_enum.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_enum_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_field_base.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_helpers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_map_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_message_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_options.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_primitive_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_reflection_class.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_enum_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_message_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_repeated_primitive_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_source_generator_base.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/csharp/csharp_wrapper_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_doc_comment.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_field_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_enum_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_extension.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_extension_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_file.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_generator_factory.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_helpers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_map_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_map_field_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_builder.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_builder_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_field_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_message_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_name_resolver.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_options.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_primitive_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_primitive_field_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_service.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_shared_code_generator.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_string_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/java/java_string_field_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_enum.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_enum_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_extension.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_file.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_helpers.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_map_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_message_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_nsobject_methods.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_oneof.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/objectivec/objectivec_primitive_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/scc.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/subprocess.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/compiler/zip_writer.h
  )

add_library(protogen
  STATIC
    ${PROTOC_SOURCES_FILES}
    ${PROTOC_SOURCES_INCLUDES}
  )

target_include_directories(protogen
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf
  )

target_link_libraries(protogen
  PRIVATE
    typhoon-default-interface
    typhoon-no-warning-interface
  PUBLIC
    threads
    zlib
    protobuf
  )

target_compile_definitions(protogen
  PUBLIC
    -DHAVE_ZLIB
)

if(MSVC)
  target_compile_options(protogen
    PUBLIC
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

  target_compile_definitions(protogen
    PUBLIC
      -D_SCL_SECURE_NO_WARNINGS
    )
else()
  target_compile_definitions(protogen
    PUBLIC
      -DHAVE_PTHREAD
    )
endif()

set_target_properties(protogen
  PROPERTIES
    FOLDER
      "third_party"
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
