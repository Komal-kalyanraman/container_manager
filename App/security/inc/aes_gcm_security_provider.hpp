#pragma once

#include "inc/security_provider.hpp"
#include <string>
#include <vector>

/**
 * @brief AES-256-GCM implementation of ISecurityProvider
 */
class AesGcmSecurityProvider : public ISecurityProvider {
public:
    AesGcmSecurityProvider() = default;
    ~AesGcmSecurityProvider() override = default;
    
    bool Decrypt(const std::string& input, std::string& output) override;
};