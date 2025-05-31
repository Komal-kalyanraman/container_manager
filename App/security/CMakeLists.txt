# Set the library name for the utils module
set(APP_NAME cm_security)

# Add the static library and its source files
add_library(${APP_NAME} STATIC
    src/aes_gcm_decrypt.cpp
    src/aes_gcm_security_provider.cpp
)

# If encryption is enabled, find and link OpenSSL for AES-GCM support
if(ENABLE_ENCRYPTION)
    find_package(OpenSSL REQUIRED)
    target_link_libraries(${APP_NAME} PUBLIC OpenSSL::Crypto)
endif()