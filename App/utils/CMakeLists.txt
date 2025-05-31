# Set the library name for the utils module
set(APP_NAME cm_utils)

# Add the static library and its source files
add_library(${APP_NAME} STATIC
    src/threadpool.cpp
    src/init_handler.cpp
    src/curl_handler.cpp
    src/aes_gcm_decrypt.cpp
)

# Find and link the CURL library
find_package(CURL REQUIRED)
target_link_libraries(${APP_NAME} PUBLIC CURL::libcurl)

# Link glog and cpp_redis libraries to the utils module
target_link_libraries(cm_utils PUBLIC glog::glog cpp_redis)

# If encryption is enabled, find and link OpenSSL for AES-GCM support
if(ENABLE_ENCRYPTION)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${APP_NAME} PUBLIC OpenSSL::Crypto)
endif()