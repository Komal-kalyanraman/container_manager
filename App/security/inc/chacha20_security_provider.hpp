#pragma once

#include "inc/security_provider.hpp"
#include <string>
#include <vector>

#include "inc/common.hpp"
/**
 * @brief ChaCha20-Poly1305 implementation for automotive OTA security
 * Commonly used in automotive due to excellent performance on ARM processors
 * and resistance to timing attacks
 */
class ChaCha20SecurityProvider : public ISecurityProvider {
public:
    ChaCha20SecurityProvider() = default;
    ~ChaCha20SecurityProvider() override = default;
    
    bool Decrypt(const std::string& input, std::string& output) override;
    
private:
    std::vector<unsigned char> LoadChaChaKey(const std::string& path = kChaCha20FilePath);
    bool DecryptChaCha20Poly1305(const std::string& encrypted_data, std::string& decrypted_data);
};