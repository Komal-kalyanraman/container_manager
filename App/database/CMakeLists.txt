set(APP_NAME cm_database)

add_library(${APP_NAME} STATIC
    src/redis_database.cpp
)

target_link_libraries(cm_database PUBLIC glog::glog cpp_redis)