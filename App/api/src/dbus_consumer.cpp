/**
 * @file dbus_consumer.cpp
 * @brief Implements the DbusConsumer class for receiving and processing JSON requests over D-Bus.
 */

#include "inc/dbus_consumer.hpp"
#include <iostream>

DbusConsumer::DbusConsumer(const DbusConfig& config, std::shared_ptr<RequestExecutor> executor)
    : config_(config), executor_(std::move(executor)), running_(false) {}

DbusConsumer::~DbusConsumer() {
    Stop();
}

void DbusConsumer::Start() {
    running_ = true;
    listenThread_ = std::thread(&DbusConsumer::ListenLoop, this);
}

void DbusConsumer::Stop() {
    running_ = false;
    if (listenThread_.joinable())
        listenThread_.join();
}

void DbusConsumer::ListenLoop() {
    // Connect to the session bus and request the name
    connection_ = sdbus::createSessionBusConnection();
    connection_->requestName(config_.BusName);

    // Create an object to listen for method calls
    object_ = sdbus::createObject(*connection_, config_.ObjectPath);

    // Register a method handler for "Execute"
    object_->registerMethod(config_.Method)
        .onInterface(config_.Interface)
        .implementedAs([this](const std::string& jsonPayload) {
            std::cout << "[DBus] Received: " << jsonPayload << std::endl;
            try {
                executor_->Execute(jsonPayload);
            } catch (const std::exception& e) {
                std::cerr << "[DBus] Error executing request: " << e.what() << std::endl;
            }
        });

    object_->finishRegistration();

    // Enter the event loop
    while (running_) {
        connection_->processPendingRequest();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
}