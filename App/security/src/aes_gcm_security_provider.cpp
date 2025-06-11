/**
 * @file aes_gcm_security_provider.cpp
 * @brief Implementation of the AesGcmSecurityProvider class for AES-256-GCM decryption.
 *
 * This file provides authenticated decryption using the AES-256-GCM algorithm.
 * It is used by the executor layer to transparently decrypt incoming encrypted
 * payloads (JSON or Protobuf) for all protocols.
 * All configuration constants are sourced from AesGcmConfig struct.
 */

#include "inc/aes_gcm_security_provider.hpp"
#include "inc/common.hpp"
#include <fstream>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/rand.h>

/**
 * @brief Decrypts the input data using AES-256-GCM.
 * @param input The encrypted input data (IV|TAG|CIPHERTEXT).
 * @param output The decrypted output string.
 * @return True if decryption is successful, false otherwise.
 */
bool AesGcmSecurityProvider::Decrypt(const std::string& input, std::string& output) {
    return DecryptAesGcm(input, output);
}

/**
 * @brief Performs AES-256-GCM decryption.
 *        Expects input format: [12-byte IV][16-byte tag][ciphertext].
 * @param encrypted_data The encrypted input data.
 * @param decrypted_data The decrypted output string.
 * @return True if decryption is successful, false otherwise.
 */
bool AesGcmSecurityProvider::DecryptAesGcm(const std::string& encrypted_data, std::string& decrypted_data) {
    try {
        auto key = LoadAesKey();
        
        // AES-256-GCM format: [IV][tag][encrypted_payload]
        // Using AesGcmConfig constants for validation
        if (encrypted_data.size() < AesGcmConfig::kMinDataLen) {
            return false;
        }
        
        const unsigned char* iv = reinterpret_cast<const unsigned char*>(encrypted_data.data());
        const unsigned char* tag = iv + AesGcmConfig::kIvLen;
        const unsigned char* ciphertext = tag + AesGcmConfig::kTagLen;
        int ciphertext_len = encrypted_data.size() - AesGcmConfig::kMinDataLen;
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return false;
        
        // Initialize AES-256-GCM decryption
        if (EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        // Set IV length using config constant
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, AesGcmConfig::kIvLen, nullptr) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        // Initialize key and IV
        if (EVP_DecryptInit_ex(ctx, nullptr, nullptr, key.data(), iv) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        std::vector<unsigned char> plaintext(ciphertext_len);
        int len;
        
        // Decrypt the data
        if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext, ciphertext_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        // Set the authentication tag using config constant
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AesGcmConfig::kTagLen, 
                               const_cast<unsigned char*>(tag)) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false;
        }
        
        // Finalize decryption and verify authentication
        int final_len;
        if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &final_len) != 1) {
            EVP_CIPHER_CTX_free(ctx);
            return false; // Authentication failed
        }
        
        EVP_CIPHER_CTX_free(ctx);
        decrypted_data = std::string(reinterpret_cast<char*>(plaintext.data()), len + final_len);
        return true;
        
    } catch (const std::exception& e) {
        return false;
    }
}

/**
 * @brief Loads the AES-256 key from a hex-encoded file.
 * @param path Path to the key file (default: AesGcmConfig::kKeyFilePath).
 * @return The key as a vector of bytes.
 * @throws std::runtime_error if the file cannot be opened or the key is invalid.
 */
std::vector<unsigned char> AesGcmSecurityProvider::LoadAesKey(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Unable to open AES key file");
    
    std::string hex;
    file >> hex;
    
    // Use AesGcmConfig constants for validation
    if (hex.size() != AesGcmConfig::kKeyFileHexLen) {
        throw std::runtime_error("AES key must be " + std::to_string(AesGcmConfig::kKeyLen) + 
                                " bytes (" + std::to_string(AesGcmConfig::kKeyFileHexLen) + " hex chars)");
    }
    
    std::vector<unsigned char> key(AesGcmConfig::kKeyLen);
    for (size_t i = 0; i < AesGcmConfig::kKeyLen; ++i) {
        key[i] = static_cast<unsigned char>(std::stoi(hex.substr(i*2, 2), nullptr, 16));
    }
    return key;
}