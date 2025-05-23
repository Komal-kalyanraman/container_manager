# ==============================================================================
# App/CMakeLists.txt for Container Manager
#
# This is the main build configuration for the Container Manager project.
# It supports multiple protocols (REST, MQTT, Message Queue, D-Bus, gRPC) and
# multiple data formats (JSON, Protobuf). By default, all protocols and formats
# are enabled, but you can selectively enable/disable features to minimize
# dependencies and build only what you need.
#
# To customize, set the following options before running CMake:
#   -DENABLE_REST=ON/OFF
#   -DENABLE_MQTT=ON/OFF
#   -DENABLE_MSGQUEUE=ON/OFF
#   -DENABLE_DBUS=ON/OFF
#   -DENABLE_GRPC=ON/OFF
#   -DENABLE_PROTOBUF=ON/OFF
#
# Example:
#   cmake .. -DENABLE_REST=OFF -DENABLE_MQTT=OFF -DENABLE_PROTOBUF=OFF
#
# By default, all options are ON for a full-featured build.
# ==============================================================================

cmake_minimum_required(VERSION 3.10)
project(cm VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wno-deprecated-declarations)

# ---------------- Protocol Feature Options ----------------
option(ENABLE_REST      "Enable REST/HTTP server support" ON)
option(ENABLE_MQTT      "Enable MQTT subscriber support" ON)
option(ENABLE_MSGQUEUE  "Enable POSIX Message Queue support" ON)
option(ENABLE_DBUS      "Enable D-Bus consumer support" ON)
option(ENABLE_GRPC      "Enable gRPC protocol support" ON)

add_compile_definitions(
    ENABLE_REST=$<BOOL:${ENABLE_REST}>
    ENABLE_MQTT=$<BOOL:${ENABLE_MQTT}>
    ENABLE_MSGQUEUE=$<BOOL:${ENABLE_MSGQUEUE}>
    ENABLE_DBUS=$<BOOL:${ENABLE_DBUS}>
    ENABLE_GRPC=$<BOOL:${ENABLE_GRPC}>
    ENABLE_PROTOBUF=$<BOOL:${ENABLE_PROTOBUF}>
)

# ---------------- Data Format Feature Options ----------------
option(ENABLE_PROTOBUF  "Enable Protobuf data format support" ON)

set(HOME_DIR $ENV{HOME})
message(STATUS "Home Directory : ${HOME_DIR}")

# ---------------- Dependencies ----------------
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(nlohmann_json REQUIRED)

if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  find_package(Protobuf REQUIRED)
endif()

if(ENABLE_MQTT)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
  pkg_check_modules(MOSQUITTOPP REQUIRED libmosquittopp)
endif()

if(ENABLE_DBUS)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDBUSCPP REQUIRED sdbus-c++)
endif()

# ---------------- Include Directories ----------------
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

if(ENABLE_MQTT)
  include_directories(${MOSQUITTO_INCLUDE_DIRS})
  include_directories(${MOSQUITTOPP_INCLUDE_DIRS})
endif()
if(ENABLE_DBUS)
  include_directories(${SDBUSCPP_INCLUDE_DIRS})
endif()

add_subdirectory(api)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(runtime)
add_subdirectory(executor)
add_subdirectory(database)

link_directories("${HOME_DIR}/cpp_redis/build")

# --------- Protobuf/gRPC code generation ---------
if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/executor/proto")
  set(PROTO_FILES "${PROTO_SRC_DIR}/container_manager.proto")

  # Place generated files inside the proto folder for better organization
  set(GENERATED_PROTO_DIR "${PROTO_SRC_DIR}/generated")
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
  )
  set(PROTO_HDRS
    ${GENERATED_PROTO_DIR}/container_manager.pb.h
  )

  include_directories(${GENERATED_PROTO_DIR})

  add_library(container_manager_proto
    ${PROTO_SRCS}
    ${PROTO_HDRS}
  )
endif()

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
  CURL::libcurl
)

if(ENABLE_MQTT)
  target_link_libraries(CM ${MOSQUITTO_LIBRARIES} ${MOSQUITTOPP_LIBRARIES})
endif()
if(ENABLE_DBUS)
  target_link_libraries(CM ${SDBUSCPP_LIBRARIES})
endif()
if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  target_link_libraries(CM protobuf container_manager_proto)
endif()
if(ENABLE_GRPC)
  target_link_libraries(CM grpc++ grpc)
endif()

# ---------------- Notes ----------------
# - To build only with the protocols and formats you need, set the ENABLE_* options accordingly.
# - If you only need D-Bus with JSON, you can disable REST, MQTT, MessageQueue, Protobuf, and gRPC.
# - This reduces required dependencies and build time.
# - All protocol and format-specific code should be guarded with the corresponding ENABLE_* option in your source files.