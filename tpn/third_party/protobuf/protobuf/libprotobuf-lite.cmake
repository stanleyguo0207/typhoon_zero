set(PROTOBUF_LITE_SOURCES_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/any_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/arena.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/arenastring.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/extension_set.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/field_access_listener.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_enum_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_table_driven_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_util.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/implicit_weak_message.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/coded_stream.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/io_win32.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/strtod.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream_impl.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream_impl_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/map.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/message_lite.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/parse_context.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/repeated_field.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/bytestream.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/common.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/int128.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/status.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/statusor.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/stringpiece.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/stringprintf.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/structurally_valid.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/strutil.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/time.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wire_format_lite.cc
  )

set(PROTOBUF_LITE_SOURCES_INCLUDES
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/arena.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/arenastring.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/extension_set.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/generated_message_util.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/implicit_weak_message.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/parse_context.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/coded_stream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/strtod.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream_impl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/io/zero_copy_stream_impl_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/message_lite.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/repeated_field.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/bytestream.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/common.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/int128.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/once.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/status.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/statusor.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/stringpiece.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/stringprintf.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/strutil.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/stubs/time.h
  ${CMAKE_CURRENT_SOURCE_DIR}/protobuf/google/protobuf/wire_format_lite.h
  )

add_library(protobuf-lite
  STATIC
    ${PROTOBUF_LITE_SOURCES_FILES}
    ${PROTOBUF_LITE_SOURCES_INCLUDES}
  )

target_include_directories(protobuf-lite
  PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/protobuf
  )

target_link_libraries(protobuf-lite
  PRIVATE
    typhoon-default-interface
    typhoon-no-warning-interface
  PUBLIC
    threads
    zlib
  )

target_compile_definitions(protobuf-lite
  PUBLIC
    -DHAVE_ZLIB
)

if(MSVC)
  target_compile_options(protobuf-lite
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

  target_compile_definitions(protobuf-lite
    PUBLIC
      -D_SCL_SECURE_NO_WARNINGS
    )
else()
  target_compile_definitions(protobuf-lite
    PUBLIC
      -DHAVE_PTHREAD
    )
endif()

set_target_properties(protobuf-lite
  PROPERTIES
    FOLDER
      "third_party"
  )
