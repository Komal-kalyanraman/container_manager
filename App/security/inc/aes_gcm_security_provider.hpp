#pragma once

#include "inc/security_provider.hpp"
#include <string>
#include <vector>

#include "inc/common.hpp"

/**
 * @brief AES-256-GCM implementation for secure OTA payloads
 * Industry standard encryption with authenticated encryption
 */
class AesGcmSecurityProvider : public ISecurityProvider {
public:
    AesGcmSecurityProvider() = default;
    ~AesGcmSecurityProvider() override = default;
    
    bool Decrypt(const std::string& input, std::string& output) override;
    
private:
    std::vector<unsigned char> LoadAesKey(const std::string& path = kAesKeyFilePath);
    bool DecryptAesGcm(const std::string& encrypted_data, std::string& decrypted_data);
};