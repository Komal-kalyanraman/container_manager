#include "inc/aes_gcm_security_provider.hpp"
#include "inc/aes_gcm_decrypt.hpp"

bool AesGcmSecurityProvider::Decrypt(const std::string& input, std::string& output) {
    return decrypt_ota_payload(input, output);
}