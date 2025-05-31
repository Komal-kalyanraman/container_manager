#pragma once

#include "inc/security_provider.hpp"

/**
 * @brief Null object pattern implementation - no-op security provider
 * Used when encryption is disabled
 */
class NullSecurityProvider : public ISecurityProvider {
public:
    bool Decrypt(const std::string& input, std::string& output) override {
        output = input;  // No decryption, just pass through
        return true;
    }
};