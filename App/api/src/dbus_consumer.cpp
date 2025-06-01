/**
 * @file dbus_consumer.cpp
 * @brief D-Bus message consumer implementation for Container Manager
 *
 * Implements the DBusConsumer class for handling container management requests
 * over the D-Bus session bus. Supports encrypted and unencrypted payloads,
 * automatic Base64 decoding for binary data, and both JSON and Protobuf formats.
 */

#include "inc/dbus_consumer.hpp"
#include <iostream>
#include "inc/logging.hpp"

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/buffer.h>
#endif

/**
 * @brief Constructs D-Bus consumer with configuration and request executor.
 */
DBusConsumer::DBusConsumer(const DbusConfig& config, std::shared_ptr<RequestExecutor> executor)
    : config_(config),
      bus_name_(config.BusName),
      object_path_(config.ObjectPath),
      executor_(std::move(executor)),
      running_(false) {}

/**
 * @brief Destructor ensures clean shutdown of D-Bus consumer.
 */
DBusConsumer::~DBusConsumer() {
    Stop();
}

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
/**
 * @brief Decode Base64 string using OpenSSL BIO interface.
 * @param encoded The Base64-encoded input string.
 * @return Decoded binary data as a string.
 */
std::string base64_decode(const std::string& encoded) {
    BIO *bio, *b64;
    int decode_len = ((encoded.length() * 3) / 4);
    char *buffer = new char[decode_len + 1];
    
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(encoded.c_str(), encoded.length());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    
    int length = BIO_read(bio, buffer, encoded.length());
    BIO_free_all(bio);
    
    std::string result(buffer, length > 0 ? length : 0);
    delete[] buffer;
    return result;
}

/**
 * @brief Check if string contains valid Base64 data.
 * @param str Input string to check.
 * @return True if the string is valid Base64, false otherwise.
 */
bool is_base64(const std::string& str) {
    if (str.empty() || str.length() % 4 != 0) return false;
    
    // Check for valid Base64 characters
    for (char c : str) {
        if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || 
              (c >= '0' && c <= '9') || c == '+' || c == '/' || c == '=')) {
            return false;
        }
    }
    return true;
}
#else
/**
 * @brief Simple Base64 decoder when encryption is disabled.
 * @param encoded The Base64-encoded input string.
 * @return Decoded binary data as a string.
 */
std::string simple_base64_decode(const std::string& encoded) {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    int val = 0, valb = -8;
    
    for (unsigned char c : encoded) {
        if (c == '=') break;
        size_t pos = chars.find(c);
        if (pos == std::string::npos) continue;
        
        val = (val << 6) + pos;
        valb += 6;
        if (valb >= 0) {
            decoded.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return decoded;
}

/**
 * @brief Check if string looks like Base64 when encryption is disabled.
 * @param str Input string to check.
 * @return True if the string looks like Base64, false otherwise.
 */
bool looks_like_base64(const std::string& str) {
    if (str.empty() || str.length() % 4 != 0) return false;
    
    return str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=") == std::string::npos;
}
#endif

/**
 * @brief Start the D-Bus consumer in a separate thread.
 */
void DBusConsumer::Start() {
    running_ = true;
    listen_thread_ = std::thread(&DBusConsumer::ListenLoop, this);
}

/**
 * @brief Stop the D-Bus consumer and wait for thread completion.
 */
void DBusConsumer::Stop() {
    running_ = false;
    if (listen_thread_.joinable())
        listen_thread_.join();
}

/**
 * @brief Main D-Bus event loop for processing incoming method calls.
 *        Handles Base64 decoding if needed and forwards payload to the executor.
 */
void DBusConsumer::ListenLoop() {
    try {
        // Establish D-Bus session connection and register object
        connection_ = sdbus::createSessionBusConnection();
        connection_->requestName(bus_name_);
        object_ = sdbus::createObject(*connection_, object_path_);

        // Register the method handler for incoming requests
        object_->registerMethod(config_.Method)
            .onInterface(config_.Interface)
            .implementedAs([this](const std::string& payload) {
                std::string actual_payload = payload;
                
                std::cout << "[DBus] Received payload of length: " << payload.length() << std::endl;
                
                // Detect and handle Base64-encoded data
                bool is_base64_data = false;
#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
                is_base64_data = is_base64(payload);
#else
                is_base64_data = looks_like_base64(payload);
#endif

                if (is_base64_data) {
                    try {
#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
                        actual_payload = base64_decode(payload);
                        std::cout << "[DBus] Successfully decoded Base64 payload using OpenSSL" << std::endl;
#else
                        actual_payload = simple_base64_decode(payload);
                        std::cout << "[DBus] Successfully decoded Base64 payload" << std::endl;
#endif
                    } catch (const std::exception& e) {
                        std::cerr << "[DBus] Base64 decode failed: " << e.what() << std::endl;
                        actual_payload = payload; // Fall back to original
                    }
                } else {
                    std::cout << "[DBus] Processing payload as plain text" << std::endl;
                }
                
                // Forward to executor for decryption and processing
                try {
                    auto result = executor_->Execute(actual_payload);
                    
                    if (result.contains("status")) {
                        if (result["status"] == "error") {
                            std::cerr << "[DBus] Execution error: " 
                                     << result.value("message", "Unknown error") << std::endl;
                        } else {
                            std::cout << "[DBus] Execution success." << std::endl;
                        }
                    }
                } catch (const std::exception& e) {
                    std::cerr << "[DBus] Error executing request: " << e.what() << std::endl;
                }
            });

        object_->finishRegistration();
        std::cout << "[DBus] Consumer started on " << bus_name_ << object_path_ << std::endl;

        // Main event loop: process incoming D-Bus requests
        while (running_) {
            connection_->processPendingRequest();
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }
    } catch (const std::exception& e) {
        std::cerr << "[DBus] Error in ListenLoop: " << e.what() << std::endl;
    }
}