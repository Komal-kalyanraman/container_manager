/**
 * @file main.cpp
 * @brief Entry point for the Container Manager application.
 *
 * Initializes logging, database, and starts all enabled protocol consumers:
 * - HTTP server (REST API)
 * - MQTT subscriber
 * - POSIX Message Queue consumer
 * - D-Bus consumer (session bus)
 *
 * Protocols and data formats are enabled/disabled via CMake flags.
 */

#include <memory>
#include <thread>
#include <vector>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/init_handler.hpp"
#include "inc/json_request_executor.hpp"
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

int main() {
    InitProject();

#if ENABLE_PROTOBUF
    auto executor = std::make_shared<ProtoRequestExecutorHandler>();
#else
    auto executor = std::make_shared<JsonRequestExecutorHandler>();
#endif

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
    auto mqtt_consumer = std::make_shared<MosquittoMqttSubscriber>(
        mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    protocol_threads.emplace_back([mqtt_consumer]() {
        mqtt_consumer->Start();
    });
#endif

#if ENABLE_MSGQUEUE
    MessageQueueConfig mq_cfg;
    auto mq_consumer = std::make_shared<MessageQueueConsumer>(mq_cfg, executor);
    mq_consumer->Start();
#endif

#if ENABLE_DBUS
    DbusConfig dbus_cfg;
    auto dbus_consumer = std::make_shared<DbusConsumer>(dbus_cfg, executor);
    dbus_consumer->Start();
#endif

    // Dummy thread to keep main alive (remove when all protocols are managed elegantly)
    protocol_threads.emplace_back([]() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(60));
        }
    });

    // Wait for all protocol threads to finish
    for (auto& t : protocol_threads) {
        if (t.joinable()) t.join();
    }

    // Stop protocol consumers (if needed)
#if ENABLE_MQTT
    mqtt_consumer->Stop();
#endif
#if ENABLE_MSGQUEUE
    mq_consumer->Stop();
#endif
#if ENABLE_DBUS
    dbus_consumer->Stop();
#endif

    google::ShutdownGoogleLogging();
    return 0;
}