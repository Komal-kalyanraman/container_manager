set(APP_NAME cm_dataprocessing)

add_library(${APP_NAME} STATIC
    src/cm_json_requestexecutor_handler.cpp
    # src/cm_proto_requestexecutor_handler.cpp
)

target_link_libraries(cm_dataprocessing PUBLIC glog::glog cm_bl)