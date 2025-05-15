set(APP_NAME cm_core)

add_library(${APP_NAME} STATIC
    src/container_service.cpp
)

target_link_libraries(cm_core PUBLIC glog::glog cm_runtime)