/**
 * @file main.cpp
 * @brief Entry point for the Container Manager application.
 *
 * This file initializes logging, clears the database, starts the HTTP server and MQTT subscriber,
 * and handles application shutdown. The HTTP server listens for REST requests, while the MQTT subscriber
 * listens for messages on a configured topic. Both use a shared request executor for processing incoming data.
 */

#include <memory>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/http_server.hpp"
#include "inc/init_handler.hpp"
#include "inc/json_request_executor.hpp"
#include "inc/mosquitto_mqtt_subscriber.hpp"

/**
 * @brief Main function for the Container Manager application.
 * 
 * Initializes the project, creates the shared request executor, starts the HTTP server and MQTT subscriber,
 * and waits for the HTTP server thread to finish. Cleans up logging resources on shutdown.
 * 
 * @return int Exit status code.
 */
int main() {
    InitProject();  ///< Initialize logging and clear the database.

    // Create the shared JSON request executor
    auto executor = std::make_shared<JsonRequestExecutorHandler>();

    // HTTP server configuration
    ServerConfig server_cfg;

    // Start HTTP server in a separate thread
    HttpServerHandler server(executor, server_cfg.ThreadPoolSize);
    std::thread http_thread([&server, &server_cfg]() {
        server.Start(server_cfg.Host, server_cfg.Port);
    });

    // MQTT subscriber configuration
    MqttConfig mqtt_cfg;

    // Start MQTT subscriber (runs in its own thread internally)
    MosquittoMqttSubscriber mqtt_sub(mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    mqtt_sub.Start();

    // Wait for HTTP server thread to finish (in production, handle shutdown signals properly)
    http_thread.join();

    google::ShutdownGoogleLogging();  ///< Cleanly shutdown Google logging.
    return 0;
}