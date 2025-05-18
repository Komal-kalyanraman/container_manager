cmake_minimum_required(VERSION 3.10)
project(cm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wno-deprecated-declarations)

# find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
# if(CLANG_TIDY_EXE)
#   set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=*")
#   message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
# else()
#   message(WARNING "clang-tidy not found! Static analysis will be skipped.")
# endif()

set(HOME_DIR $ENV{HOME})
message(STATUS "Home Directory : ${HOME_DIR}")

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(Protobuf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
pkg_check_modules(MOSQUITTOPP REQUIRED libmosquittopp)
pkg_check_modules(SDBUSCPP REQUIRED sdbus-c++)

include_directories(${CURL_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_SOURCE_DIR}/third_party/httplib)
include_directories("${PROJECT_SOURCE_DIR}/api")
include_directories("${PROJECT_SOURCE_DIR}/core")
include_directories("${PROJECT_SOURCE_DIR}/utils")
include_directories("${PROJECT_SOURCE_DIR}/runtime")
include_directories("${PROJECT_SOURCE_DIR}/executor")
include_directories("${PROJECT_SOURCE_DIR}/database")
include_directories(/usr/local/include)
include_directories("${HOME_DIR}/cpp_redis/includes")
include_directories(${MOSQUITTO_INCLUDE_DIRS})
include_directories(${MOSQUITTOPP_INCLUDE_DIRS})
include_directories(${SDBUSCPP_INCLUDE_DIRS})

add_subdirectory(api)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(runtime)
add_subdirectory(executor)
add_subdirectory(database)

link_directories("${HOME_DIR}/cpp_redis/build")

# --------- Protobuf/gRPC code generation ---------
set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/api/proto")
set(PROTO_FILES "${PROTO_SRC_DIR}/container_manager.proto")

set(GENERATED_PROTO_DIR "${CMAKE_CURRENT_BINARY_DIR}/generated")
file(MAKE_DIRECTORY ${GENERATED_PROTO_DIR})

add_custom_command(
  OUTPUT
    ${GENERATED_PROTO_DIR}/container_manager.pb.cc
    ${GENERATED_PROTO_DIR}/container_manager.pb.h
    ${GENERATED_PROTO_DIR}/container_manager.grpc.pb.cc
    ${GENERATED_PROTO_DIR}/container_manager.grpc.pb.h
  COMMAND protoc
    --proto_path=${PROTO_SRC_DIR}
    --cpp_out=${GENERATED_PROTO_DIR}
    --grpc_out=${GENERATED_PROTO_DIR}
    --plugin=protoc-gen-grpc=`which grpc_cpp_plugin`
    ${PROTO_FILES}
  DEPENDS ${PROTO_FILES}
)

set(PROTO_SRCS
  ${GENERATED_PROTO_DIR}/container_manager.pb.cc
  ${GENERATED_PROTO_DIR}/container_manager.grpc.pb.cc
)
set(PROTO_HDRS
  ${GENERATED_PROTO_DIR}/container_manager.pb.h
  ${GENERATED_PROTO_DIR}/container_manager.grpc.pb.h
)

include_directories(${GENERATED_PROTO_DIR})

add_library(container_manager_proto
  ${PROTO_SRCS}
  ${PROTO_HDRS}
)

####### GTests ########
SET(GTEST OFF)     # Set ON for building the tests, OFF for not
if(GTEST)
  add_subdirectory(unitTest)
endif()

add_executable(CM
  main.cpp
)

target_link_libraries(CM 
  ${CMAKE_THREAD_LIBS_INIT}
  ${MOSQUITTO_LIBRARIES}
  ${MOSQUITTOPP_LIBRARIES}
  glog::glog
  cm_core
  cm_utils
  cm_database
  cm_runtime
  cm_api
  cm_executor
  librt.so
  pthread
  cpp_redis
  ${SDBUSCPP_LIBRARIES} 
  grpc++
  grpc
  protobuf
  container_manager_proto
)