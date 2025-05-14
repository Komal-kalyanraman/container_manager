set(APP_NAME cm_common)

add_library(${APP_NAME} STATIC
    src/cm_database_handler.cpp
    src/cm_threadpool.cpp
)

target_link_libraries(cm_common PUBLIC glog::glog)