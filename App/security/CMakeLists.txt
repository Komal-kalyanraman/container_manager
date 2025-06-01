# CMake build script for the security providers library (cm_security).
# This script selects and builds the appropriate security provider(s)
# based on the ENABLE_ENCRYPTION and SECURITY_ALGORITHM CMake flags.

set(APP_NAME cm_security)

# Base source files list (will be populated below)
set(SRC_LIST)

# Add algorithm-specific sources based on CMake configuration
if(ENABLE_ENCRYPTION)
    # If encryption is enabled, select the implementation based on SECURITY_ALGORITHM
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
    
    # Build the static library with the selected sources
    add_library(${APP_NAME} STATIC ${SRC_LIST})
    # Link against OpenSSL for cryptographic operations
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${APP_NAME} PUBLIC OpenSSL::Crypto)
    # Link against glog for logging
    target_link_libraries(${APP_NAME} PUBLIC glog::glog)
else()
    # When encryption is disabled, only build the null provider
    list(APPEND SRC_LIST src/null_security_provider.cpp)
    add_library(${APP_NAME} STATIC ${SRC_LIST})
    target_link_libraries(${APP_NAME} PUBLIC glog::glog)
    message(STATUS "Security: Building with encryption disabled")
endif()