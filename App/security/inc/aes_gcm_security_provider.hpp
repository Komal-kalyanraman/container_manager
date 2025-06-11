/**
 * @file aes_gcm_security_provider.hpp
 * @brief Declares the AesGcmSecurityProvider class for AES-256-GCM decryption.
 *
 * This class implements the ISecurityProvider interface and provides
 * authenticated decryption using the AES-256-GCM algorithm.
 * It is used by the executor layer to transparently decrypt incoming
 * encrypted payloads (JSON or Protobuf) for all protocols.
 * Configuration constants are defined in AesGcmConfig struct.
 */

#pragma once

#include "inc/security_provider.hpp"
#include <string>
#include <vector>
#include "inc/common.hpp"

/**
 * @class AesGcmSecurityProvider
 * @brief AES-256-GCM implementation for secure OTA payloads.
 *
 * Provides authenticated decryption using AES-256-GCM.
 * Loads the key from a hex-encoded file at runtime using AesGcmConfig constants.
 * Expected input format: [12-byte IV][16-byte tag][ciphertext]
 */
class AesGcmSecurityProvider : public ISecurityProvider {
public:
    /**
     * @brief Default constructor.
     */
    AesGcmSecurityProvider() = default;

    /**
     * @brief Default destructor.
     */
    ~AesGcmSecurityProvider() override = default;
    
    /**
     * @brief Decrypts the input data using AES-256-GCM.
     * @param input The encrypted input data (IV|TAG|CIPHERTEXT).
     * @param output The decrypted output string.
     * @return True if decryption is successful, false otherwise.
     */
    bool Decrypt(const std::string& input, std::string& output) override;
    
private:
    /**
     * @brief Loads the AES-256 key from a hex-encoded file.
     * @param path Path to the key file (default: AesGcmConfig::kKeyFilePath).
     * @return The key as a vector of bytes.
     * @throws std::runtime_error if the file cannot be opened or the key is invalid.
     */
    std::vector<unsigned char> LoadAesKey(const std::string& path = AesGcmConfig::kKeyFilePath);

    /**
     * @brief Performs AES-256-GCM decryption.
     * @param encrypted_data The encrypted input data.
     * @param decrypted_data The decrypted output string.
     * @return True if decryption is successful, false otherwise.
     */
    bool DecryptAesGcm(const std::string& encrypted_data, std::string& decrypted_data);
};