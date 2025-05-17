cmake_minimum_required(VERSION 3.10)
project(mco VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Warning flags
SET(WARNING_FLAG OFF)     # ON for showing the warning
if(WARNING_FLAG)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wformat-security")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wconversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wsign-conversion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wduplicated-cond")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfloat-equal")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wshadow")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wlogical-not-parentheses")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnull-dereference")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wextra")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_FORTIFY_SOURCE=2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,--exclude-libs,ALL")
endif()

set(HOME_DIR $ENV{HOME})
message(STATUS "Home Directory : ${HOME_DIR}")

find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(glog 0.4.0 REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
pkg_check_modules(MOSQUITTOPP REQUIRED libmosquittopp)

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

add_subdirectory(api)
add_subdirectory(core)
add_subdirectory(utils)
add_subdirectory(runtime)
add_subdirectory(executor)
add_subdirectory(database)

link_directories("${HOME_DIR}/cpp_redis/build")

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
)
