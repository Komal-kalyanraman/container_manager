set(APP_NAME cm_communication)

add_library(${APP_NAME} STATIC
    src/cm_http_server_handler.cpp
    # src/cm_messagequeue_consumer_handler.cpp
)

target_link_libraries(cm_communication PUBLIC glog::glog)