#include "inc/chacha20_security_provider.hpp"
#include "inc/common.hpp"
#include <fstream>
#include <stdexcept>
#include <openssl/evp.h>
#include <openssl/rand.h>

bool ChaCha20SecurityProvider::Decrypt(const std::string& input, std::string& output) {
    return DecryptChaCha20Poly1305(input, output);
}

bool ChaCha20SecurityProvider::DecryptChaCha20Poly1305(const std::string& encrypted_data, std::string& decrypted_data) {
    try {
        auto key = LoadChaChaKey();
        
        // ChaCha20-Poly1305 format: [12-byte nonce][16-byte tag][encrypted_payload]
        if (encrypted_data.size() < 28) { // 12 + 16 = 28 minimum
            return false;
        }
        
        const unsigned char* nonce = reinterpret_cast<const unsigned char*>(encrypted_data.data());
        const unsigned char* tag = nonce + 12;
        const unsigned char* ciphertext = tag + 16;
        int ciphertext_len = encrypted_data.size() - 28;
        
        EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
        if (!ctx) return false;
        
        // Initialize ChaCha20-Poly1305 decryption
        if (EVP_DecryptInit_ex(ctx, EVP_chacha20_poly1305(), nullptr, key.data(), nonce) != 1) {
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
        
        // Set the authentication tag
        if (EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_AEAD_SET_TAG, 16, const_cast<unsigned char*>(tag)) != 1) {
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

std::vector<unsigned char> ChaCha20SecurityProvider::LoadChaChaKey(const std::string& path) {
    std::ifstream file(path);
    if (!file) throw std::runtime_error("Unable to open ChaCha20 key file");
    std::string hex;
    file >> hex;
    if (hex.size() != 64) throw std::runtime_error("ChaCha20 key must be 32 bytes (64 hex chars)");
    std::vector<unsigned char> key(32);
    for (size_t i = 0; i < 32; ++i) {
        key[i] = static_cast<unsigned char>(std::stoi(hex.substr(i*2, 2), nullptr, 16));
    }
    return key;
}