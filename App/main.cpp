/**
 * @file main.cpp
 * @brief Entry point for the Container Manager application.
 *
 * This file initializes all core subsystems (logging, database, message queue, MQTT, etc.)
 * and starts all enabled protocol consumers:
 *   - HTTP server (REST API)
 *   - MQTT subscriber
 *   - POSIX Message Queue consumer
 *   - D-Bus consumer (session bus)
 *
 * Protocols and data formats are enabled/disabled via CMake flags.
 */

#include <memory>
#include <thread>
#include <vector>
#include <atomic>
#include <csignal>
#include <iostream>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/init_handler.hpp"
#include "inc/container_service.hpp"
#include "inc/json_request_executor.hpp"
#if ENABLE_REDIS
#include "inc/redis_database.hpp"
#else
#include "inc/embedded_database.hpp"
#endif
#if ENABLE_PROTOBUF
#include "inc/protobuf_request_executor.hpp"
#endif
#if ENABLE_REST
#include "inc/http_server.hpp"
#endif
#if ENABLE_MQTT
#include "inc/mosquitto_mqtt_subscriber.hpp"
#endif
#if ENABLE_MSGQUEUE
#include "inc/posix_message_queue_consumer.hpp"
#endif
#if ENABLE_DBUS
#ifdef DEPRECATED
#undef DEPRECATED
#endif
#include "inc/dbus_consumer.hpp"
#endif

std::atomic<bool> shutdown_requested{false};

/**
 * @brief Signal handler for graceful shutdown.
 *
 * Sets the shutdown flag and prints a shutdown message.
 * Called when SIGINT or SIGTERM is received.
 */
void SignalHandler(int) {
    shutdown_requested = true;
    std::cout << "Shutdown signal received. Stopping protocol consumers..." << std::endl;
}

/**
 * @brief Main entry point for the Container Manager application.
 *        Initializes all subsystems, starts protocol consumers, and handles graceful shutdown.
 * @return Exit code.
 */
int main() {
    // Instantiate the database and service handler using dependency injection
#if ENABLE_REDIS
    RedisDatabaseHandler db;
#else
    EmbeddedDatabaseHandler db;
#endif
    ContainerServiceHandler service(db);

    // Initialize all subsystems (logging, database, message queue, MQTT, etc.)
    InitProject(db);

    // Create the request executor (Protobuf or JSON) with injected dependencies
#if ENABLE_PROTOBUF
    auto executor = std::make_shared<ProtoRequestExecutorHandler>(db, service);
#else
    auto executor = std::make_shared<JsonRequestExecutorHandler>(db, service);
#endif

    // Register signal handler for graceful shutdown
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    std::vector<std::thread> protocol_threads;

#if ENABLE_REST
    // Start HTTP server in a separate thread
    ServerConfig server_cfg;
    HttpServerHandler server(executor, server_cfg.ThreadPoolSize);
    protocol_threads.emplace_back([&server, &server_cfg]() {
        server.Start(server_cfg.Host, server_cfg.Port);
    });
#endif

#if ENABLE_MQTT
    // Start MQTT subscriber in a separate thread
    MqttConfig mqtt_cfg;
    auto mqtt_consumer = std::make_shared<MosquittoMqttSubscriber>(
        mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    protocol_threads.emplace_back([mqtt_consumer]() {
        mqtt_consumer->Start();
    });
#endif

#if ENABLE_MSGQUEUE
    // Start POSIX message queue consumer in a separate thread
    MessageQueueConfig mq_cfg;
    auto mq_consumer = std::make_shared<MessageQueueConsumer>(mq_cfg, executor);
    protocol_threads.emplace_back([mq_consumer]() {
        mq_consumer->Start();
    });
#endif

#if ENABLE_DBUS
    // Start D-Bus consumer in a separate thread
    DbusConfig dbus_cfg;
    auto dbus_consumer = std::make_shared<DBusConsumer>(dbus_cfg, executor);
    protocol_threads.emplace_back([dbus_consumer]() {
        dbus_consumer->Start();
    });
#endif

    /**
     * Main thread waits for shutdown signal.
     * When shutdown is requested, Stop() is called on all protocol consumers.
     */
    while (!shutdown_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(kMainShutdownPollMs));
    }

    // Stop protocol consumers gracefully
#if ENABLE_REST
    server.Stop();
#endif
#if ENABLE_MQTT
    mqtt_consumer->Stop();
#endif
#if ENABLE_MSGQUEUE
    mq_consumer->Stop();
#endif
#if ENABLE_DBUS
    dbus_consumer->Stop();
#endif

    // Wait for all protocol threads to finish
    for (auto& t : protocol_threads) {
        if (t.joinable()) t.join();
    }

    // Shutdown logging and exit
    google::ShutdownGoogleLogging();
    return 0;
}