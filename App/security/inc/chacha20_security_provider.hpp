/**
 * @file chacha20_security_provider.hpp
 * @brief Declares the ChaCha20SecurityProvider class for ChaCha20-Poly1305 decryption.
 *
 * This class implements the ISecurityProvider interface and provides
 * authenticated decryption using the ChaCha20-Poly1305 algorithm.
 * It is commonly used in automotive and embedded systems due to its
 * high performance on ARM processors and resistance to timing attacks.
 * The executor layer uses this class to transparently decrypt incoming
 * encrypted payloads (JSON or Protobuf) for all protocols.
 */

#pragma once

#include "inc/security_provider.hpp"
#include <string>
#include <vector>
#include "inc/common.hpp"

/**
 * @class ChaCha20SecurityProvider
 * @brief ChaCha20-Poly1305 implementation for secure OTA payloads.
 *
 * Provides authenticated decryption using ChaCha20-Poly1305.
 * Loads the key from a hex-encoded file at runtime.
 */
class ChaCha20SecurityProvider : public ISecurityProvider {
public:
    /**
     * @brief Default constructor.
     */
    ChaCha20SecurityProvider() = default;

    /**
     * @brief Default destructor.
     */
    ~ChaCha20SecurityProvider() override = default;
    
    /**
     * @brief Decrypts the input data using ChaCha20-Poly1305.
     * @param input The encrypted input data (NONCE|TAG|CIPHERTEXT).
     * @param output The decrypted output string.
     * @return True if decryption is successful, false otherwise.
     */
    bool Decrypt(const std::string& input, std::string& output) override;
    
private:
    /**
     * @brief Loads the ChaCha20 key from a hex-encoded file.
     * @param path Path to the key file (default: kChaCha20FilePath).
     * @return The key as a vector of bytes.
     */
    std::vector<unsigned char> LoadChaChaKey(const std::string& path = kChaCha20FilePath);

    /**
     * @brief Performs ChaCha20-Poly1305 decryption.
     * @param encrypted_data The encrypted input data.
     * @param decrypted_data The decrypted output string.
     * @return True if decryption is successful, false otherwise.
     */
    bool DecryptChaCha20Poly1305(const std::string& encrypted_data, std::string& decrypted_data);
};