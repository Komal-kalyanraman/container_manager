set(APP_NAME cm_bl)

add_library(${APP_NAME} STATIC
    src/cm_container_service_handler.cpp
)

target_link_libraries(cm_bl PUBLIC glog::glog)