/**
 * @file dbus_consumer.cpp
 * @brief Implements the DBusConsumer class for receiving and processing JSON/Protobuf requests over D-Bus.
 */

#include "inc/dbus_consumer.hpp"
#include <iostream>
#include "inc/logging.hpp"

DBusConsumer::DBusConsumer(const DbusConfig& config, std::shared_ptr<RequestExecutor> executor)
    : config_(config),
      bus_name_(config.BusName),
      object_path_(config.ObjectPath),
      executor_(std::move(executor)),
      running_(false) {}

DBusConsumer::~DBusConsumer() {
    Stop();
}

void DBusConsumer::Start() {
    running_ = true;
    listen_thread_ = std::thread(&DBusConsumer::ListenLoop, this);
}

void DBusConsumer::Stop() {
    running_ = false;
    if (listen_thread_.joinable())
        listen_thread_.join();
}

void DBusConsumer::ListenLoop() {
    // Connect to the session bus and request the name
    connection_ = sdbus::createSessionBusConnection();
    connection_->requestName(bus_name_);

    // Create an object to listen for method calls
    object_ = sdbus::createObject(*connection_, object_path_);

    // Register a method handler for "Execute"
    object_->registerMethod(config_.Method)
        .onInterface(config_.Interface)
        .implementedAs([this](const std::string& jsonPayload) {
            CM_LOG_INFO << "[DBus] Received: " << jsonPayload << std::endl;
            try {
                auto result = executor_->Execute(jsonPayload);
                // Log the result status
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

    // Enter the event loop
    while (running_) {
        connection_->processPendingRequest();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}