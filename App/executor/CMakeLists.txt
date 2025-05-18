set(APP_NAME cm_executor)

add_library(${APP_NAME} STATIC
    src/json_request_executor.cpp
    src/protobuf_request_executor.cpp
)

target_include_directories(${APP_NAME} PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/proto/generated
)

target_link_libraries(cm_executor PUBLIC glog::glog cm_core cm_database protobuf)