#pragma once

#include "inc/security_provider.hpp"

/**
 * @brief Null object pattern implementation - no encryption/decryption
 */
class NullSecurityProvider : public ISecurityProvider {
public:
    NullSecurityProvider() = default;
    ~NullSecurityProvider() override = default;
    
    bool Decrypt(const std::string& input, std::string& output) override;
};