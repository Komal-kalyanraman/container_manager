set(APP_NAME cm_utils)

add_library(${APP_NAME} STATIC
    src/threadpool.cpp
)

target_link_libraries(cm_utils PUBLIC glog::glog cpp_redis)