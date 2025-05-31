#pragma once
#include <vector>
#include <string>

/**
 * Decrypts AES-256-GCM encrypted ciphertext.
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
);

/**
 * Decrypts an OTA (over-the-air) payload formatted as [IV|TAG|CIPHERTEXT].
 * @param input     The input string containing IV, tag, and ciphertext concatenated.
 * @param plaintext Output string for the decrypted data.
 * @return true on successful decryption and authentication, false otherwise.
 */
bool decrypt_ota_payload(
    const std::string& input,
    std::string& plaintext
);