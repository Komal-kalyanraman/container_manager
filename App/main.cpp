/**
 * @file main.cpp
 * @brief Entry point for the Container Manager application.
 *
 * This file initializes logging, clears the database, and starts all protocol consumers:
 * - HTTP server (REST API)
 * - MQTT subscriber
 * - POSIX Message Queue consumer
 * - D-Bus consumer (session bus, for user applications)
 *
 * All protocols use a shared request executor for processing incoming requests (JSON or Protobuf).
 * The application waits for the HTTP server thread to finish and then performs a clean shutdown.
 *
 * Usage:
 *   - Initializes the project and all protocol consumers.
 *   - Handles incoming requests via REST, MQTT, Message Queue, and D-Bus.
 *   - Ensures proper shutdown and resource cleanup.
 */
#include <memory>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/http_server.hpp"
#include "inc/init_handler.hpp"
#if ENABLE_DBUS
#include "inc/dbus_consumer.hpp"
#endif
#include "inc/json_request_executor.hpp"
#if ENABLE_PROTOBUF
#include "inc/protobuf_request_executor.hpp"
#endif
#if ENABLE_MQTT
#include "inc/mosquitto_mqtt_subscriber.hpp"
#endif
#if ENABLE_MSGQUEUE
#include "inc/posix_message_queue_consumer.hpp"
#endif

int main() {
    InitProject();

    // auto executor = std::make_shared<JsonRequestExecutorHandler>();
    auto executor = std::make_shared<ProtoRequestExecutorHandler>();

    std::vector<std::thread> protocol_threads;

#if ENABLE_REST
    ServerConfig server_cfg;
    HttpServerHandler server(executor, server_cfg.ThreadPoolSize);
    protocol_threads.emplace_back([&server, &server_cfg]() {
        server.Start(server_cfg.Host, server_cfg.Port);
    });
#endif

#if ENABLE_MQTT
    MqttConfig mqtt_cfg;
    auto mqtt_thread = std::thread([&]() {
        MosquittoMqttSubscriber mqtt_sub(mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
        mqtt_sub.Start();
    });
    protocol_threads.emplace_back(std::move(mqtt_thread));
#endif

#if ENABLE_MSGQUEUE
    MessageQueueConfig mq_cfg;
    auto mq_consumer = std::make_shared<MessageQueueConsumer>(mq_cfg, executor);
    mq_consumer->Start();
    // Optionally store mq_consumer for later Stop()
#endif

#if ENABLE_DBUS
    DbusConfig dbus_cfg;
    // Keep this alive for the whole program
    auto dbus_consumer = std::make_shared<DbusConsumer>(dbus_cfg, executor);
    dbus_consumer->Start();
    // Optionally store dbus_consumer in a vector if you want to call Stop() on shutdown
#endif

    std::thread dummy_thread([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }
    });
    protocol_threads.emplace_back(std::move(dummy_thread));

    // Wait for all protocol threads to finish
    for (auto& t : protocol_threads) {
        if (t.joinable()) t.join();
    }

    // dbus_consumer->Stop();
    google::ShutdownGoogleLogging();
    return 0;
}