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
#include <algorithm>
#endif

#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
std::string base64_decode(const std::string& encoded);
bool is_base64(const std::string& str);
#else
std::string simple_base64_decode(const std::string& encoded);
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
 */
std::string base64_decode(const std::string& encoded) {
    BIO *bio, *b64;
    char *buffer = (char*)malloc(encoded.length());
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_mem_buf(encoded.c_str(), encoded.length());
    bio = BIO_push(b64, bio);
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    int length = BIO_read(bio, buffer, encoded.length());
    BIO_free_all(bio);
    std::string result(buffer, length);
    free(buffer);
    return result;
}

/**
 * @brief Check if string contains valid Base64 data.
 */
bool is_base64(const std::string& str) {
    if (str.empty() || str.length() % 4 != 0) return false;
    return str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=") == std::string::npos;
}
#else
/**
 * @brief Simple Base64 decoder for Protobuf data when encryption is disabled.
 */
std::string simple_base64_decode(const std::string& encoded) {
    static const std::string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string decoded;
    int val = 0, valb = -8;
    for (unsigned char c : encoded) {
        if (chars.find(c) == std::string::npos) break;
        val = (val << 6) + chars.find(c);
        valb += 6;
        if (valb >= 0) {
            decoded.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return decoded;
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
 */
void DBusConsumer::ListenLoop() {
    connection_ = sdbus::createSessionBusConnection();
    connection_->requestName(bus_name_);
    object_ = sdbus::createObject(*connection_, object_path_);

    object_->registerMethod(config_.Method)
        .onInterface(config_.Interface)
        .implementedAs([this](const std::string& jsonPayload) {
            std::string actual_payload = jsonPayload;
            CM_LOG_INFO << "[DBus] Received: " << jsonPayload << std::endl;
#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
            if (is_base64(jsonPayload)) {
#else
            bool looks_like_base64 = !jsonPayload.empty() && 
                                   jsonPayload.length() % 4 == 0 &&
                                   jsonPayload.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=") == std::string::npos;
            if (looks_like_base64) {
#endif
                try {
#if defined(ENABLE_ENCRYPTION) && ENABLE_ENCRYPTION
                    actual_payload = base64_decode(jsonPayload);
                    CM_LOG_INFO << "[DBus] Decoded Base64 payload using OpenSSL" << std::endl;
#else
                    actual_payload = simple_base64_decode(jsonPayload);
                    CM_LOG_INFO << "[DBus] Decoded Base64 protobuf payload" << std::endl;
#endif
                } catch (const std::exception& e) {
                    CM_LOG_WARN << "[DBus] Base64 decode failed, treating as plain text: " << e.what() << std::endl;
                    actual_payload = jsonPayload;
                }
            }
            try {
                auto result = executor_->Execute(actual_payload);
                if (result.contains("status")) {
                    if (result["status"] == "error") {
                        CM_LOG_ERROR << "[DBus] Execution error: "
                                     << result.value("message", "Unknown error") << std::endl;
                    } else {
                        CM_LOG_INFO << "[DBus] Execution success." << std::endl;
                    }
                } else {
                    CM_LOG_WARN << "[DBus] No status field in execution result." << std::endl;
                }
            } catch (const std::exception& e) {
                CM_LOG_ERROR << "[DBus] Error executing request: " << e.what() << std::endl;
            }
        });

    object_->finishRegistration();

    while (running_) {
        connection_->processPendingRequest();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}