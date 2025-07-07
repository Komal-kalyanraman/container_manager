/**
 * @file main.cpp
 * @brief Entry point for the Container Manager service.
 *
 * This file contains the main function for the Container Manager project.
 * It initializes the selected database backend, sets up the service and protocol handlers,
 * and starts protocol consumers (REST, MQTT, Message Queue, D-Bus) in separate threads.
 * The application supports modular protocol and data format selection, pluggable database backends,
 * and optional encryption (AES-GCM or ChaCha20). Graceful shutdown is handled via signal handlers.
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
#include "inc/heartbeat_server.hpp" 
#include "inc/container_service.hpp"
#include "inc/json_request_executor.hpp"
#include "inc/null_security_provider.hpp"

// Conditionally include security providers based on selected algorithm
#if defined(SECURITY_ALGORITHM_AES_GCM)
#include "inc/aes_gcm_security_provider.hpp"
#elif defined(SECURITY_ALGORITHM_CHACHA20)
#include "inc/chacha20_security_provider.hpp"
#endif

// Conditionally include database backends
#if ENABLE_REDIS
#include "inc/redis_database.hpp"
#else
#include "inc/embedded_database.hpp"
#endif

// Conditionally include protobuf executor
#if ENABLE_PROTOBUF
#include "inc/protobuf_request_executor.hpp"
#endif

// Conditionally include protocol consumers
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

/**
 * @var shutdown_requested
 * @brief Atomic flag to signal shutdown to all protocol consumers.
 */
std::atomic<bool> shutdown_requested{false};

/**
 * @brief Signal handler for graceful shutdown (SIGINT/SIGTERM).
 * @param signum Signal number.
 */
void SignalHandler(int signum) {
    shutdown_requested = true;
    std::cout << "Shutdown signal received. Stopping protocol consumers..." << std::endl;
}

/**
 * @brief Main entry point for the Container Manager service.
 *
 * Initializes logging, selects the database backend (Redis or Embedded), sets up the service handler,
 * and starts protocol consumers for REST, MQTT, POSIX Message Queue, and D-Bus (if enabled).
 * Handles graceful shutdown and joins all protocol threads before exiting.
 *
 * Protocol consumers (including the heartbeat server) are configured using their respective config
 * structures for consistency and maintainability.
 *
 * @return int Exit code.
 */
int main() {
    // Print enabled features at startup
    std::cout << "==== " << kServiceName << " v" << kVersion << " ====" << std::endl;
    std::cout << "Features enabled:" << std::endl;
    std::cout << "  ✓ Heartbeat Server" << std::endl;
#if ENABLE_REST
    std::cout << "  ✓ REST/HTTP Server" << std::endl;
#endif
#if ENABLE_MQTT
    std::cout << "  ✓ MQTT Subscriber" << std::endl;
#endif
#if ENABLE_MSGQUEUE
    std::cout << "  ✓ POSIX Message Queue" << std::endl;
#endif
#if ENABLE_DBUS
    std::cout << "  ✓ D-Bus Consumer" << std::endl;
#endif
#if ENABLE_PROTOBUF
    std::cout << "  ✓ Protobuf Support" << std::endl;
#else
    std::cout << "  ✓ JSON Support" << std::endl;
#endif
#if ENABLE_REDIS
    std::cout << "  ✓ Redis Database" << std::endl;
#else
    std::cout << "  ✓ Embedded Database" << std::endl;
#endif
#if ENABLE_ENCRYPTION
    #if defined(SECURITY_ALGORITHM_AES_GCM)
        std::cout << "  🔒 Security: AES-256-GCM" << std::endl;
    #elif defined(SECURITY_ALGORITHM_CHACHA20)
        std::cout << "  🔒 Security: ChaCha20-Poly1305 (Automotive)" << std::endl;
    #endif
#else
    std::cout << "  🔓 Security: DISABLED" << std::endl;
#endif
    std::cout << "===================================" << std::endl;

    // Select database backend (Redis or Embedded)
#if ENABLE_REDIS
    auto db = std::make_unique<RedisDatabaseHandler>();
#else
    auto db = std::make_unique<EmbeddedDatabaseHandler>();
#endif
    
    // Create the service handler for container operations
    auto service = std::make_unique<ContainerServiceHandler>(*db);

    // Initialize project (logging, DB, etc.)
    InitProject(*db);

    // Create security provider based on CMake configuration
    std::unique_ptr<ISecurityProvider> security_provider;
#if defined(SECURITY_ALGORITHM_AES_GCM)
    security_provider = std::make_unique<AesGcmSecurityProvider>();
    std::cout << "🔑 AES-256-GCM Security Provider initialized" << std::endl;
#elif defined(SECURITY_ALGORITHM_CHACHA20)
    security_provider = std::make_unique<ChaCha20SecurityProvider>();
    std::cout << "🔑 ChaCha20-Poly1305 Security Provider initialized (Automotive OTA)" << std::endl;
#else
    security_provider = std::make_unique<NullSecurityProvider>();
    std::cout << "🔓 Null Security Provider initialized (encryption disabled)" << std::endl;
#endif

    // Create request executor with security provider reference
#if ENABLE_PROTOBUF
    auto executor = std::make_shared<ProtoRequestExecutorHandler>(*db, *service, *security_provider);
#else
    auto executor = std::make_shared<JsonRequestExecutorHandler>(*db, *service, *security_provider);
#endif

    // Register signal handlers for graceful shutdown
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);

    // Vector to hold protocol consumer threads
    std::vector<std::thread> protocol_threads;

    // Start dedicated heartbeat server in a separate thread (using config struct for uniformity)
    HeartbeatConfig heartbeat_cfg;
    auto heartbeat_server = std::make_shared<HeartbeatServer>(heartbeat_cfg);
    protocol_threads.emplace_back([heartbeat_server]() {
        heartbeat_server->Start();
    });

    // Start REST/HTTP server in a separate thread if enabled
#if ENABLE_REST
    ServerConfig server_cfg;
    auto server = std::make_shared<HttpServerHandler>(server_cfg.ThreadPoolSize, executor);
    std::string host = server_cfg.Host;
    int port = server_cfg.Port;
    protocol_threads.emplace_back([server, host, port]() {
        server->Start(host, port);
    });
#endif

    // Start MQTT subscriber in a separate thread if enabled
#if ENABLE_MQTT
    MqttConfig mqtt_cfg;
    auto mqtt_consumer = std::make_shared<MosquittoMqttSubscriber>(
        mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    protocol_threads.emplace_back([mqtt_consumer]() {
        mqtt_consumer->Start();
    });
#endif

    // Start POSIX Message Queue consumer in a separate thread if enabled
#if ENABLE_MSGQUEUE
    MessageQueueConfig mq_cfg;
    auto mq_consumer = std::make_shared<MessageQueueConsumer>(mq_cfg, executor);
    protocol_threads.emplace_back([mq_consumer]() {
        mq_consumer->Start();
    });
#endif

    // Start D-Bus consumer in a separate thread if enabled
#if ENABLE_DBUS
    DbusConfig dbus_cfg;
    auto dbus_consumer = std::make_shared<DBusConsumer>(dbus_cfg, executor);
    protocol_threads.emplace_back([dbus_consumer]() {
        dbus_consumer->Start();
    });
#endif

    // Main loop: wait for shutdown signal
    while (!shutdown_requested) {
        std::this_thread::sleep_for(std::chrono::milliseconds(kPollIntervalMs));
    }

    // Stop all protocol consumers gracefully
#if ENABLE_REST
    server->Stop();
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

    // Stop heartbeat server
    heartbeat_server->Stop();

    // Join all protocol threads to ensure clean shutdown
    for (auto& t : protocol_threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    // Shutdown logging and exit
    google::ShutdownGoogleLogging();
    return 0;
}