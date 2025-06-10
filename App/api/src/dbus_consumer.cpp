/**
 * @file dbus_consumer.cpp
 * @brief D-Bus message consumer implementation for Container Manager
 *
 * Implements the DBusConsumer class for handling container management requests
 * over the D-Bus session bus. Automatically detects and decodes Base64 data
 * before forwarding to the executor.
 */

#include "inc/dbus_consumer.hpp"
#include <iostream>
#include "inc/logging.hpp"

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

/**
 * @brief Simple Base64 decoder for all Base64 data.
 * @param encoded The Base64-encoded input string.
 * @return Decoded binary data as a string.
 */
std::string DBusConsumer::DecodeBase64(const std::string& encoded) {
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
 * @brief Check if string looks like Base64 data.
 * @param str Input string to check.
 * @return True if the string looks like Base64, false otherwise.
 */
bool DBusConsumer::IsBase64(const std::string& str) {
    if (str.empty() || str.length() % 4 != 0) return false;
    
    return str.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=") == std::string::npos;
}

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
 *        Handles Base64 decoding and forwards clean data to the executor.
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
                std::cout << "[DBus] Received payload of length: " << payload.length() << std::endl;
                
                std::string clean_payload = payload;
                
                // Detect and decode Base64 if present
                if (IsBase64(payload)) {
                    try {
                        clean_payload = DecodeBase64(payload);
                        std::cout << "[DBus] Decoded Base64 payload (original: " << payload.length() 
                                 << " bytes, decoded: " << clean_payload.length() << " bytes)" << std::endl;
                    } catch (const std::exception& e) {
                        std::cerr << "[DBus] Base64 decode failed: " << e.what() 
                                 << ", using original payload" << std::endl;
                        clean_payload = payload; // Fallback to original
                    }
                } else {
                    std::cout << "[DBus] Payload is plain text, forwarding as-is" << std::endl;
                }
                
                // Forward clean data to executor
                try {
                    auto result = executor_->Execute(clean_payload);
                    
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