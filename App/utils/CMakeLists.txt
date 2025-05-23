set(APP_NAME cm_utils)

add_library(${APP_NAME} STATIC
    src/threadpool.cpp
    src/init_handler.cpp
    src/curl_handler.cpp
)

find_package(CURL REQUIRED)
target_link_libraries(${APP_NAME} PUBLIC CURL::libcurl)

target_link_libraries(cm_utils PUBLIC glog::glog cpp_redis)