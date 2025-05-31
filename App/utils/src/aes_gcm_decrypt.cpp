#include "inc/aes_gcm_decrypt.hpp"

#include <fstream>
#include <vector>
#include <stdexcept>
#include <cstring>
#include <openssl/evp.h>

#include "inc/common.hpp"

/**
 * @brief Loads the AES-256 key from a file at runtime.
 *        The key file should contain a 64-character hex string (32 bytes).
 * @param path Path to the key file (default: kAesKeyFilePath).
 * @return std::vector<unsigned char> containing the 32-byte key.
 * @throws std::runtime_error if the file cannot be opened or the key is invalid.
 */
std::vector<unsigned char> LoadAesKey(const std::string& path = kAesKeyFilePath) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Unable to open AES key file");
    std::string hex;
    file >> hex;
    if (hex.size() != 64) throw std::runtime_error("AES key must be 32 bytes (64 hex chars)");
    std::vector<unsigned char> key(32);
    for (size_t i = 0; i < 32; ++i) {
        key[i] = static_cast<unsigned char>(std::stoi(hex.substr(i*2, 2), nullptr, 16));
    }
    return key;
}

/**
 * @brief Decrypts AES-256-GCM encrypted ciphertext.
 * 
 * @param ciphertext The encrypted data (excluding IV and tag).
 * @param key        The 32-byte AES key.
 * @param iv         The 12-byte initialization vector (IV).
 * @param tag        The 16-byte authentication tag.
 * @param plaintext  Output vector for the decrypted data.
 * @return true on successful decryption and authentication, false otherwise.
 */
bool aes_gcm_decrypt(
    const std::vector<unsigned char>& ciphertext,
    const unsigned char* key,
    const unsigned char* iv,
    const unsigned char* tag,
    std::vector<unsigned char>& plaintext
) {
    // Create and initialize the decryption context
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;
    int len = 0;
    int plaintext_len = 0;
    plaintext.resize(ciphertext.size());

    // Initialize decryption operation for AES-256-GCM
    bool ok = EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1
        && EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, nullptr) == 1
        && EVP_DecryptInit_ex(ctx, nullptr, nullptr, key, iv) == 1
        && EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) == 1;
    plaintext_len = len;

    // Set expected authentication tag value
    ok = ok && EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag) == 1;

    // Finalize decryption and check authentication
    int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    EVP_CIPHER_CTX_free(ctx);

    if (!ok || ret <= 0) return false;
    plaintext.resize(plaintext_len);
    return true;
}

/**
 * @brief Decrypts an OTA (over-the-air) payload formatted as [IV|TAG|CIPHERTEXT].
 *        Loads the AES key from file at runtime.
 * 
 * @param input     The input string containing IV, tag, and ciphertext concatenated.
 * @param plaintext Output string for the decrypted data.
 * @return true on successful decryption and authentication, false otherwise.
 */
bool decrypt_ota_payload(const std::string& input, std::string& plaintext) {
    // Check input size is sufficient for IV and tag
    if (input.size() < kAesIvLen + kAesTagLen) return false;

    // Extract IV, tag, and ciphertext from the input
    const unsigned char* iv = reinterpret_cast<const unsigned char*>(input.data());
    const unsigned char* tag = reinterpret_cast<const unsigned char*>(input.data()) + kAesIvLen;
    const unsigned char* ciphertext = reinterpret_cast<const unsigned char*>(input.data()) + kAesIvLen + kAesTagLen;
    size_t ciphertext_len = input.size() - kAesIvLen - kAesTagLen;

    std::vector<unsigned char> plain;
    std::vector<unsigned char> key;
    try {
        // Load AES key from file
        key = LoadAesKey();
    } catch (const std::exception& e) {
        return false;
    }

    // Perform AES-GCM decryption
    bool ok = aes_gcm_decrypt(
        std::vector<unsigned char>(ciphertext, ciphertext + ciphertext_len),
        key.data(),
        iv,
        tag,
        plain
    );
    if (ok) {
        plaintext.assign(plain.begin(), plain.end());
    }
    return ok;
}