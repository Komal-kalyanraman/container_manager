/**
 * @file mosquitto_mqtt_subscriber.hpp
 * @brief Declares the MosquittoMqttSubscriber class for handling MQTT messages using Mosquitto.
 */

#pragma once

#include <mosquittopp.h>
#include <memory>
#include <string>
#include <atomic>
#include "inc/request_executor.hpp"

/**
 * @class MosquittoMqttSubscriber
 * @brief Subscribes to an MQTT topic and dispatches received messages to a RequestExecutor.
 *
 * This class manages the connection to an MQTT broker, subscribes to a specified topic,
 * and forwards received messages to the provided RequestExecutor for processing.
 * It supports starting and stopping the MQTT client loop and handles graceful shutdown.
 */
class MosquittoMqttSubscriber : public mosqpp::mosquittopp {
public:
    /**
     * @brief Constructs a MosquittoMqttSubscriber.
     * @param broker_address The MQTT broker address.
     * @param broker_port The MQTT broker port.
     * @param topic The topic to subscribe to.
     * @param executor Shared pointer to a RequestExecutor for processing messages.
     */
    MosquittoMqttSubscriber(const std::string& broker_address,
                            const int& broker_port,
                            const std::string& topic,
                            std::shared_ptr<RequestExecutor> executor);

    /**
     * @brief Destructor. Stops the MQTT client loop and cleans up resources.
     */
    ~MosquittoMqttSubscriber();

    /**
     * @brief Starts the MQTT client loop in a background thread.
     *
     * Connects to the broker and begins processing incoming messages.
     */
    void Start();

    /**
     * @brief Stops the MQTT client loop and disconnects from the broker.
     *
     * Signals the client to stop processing messages and performs a clean disconnect.
     */
    void Stop();

protected:
    /**
     * @brief Callback for successful connection to the MQTT broker.
     * @param rc The result code of the connection attempt.
     */
    void on_connect(int rc) override;

    /**
     * @brief Callback for receiving a message from the subscribed topic.
     * @param message The received MQTT message.
     */
    void on_message(const struct mosquitto_message* message) override;

private:
    std::string topic_;                             ///< The MQTT topic to subscribe to.
    std::shared_ptr<RequestExecutor> executor_;     ///< Shared pointer to the request executor.
    std::atomic<bool> running_{false};              ///< Indicates if the subscriber is running.
};