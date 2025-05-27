set(APP_NAME cm_database)

if(ENABLE_REDIS)
    add_library(${APP_NAME} STATIC
        src/redis_database.cpp
        src/embedded_database.cpp
    )
    target_link_libraries(${APP_NAME} PUBLIC glog::glog cpp_redis)
else()
    add_library(${APP_NAME} STATIC
        src/embedded_database.cpp
    )
    target_link_libraries(${APP_NAME} PUBLIC glog::glog)
endif()