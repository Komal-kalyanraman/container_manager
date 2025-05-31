#pragma once

#include <string>

/**
 * @brief Abstract interface for security providers (encryption/decryption)
 * Allows pluggable security implementations (AES-GCM, ChaCha20, etc.)
 */
class ISecurityProvider {
public:
    virtual ~ISecurityProvider() = default;
    
    /**
     * @brief Decrypt input data and store result in output
     * @param input Encrypted input data
     * @param output Decrypted output data
     * @return true if decryption successful, false otherwise
     */
    virtual bool Decrypt(const std::string& input, std::string& output) = 0;
    
    // Optionally: virtual bool Encrypt(const std::string& input, std::string& output) = 0;
};