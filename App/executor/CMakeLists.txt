# ==============================================================================
# App/executor/CMakeLists.txt for Container Manager
#
# This CMake configuration builds the cm_executor static library, which provides
# request executor implementations for different data formats (JSON, Protobuf).
# Source files and dependencies are included conditionally based on the enabled
# data format flags, so you only need to install and build what you require.
#
# Usage:
#   - To enable/disable data formats, set the ENABLE_* flags in the top-level CMake.
#   - Only the required source files and libraries for enabled formats are included.
#   - Example: To build only with JSON, use -DENABLE_PROTOBUF=OFF.
# ==============================================================================

set(APP_NAME cm_executor)

set(SRC_LIST src/json_request_executor.cpp)

if(ENABLE_PROTOBUF)
    list(APPEND SRC_LIST src/protobuf_request_executor.cpp)
endif()

add_library(${APP_NAME} STATIC ${SRC_LIST})

target_include_directories(${APP_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/generated
)

target_link_libraries(${APP_NAME} PUBLIC glog::glog cm_core cm_database cm_security)

if(ENABLE_PROTOBUF)
    target_link_libraries(${APP_NAME} PUBLIC protobuf)
endif()