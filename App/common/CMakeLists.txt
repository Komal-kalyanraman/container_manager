set(APP_NAME cm_common)

add_library(${APP_NAME} STATIC
    src/cm_threadpool.cpp
    src/cm_redis_database_handler.cpp
)

target_link_libraries(cm_common PUBLIC glog::glog cpp_redis)