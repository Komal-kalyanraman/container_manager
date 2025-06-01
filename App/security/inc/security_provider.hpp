/**
 * @file security_provider.hpp
 * @brief Declares the ISecurityProvider abstract interface for encryption/decryption.
 *
 * This interface allows pluggable security implementations (e.g., AES-GCM, ChaCha20, Null).
 * It is used by the executor layer to transparently decrypt incoming payloads,
 * regardless of the underlying encryption algorithm or whether encryption is enabled.
 */

#pragma once

#include <string>

/**
 * @class ISecurityProvider
 * @brief Abstract interface for security providers (encryption/decryption).
 *
 * All security providers must implement this interface to support decryption.
 * This enables the use of different algorithms or a null provider (no encryption)
 * without changing the rest of the codebase.
 */
class ISecurityProvider {
public:
    virtual ~ISecurityProvider() = default;
    
    /**
     * @brief Decrypt input data and store result in output.
     * @param input Encrypted input data.
     * @param output Decrypted output data.
     * @return true if decryption successful, false otherwise.
     */
    virtual bool Decrypt(const std::string& input, std::string& output) = 0;
};