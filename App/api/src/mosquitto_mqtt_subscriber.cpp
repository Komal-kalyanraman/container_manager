/**
 * @file mosquitto_mqtt_subscriber.cpp
 * @brief Implements the MosquittoMqttSubscriber class for handling MQTT messages using Mosquitto.
 */

#include "inc/mosquitto_mqtt_subscriber.hpp"
#include <iostream>
#include <nlohmann/json.hpp>
#include "inc/logging.hpp"

/**
 * @brief Constructs a MosquittoMqttSubscriber, initializes the Mosquitto library, and connects to the broker.
 * @param broker_address The MQTT broker address.
 * @param broker_port The MQTT broker port.
 * @param topic The topic to subscribe to.
 * @param executor Shared pointer to a RequestExecutor for processing messages.
 */
MosquittoMqttSubscriber::MosquittoMqttSubscriber(const std::string& broker_address,
                                                 const int& broker_port,
                                                 const std::string& topic,
                                                 std::shared_ptr<RequestExecutor> executor)
    : mosqpp::mosquittopp("container_manager_mqtt_subscriber"),
      topic_(topic),
      executor_(std::move(executor)),
      running_(false)
{
    mosqpp::lib_init();
    connect(broker_address.c_str(), broker_port, 60);
}

/**
 * @brief Destructor. Stops the subscriber and cleans up the Mosquitto library.
 */
MosquittoMqttSubscriber::~MosquittoMqttSubscriber() {
    Stop();
    mosqpp::lib_cleanup();
}

/**
 * @brief Starts the MQTT client loop in a background thread.
 *
 * Sets the running flag to true and starts the Mosquitto network loop.
 */
void MosquittoMqttSubscriber::Start() {
    running_ = true;
    loop_start();
}

/**
 * @brief Stops the MQTT client loop and disconnects from the broker.
 *
 * Sets the running flag to false, disconnects from the broker, and stops the Mosquitto network loop.
 */
void MosquittoMqttSubscriber::Stop() {
    if (running_) {
        running_ = false;
        disconnect();
        loop_stop();
    }
}

/**
 * @brief Callback for successful connection to the MQTT broker.
 * @param rc The result code of the connection attempt.
 *
 * Subscribes to the configured topic if the connection is successful.
 */
void MosquittoMqttSubscriber::on_connect(int rc) {
    if (rc == 0) {
        std::cout << "[MQTT] Connected to broker, subscribing to topic: " << topic_ << std::endl;
        subscribe(nullptr, topic_.c_str());
    } else {
        std::cerr << "[MQTT] Failed to connect, return code: " << rc << std::endl;
    }
}

/**
 * @brief Callback for receiving a message from the subscribed topic.
 * @param message The received MQTT message.
 *
 * Forwards the received message payload to the RequestExecutor for processing.
 * Logs status and errors using the application's logging macros.
 */
void MosquittoMqttSubscriber::on_message(const struct mosquitto_message* message) {
    if (message && message->payload && message->payloadlen > 0 && running_) {
        std::string payload(static_cast<char*>(message->payload), message->payloadlen);
        CM_LOG_INFO << "[MQTT] Received message on topic " << message->topic << ": " << payload << std::endl;
        try {
            auto result = executor_->Execute(payload);
            // Log the result status
            if (result.contains("status")) {
                if (result["status"] == "error") {
                    CM_LOG_ERROR << "[MQTT] Execution error: "
                                 << result.value("message", "Unknown error") << std::endl;
                } else {
                    CM_LOG_INFO << "[MQTT] Execution success." << std::endl;
                }
            } else {
                CM_LOG_WARN << "[MQTT] No status field in execution result." << std::endl;
            }
        } catch (const std::exception& e) {
            CM_LOG_ERROR << "[MQTT] Error executing request: " << e.what() << std::endl;
        }
    }
}