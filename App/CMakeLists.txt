# Minimum required CMake version and project definition
cmake_minimum_required(VERSION 3.10)
project(cm VERSION 0.7.0 LANGUAGES CXX)

# Set C++ standard and compiler options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wno-deprecated-declarations)

# Feature flags for protocols, data formats, database, and encryption
option(ENABLE_REST      "Enable REST/HTTP server support" ON)
option(ENABLE_MQTT      "Enable MQTT subscriber support" ON)
option(ENABLE_MSGQUEUE  "Enable POSIX Message Queue support" ON)
option(ENABLE_DBUS      "Enable D-Bus consumer support" ON)
option(ENABLE_GRPC      "Enable gRPC protocol support" ON)
option(ENABLE_PROTOBUF  "Enable Protocol Buffers for efficient binary serialization" ON)
option(ENABLE_REDIS     "Enable Redis database backend (OFF = use embedded DB)" OFF)
option(ENABLE_ENCRYPTION "Enable AES-256-GCM encryption for secure OTA payloads" ON)

# Pass feature flags as compile definitions
add_compile_definitions(
    ENABLE_REST=$<BOOL:${ENABLE_REST}>
    ENABLE_MQTT=$<BOOL:${ENABLE_MQTT}>
    ENABLE_MSGQUEUE=$<BOOL:${ENABLE_MSGQUEUE}>
    ENABLE_DBUS=$<BOOL:${ENABLE_DBUS}>
    ENABLE_GRPC=$<BOOL:${ENABLE_GRPC}>
    ENABLE_PROTOBUF=$<BOOL:${ENABLE_PROTOBUF}>
    ENABLE_REDIS=$<BOOL:${ENABLE_REDIS}>
    ENABLE_ENCRYPTION=$<BOOL:${ENABLE_ENCRYPTION}>
)

# Set home directory and print status
set(HOME_DIR $ENV{HOME})
message(STATUS "Container Manager v0.7.0 - Home Directory: ${HOME_DIR}")

# Find required packages and libraries
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(nlohmann_json REQUIRED)

# Find Protobuf if enabled
if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  find_package(Protobuf REQUIRED)
endif()

# Find MQTT libraries if enabled
if(ENABLE_MQTT)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
  pkg_check_modules(MOSQUITTOPP REQUIRED libmosquittopp)
endif()

# Find D-Bus libraries if enabled
if(ENABLE_DBUS)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDBUSCPP REQUIRED sdbus-c++)
endif()

# Add Redis include and link directories if enabled
if(ENABLE_REDIS)
  include_directories("${HOME_DIR}/cpp_redis/includes")
  link_directories("${HOME_DIR}/cpp_redis/build")
endif()

# Find OpenSSL for encryption if enabled
if(ENABLE_ENCRYPTION)
  find_package(OpenSSL REQUIRED)
  add_compile_definitions(ENABLE_ENCRYPTION=1)
endif()

# Include directories for all modules and dependencies
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

# Include MQTT and D-Bus directories if enabled
if(ENABLE_MQTT)
  include_directories(${MOSQUITTO_INCLUDE_DIRS})
  include_directories(${MOSQUITTOPP_INCLUDE_DIRS})
endif()

if(ENABLE_DBUS)
  include_directories(${SDBUSCPP_INCLUDE_DIRS})
endif()

# Add subdirectories for each module
add_subdirectory(api)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(runtime)
add_subdirectory(executor)
add_subdirectory(database)

# Generate Protobuf sources if enabled
if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  set(PROTO_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/executor/proto")
  set(PROTO_FILES "${PROTO_SRC_DIR}/container_manager.proto")
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
    COMMENT "Generating Protocol Buffers C++ code for Container Manager"
  )
  set(PROTO_SRCS ${GENERATED_PROTO_DIR}/container_manager.pb.cc)
  set(PROTO_HDRS ${GENERATED_PROTO_DIR}/container_manager.pb.h)
  include_directories(${GENERATED_PROTO_DIR})
  add_library(container_manager_proto ${PROTO_SRCS} ${PROTO_HDRS})
endif()

# Enable Google Test unit testing framework if requested
set(GTEST ON CACHE BOOL "Enable Google Test unit testing framework")
if(GTEST)
  add_subdirectory(unitTest)
endif()

# Define the main executable
add_executable(CM main.cpp)

# Link all required libraries to the main executable
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
  CURL::libcurl
)

# Conditionally link protocol/database/encryption libraries
if(ENABLE_REDIS)
  target_link_libraries(CM cpp_redis)
endif()

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