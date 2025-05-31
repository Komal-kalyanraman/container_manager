#include "inc/aes_gcm_decrypt.hpp"
#include "inc/common.hpp"
#include <openssl/evp.h>
#include <cstring>

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
) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;
    int len = 0;
    int plaintext_len = 0;
    plaintext.resize(ciphertext.size());
    // Initialize decryption context for AES-256-GCM
    bool ok = EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, nullptr, nullptr) == 1
        && EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, 12, nullptr) == 1
        && EVP_DecryptInit_ex(ctx, nullptr, nullptr, key, iv) == 1
        && EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) == 1;
    plaintext_len = len;
    // Set authentication tag and finalize decryption
    ok = ok && EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, 16, (void*)tag) == 1;
    int ret = EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len);
    EVP_CIPHER_CTX_free(ctx);
    if (!ok || ret <= 0) return false;
    plaintext.resize(plaintext_len);
    return true;
}

/**
 * Decrypts an OTA (over-the-air) payload formatted as [IV|TAG|CIPHERTEXT].
 * @param input     The input string containing IV, tag, and ciphertext concatenated.
 * @param plaintext Output string for the decrypted data.
 * @return true on successful decryption and authentication, false otherwise.
 */
bool decrypt_ota_payload(const std::string& input, std::string& plaintext) {
    if (input.size() < kAesIvLen + kAesTagLen) return false;
    // Extract IV, tag, and ciphertext from the input
    const unsigned char* iv = reinterpret_cast<const unsigned char*>(input.data());
    const unsigned char* tag = reinterpret_cast<const unsigned char*>(input.data()) + kAesIvLen;
    const unsigned char* ciphertext = reinterpret_cast<const unsigned char*>(input.data()) + kAesIvLen + kAesTagLen;
    size_t ciphertext_len = input.size() - kAesIvLen - kAesTagLen;
    std::vector<unsigned char> plain;
    // Decrypt the payload using the global AES key
    bool ok = aes_gcm_decrypt(
        std::vector<unsigned char>(ciphertext, ciphertext + ciphertext_len),
        kAesKey,
        iv,
        tag,
        plain
    );
    if (ok) {
        plaintext.assign(plain.begin(), plain.end());
    }
    return ok;
}