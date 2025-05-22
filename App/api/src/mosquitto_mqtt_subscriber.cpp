/**
 * @file mosquitto_mqtt_subscriber.cpp
 * @brief Implements the MosquittoMqttSubscriber class for handling MQTT messages using Mosquitto.
 */

#include "inc/mosquitto_mqtt_subscriber.hpp"
#include <iostream>
#include <nlohmann/json.hpp>

/**
 * @brief Constructs a MosquittoMqttSubscriber and connects to the MQTT broker.
 * @param broker_address The MQTT broker address.
 * @param broker_port The MQTT broker port.
 * @param topic The topic to subscribe to.
 * @param executor Shared pointer to a RequestExecutor for processing messages.
 */
MosquittoMqttSubscriber::MosquittoMqttSubscriber(const std::string& broker_address,
                                                 int broker_port,
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
 * @brief Starts the MQTT client loop.
 */
void MosquittoMqttSubscriber::Start() {
    running_ = true;
    loop_start();
}

/**
 * @brief Stops the MQTT client loop.
 */
void MosquittoMqttSubscriber::Stop() {
    if (running_) {
        loop_stop();
        running_ = false;
    }
}

/**
 * @brief Callback for successful connection to the MQTT broker.
 * @param rc The result code of the connection attempt.
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
 */
void MosquittoMqttSubscriber::on_message(const struct mosquitto_message* message) {
    if (message && message->payload && message->payloadlen > 0) {
        std::string payload(static_cast<char*>(message->payload), message->payloadlen);
        std::cout << "[MQTT] Received message on topic " << message->topic << ": " << payload << std::endl;
        try {
            executor_->Execute(payload);
        } catch (const std::exception& e) {
            std::cerr << "[MQTT] Error executing request: " << e.what() << std::endl;
        }
    }
}