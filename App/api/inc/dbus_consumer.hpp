/**
 * @file dbus_consumer.hpp
 * @brief Defines the DBusConsumer class for receiving and processing JSON/Protobuf requests over D-Bus.
 */

#pragma once

#include <memory>
#include <string>
#include <thread>

#include "inc/common.hpp"
#include <sdbus-c++/sdbus-c++.h>
#include "inc/request_executor.hpp"

/**
 * @class DBusConsumer
 * @brief Handles receiving and processing JSON/Protobuf requests over D-Bus.
 *
 * This class connects to the D-Bus session bus, registers an object and interface,
 * and listens for incoming method calls (e.g., "Execute") from other applications.
 * Incoming JSON/Protobuf payloads are forwarded to the shared RequestExecutor for processing.
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

    DbusConfig config_;                                  ///< D-Bus configuration struct.
    std::string bus_name_;                               ///< The D-Bus service name to acquire.
    std::string object_path_;                            ///< The D-Bus object path to register.
    std::shared_ptr<RequestExecutor> executor_;          ///< Shared request executor.
    std::unique_ptr<sdbus::IConnection> connection_;     ///< D-Bus connection.
    std::unique_ptr<sdbus::IObject> object_;             ///< D-Bus object.
    bool running_;                                       ///< Indicates if the consumer is running.
    std::thread listen_thread_;                          ///< Background thread for listening.
};