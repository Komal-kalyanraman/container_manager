set(APP_NAME cm_security)

# Base source files
set(SRC_LIST)

# Add algorithm-specific sources based on CMake configuration
if(ENABLE_ENCRYPTION)
    if(SECURITY_ALGORITHM STREQUAL "AES_GCM")
        list(APPEND SRC_LIST 
            src/aes_gcm_security_provider.cpp
        )
        message(STATUS "Security: Building with AES-256-GCM support")
    elseif(SECURITY_ALGORITHM STREQUAL "CHACHA20")
        list(APPEND SRC_LIST 
            src/chacha20_security_provider.cpp
        )
        message(STATUS "Security: Building with ChaCha20-Poly1305 support")
    endif()
    
    add_library(${APP_NAME} STATIC ${SRC_LIST})
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${APP_NAME} PUBLIC OpenSSL::Crypto)
    target_link_libraries(${APP_NAME} PUBLIC glog::glog)
else()
    # When encryption is disabled, only build null provider
    list(APPEND SRC_LIST src/null_security_provider.cpp)
    add_library(${APP_NAME} STATIC ${SRC_LIST})
    target_link_libraries(${APP_NAME} PUBLIC glog::glog)
    message(STATUS "Security: Building with encryption disabled")
endif()