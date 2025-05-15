set(APP_NAME cm_api)

add_library(${APP_NAME} STATIC
    src/http_server.cpp
    # src/messagequeue_consumer.cpp
)

target_link_libraries(cm_api PUBLIC glog::glog)