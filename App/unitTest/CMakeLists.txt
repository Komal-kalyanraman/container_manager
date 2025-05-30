cmake_minimum_required(VERSION 3.10)

project(cm_unit_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------- Dependencies ----------------
find_package(GTest REQUIRED)
find_package(Threads REQUIRED)
find_package(glog 0.4.0 REQUIRED)
include_directories(/usr/include/nlohmann)

# Inherit protocol and feature options from parent
option(ENABLE_REST      "Enable REST/HTTP server support" ON)
option(ENABLE_MQTT      "Enable MQTT subscriber support" ON)
option(ENABLE_MSGQUEUE  "Enable POSIX Message Queue support" ON)
option(ENABLE_DBUS      "Enable D-Bus consumer support" ON)
option(ENABLE_GRPC      "Enable gRPC protocol support" ON)
option(ENABLE_PROTOBUF  "Enable Protobuf data format support" ON)
option(ENABLE_REDIS     "Enable Redis database backend (OFF = use embedded DB)" OFF)

add_compile_definitions(
    ENABLE_REST=$<BOOL:${ENABLE_REST}>
    ENABLE_MQTT=$<BOOL:${ENABLE_MQTT}>
    ENABLE_MSGQUEUE=$<BOOL:${ENABLE_MSGQUEUE}>
    ENABLE_DBUS=$<BOOL:${ENABLE_DBUS}>
    ENABLE_GRPC=$<BOOL:${ENABLE_GRPC}>
    ENABLE_PROTOBUF=$<BOOL:${ENABLE_PROTOBUF}>
    ENABLE_REDIS=$<BOOL:${ENABLE_REDIS}>
)

# ---------------- Include Directories ----------------
include_directories(
    ${CMAKE_SOURCE_DIR}/api
    ${CMAKE_SOURCE_DIR}/core
    ${CMAKE_SOURCE_DIR}/utils
    ${CMAKE_SOURCE_DIR}/runtime
    ${CMAKE_SOURCE_DIR}/executor
    ${CMAKE_SOURCE_DIR}/database
    /usr/local/include
)

# Add additional include directories for dependencies if needed
if(ENABLE_MQTT)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
  pkg_check_modules(MOSQUITTOPP REQUIRED libmosquittopp)
  include_directories(${MOSQUITTO_INCLUDE_DIRS})
  include_directories(${MOSQUITTOPP_INCLUDE_DIRS})
endif()
if(ENABLE_DBUS)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDBUSCPP REQUIRED sdbus-c++)
  include_directories(${SDBUSCPP_INCLUDE_DIRS})
endif()
if(ENABLE_REDIS)
  set(HOME_DIR $ENV{HOME})
  include_directories("${HOME_DIR}/cpp_redis/includes")
  link_directories("${HOME_DIR}/cpp_redis/build")
endif()

# ---------------- Test Sources ----------------
file(GLOB TEST_SOURCES "*.cpp")

add_executable(runUnitTests ${TEST_SOURCES})

target_link_libraries(runUnitTests
    # GTest::GTest
    # GTest::Main
    gtest
    gtest_main
    gmock
    gmock_main
    Threads::Threads
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

if(ENABLE_REDIS)
  target_link_libraries(runUnitTests cpp_redis)
endif()
if(ENABLE_MQTT)
  target_link_libraries(runUnitTests ${MOSQUITTO_LIBRARIES} ${MOSQUITTOPP_LIBRARIES})
endif()
if(ENABLE_DBUS)
  target_link_libraries(runUnitTests ${SDBUSCPP_LIBRARIES})
endif()
if(ENABLE_PROTOBUF OR ENABLE_GRPC)
  target_link_libraries(runUnitTests protobuf container_manager_proto)
endif()
if(ENABLE_GRPC)
  target_link_libraries(runUnitTests grpc++ grpc)
endif()

enable_testing()
add_test(NAME AllUnitTests COMMAND runUnitTests)