set(APP_NAME cm_executor)

add_library(${APP_NAME} STATIC
    src/json_request_executor.cpp
    # src/cm_proto_requestexecutor_handler.cpp
)

target_link_libraries(cm_executor PUBLIC glog::glog cm_core cm_database)