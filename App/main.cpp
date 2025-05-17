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
 * All protocols use a shared request executor for processing incoming JSON requests.
 * The application waits for the HTTP server thread to finish and then performs a clean shutdown.
 */

#include <memory>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/http_server.hpp"
#include "inc/init_handler.hpp"
#include "inc/dbus_consumer.hpp"
#include "inc/json_request_executor.hpp"
#include "inc/mosquitto_mqtt_subscriber.hpp"
#include "inc/posix_message_queue_consumer.hpp"

/**
 * @brief Main function for the Container Manager application.
 * 
 * Initializes the project, creates the shared request executor, starts the HTTP server, MQTT subscriber,
 * POSIX Message Queue consumer, and D-Bus consumer (on the session bus for user applications).
 * Waits for the HTTP server thread to finish. Cleans up logging resources on shutdown.
 * 
 * @return int Exit status code.
 */
int main() {
    InitProject();  ///< Initialize and clear the database, protocols.

    // Create the shared JSON request executor
    auto executor = std::make_shared<JsonRequestExecutorHandler>();

    // HTTP server configuration and startup (runs in a separate thread)
    ServerConfig server_cfg;
    HttpServerHandler server(executor, server_cfg.ThreadPoolSize);
    std::thread http_thread([&server, &server_cfg]() {
        server.Start(server_cfg.Host, server_cfg.Port);
    });

    // MQTT subscriber configuration and startup (runs in its own thread)
    MqttConfig mqtt_cfg;
    MosquittoMqttSubscriber mqtt_sub(mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    mqtt_sub.Start();

    // POSIX Message Queue consumer configuration and startup
    MessageQueueConfig mq_cfg;
    MessageQueueConsumer mq_consumer(mq_cfg, executor);
    mq_consumer.Start();

    // D-Bus consumer configuration and startup (session bus for user applications)
    DbusConfig dbus_cfg;
    DbusConsumer dbus_consumer(dbus_cfg, executor);
    dbus_consumer.Start();

    // Wait for HTTP server thread to finish (in production, handle shutdown signals properly)
    http_thread.join();

    // Clean shutdown of all protocol consumers and logging
    mq_consumer.Stop();                 ///< Stop the message queue consumer.
    dbus_consumer.Stop();               ///< Stop the dbus consumer.
    google::ShutdownGoogleLogging();    ///< Cleanly shutdown Google logging.
    return 0;
}