cmake_minimum_required(VERSION 3.22.1)

project(server C CXX)

set(CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/third_party/grpc")

set(OPENSSL_ROOT_DIR "${CMAKE_SOURCE_DIR}/third_party/openssl")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libcrypto.so")
set(OPENSSL_SSL_LIBRARY "${OPENSSL_ROOT_DIR}/lib64/libssl.so")
find_package(OpenSSL REQUIRED)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Threads REQUIRED)

#Find Protobuf installation
#Looks for protobuf-config.cmake file installed by Protobuf's cmake installation.
option(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf CONFIG REQUIRED)
message(STATUS "Using protobuf ${Protobuf_VERSION}")

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
set(_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

#Find gRPC installation
#Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(gRPC CONFIG REQUIRED)
message(STATUS "Using gRPC ${gRPC_VERSION}")

set(_GRPC_GRPCPP gRPC::grpc++)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

# Proto files
get_filename_component(urpcs_proto "${CMAKE_SOURCE_DIR}/src/protos/unary_rpc_service.proto" ABSOLUTE)
get_filename_component(urpcs_proto_path "${urpcs_proto}" PATH)

# Generated sources
set(urpcs_proto_srcs "${CMAKE_CURRENT_BINARY_DIR}/unary_rpc_service.pb.cc")
set(urpcs_proto_hdrs "${CMAKE_CURRENT_BINARY_DIR}/unary_rpc_service.pb.h")
set(urpcs_grpc_srcs "${CMAKE_CURRENT_BINARY_DIR}/unary_rpc_service.grpc.pb.cc")
set(urpcs_grpc_hdrs "${CMAKE_CURRENT_BINARY_DIR}/unary_rpc_service.grpc.pb.h")
add_custom_command(
    OUTPUT "${urpcs_proto_srcs}" "${urpcs_proto_hdrs}" "${urpcs_grpc_srcs}" "${urpcs_grpc_hdrs}"
    COMMAND ${_PROTOBUF_PROTOC}
    ARGS --grpc_out "${CMAKE_CURRENT_BINARY_DIR}"
        --cpp_out "${CMAKE_CURRENT_BINARY_DIR}"
        -I "${urpcs_proto_path}"
        --plugin=protoc-gen-grpc="${_GRPC_CPP_PLUGIN_EXECUTABLE}"
        "${urpcs_proto}"
    DEPENDS "${urpcs_proto}")

# Include generated *.pb.h file
include_directories("${CMAKE_CURRENT_BINARY_DIR}")

# urpcs_grpc_proto
add_library(urpcs_grpc_proto
    ${urpcs_grpc_srcs}
    ${urpcs_grpc_hdrs}
    ${urpcs_proto_srcs}
    ${urpcs_proto_hdrs})
target_link_libraries(urpcs_grpc_proto
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

# Target server
add_executable(server
    src/server.cc)

set_target_properties(server PROPERTIES
    BUILD_RPATH "$ORIGIN/../../third_party/grpc/lib"
)

add_library(helper
    src/helper/helper.h
    src/helper/helper.cc)

target_link_libraries(server
    helper
    urpcs_grpc_proto
    absl::flags
    absl::flags_parse
    absl::log_initialize
    absl::strings
    ${_REFLECTION}
    ${_GRPC_GRPCPP}
    ${_PROTOBUF_LIBPROTOBUF})

