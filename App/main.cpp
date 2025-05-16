/**
 * @file main.cpp
 * @brief Entry point for the Container Manager application.
 *
 * Initializes logging, clears the database, starts the HTTP server and MQTT subscriber, and handles shutdown.
 */

#include <memory>
#include <glog/logging.h>

#include "inc/common.hpp"
#include "inc/http_server.hpp"
#include "inc/init_handler.hpp"
#include "inc/json_request_executor.hpp"
#include "inc/mosquitto_mqtt_subscriber.hpp"

int main() {
    InitProject();                                                      ///< Initialize logging and clear the database.
    auto executor = std::make_shared<JsonRequestExecutorHandler>();     ///< Create the JSON request executor.

    ServerConfig server_cfg;
    
    // Start HTTP server
    HttpServerHandler server(executor, server_cfg.ThreadPoolSize);      ///< Create the HTTP server handler.
    std::thread http_thread([&server, &server_cfg]() {
        server.Start(server_cfg.Host, server_cfg.Port);                                  ///< Start the HTTP server.
    });

    MqttConfig mqtt_cfg;
    
    // Start MQTT subscriber
    MosquittoMqttSubscriber mqtt_sub(mqtt_cfg.BrokerAddress, mqtt_cfg.BrokerPort, mqtt_cfg.Topic, executor);
    mqtt_sub.Start();

    // Wait for HTTP server thread to finish (in production, handle shutdown signals properly)
    http_thread.join();

    google::ShutdownGoogleLogging();                                    ///< Cleanly shutdown Google logging.
    return 0;
}