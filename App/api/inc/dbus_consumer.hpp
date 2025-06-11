/**
 * @file dbus_consumer.hpp
 * @brief Defines the DBusConsumer class for receiving and processing requests over D-Bus.
 */

#pragma once

#include <atomic>
#include <memory>
#include <string>
#include <thread>

#include "inc/common.hpp"
#include <sdbus-c++/sdbus-c++.h>
#include "inc/request_executor.hpp"

/**
 * @class DBusConsumer
 * @brief Handles receiving and processing requests over D-Bus.
 *
 * This class connects to the D-Bus session bus, registers an object and interface,
 * and listens for incoming method calls. All D-Bus payloads are expected to be
 * Base64-encoded and are automatically decoded before forwarding to the RequestExecutor.
 * This ensures consistent data handling regardless of payload type (JSON, Protobuf, encrypted).
 */
class DBusConsumer {
public:
    /**
     * @brief Construct a DBusConsumer.
     * @param config The DbusConfig struct containing bus name, object path, and interface.
     * @param executor Shared pointer to the request executor for processing incoming requests.
     */
    DBusConsumer(const DbusConfig& config, std::shared_ptr<RequestExecutor> executor);

    /**
     * @brief Destructor. Stops the consumer if running.
     */
    ~DBusConsumer();

    /**
     * @brief Start listening for D-Bus method calls in a background thread.
     */
    void Start();

    /**
     * @brief Stop listening and join the background thread.
     */
    void Stop();

private:
    /**
     * @brief Main loop for processing incoming D-Bus requests.
     */
    void ListenLoop();

    /**
     * @brief Decode Base64 string to binary data.
     * @param encoded The Base64-encoded input string.
     * @return Decoded binary data as a string.
     * @throws std::runtime_error if Base64 decoding fails.
     */
    std::string DecodeBase64(const std::string& encoded);

    DbusConfig config_;                                  ///< D-Bus configuration struct.
    std::string bus_name_;                               ///< The D-Bus service name to acquire.
    std::string object_path_;                            ///< The D-Bus object path to register.
    std::shared_ptr<RequestExecutor> executor_;          ///< Shared request executor.
    std::unique_ptr<sdbus::IConnection> connection_;     ///< D-Bus connection.
    std::unique_ptr<sdbus::IObject> object_;             ///< D-Bus object.
    std::atomic<bool> running_{false};                   ///< Indicates if the consumer is running.
    std::thread listen_thread_;                          ///< Background thread for listening.
};