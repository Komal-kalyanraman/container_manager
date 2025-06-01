/**
 * @file null_security_provider.hpp
 * @brief Declares the NullSecurityProvider class for the "no encryption" case.
 *
 * This class implements the ISecurityProvider interface but performs no decryption.
 * It is used when encryption is disabled at build time (ENABLE_ENCRYPTION=OFF).
 * The null object pattern allows the rest of the codebase to remain agnostic to
 * whether encryption is enabled or not.
 */

#pragma once

#include "inc/security_provider.hpp"

/**
 * @class NullSecurityProvider
 * @brief Null object pattern implementation - no encryption/decryption.
 *
 * This class provides a no-op Decrypt method, always returning the input as output.
 * Used when encryption is disabled.
 */
class NullSecurityProvider : public ISecurityProvider {
public:
    NullSecurityProvider() = default;
    ~NullSecurityProvider() override = default;
    
    /**
     * @brief No-op decryption; simply returns the input as output.
     * @param input The input data (plain).
     * @param output The output data (same as input).
     * @return Always true.
     */
    bool Decrypt(const std::string& input, std::string& output) override;
};