/**
 * @file mosquitto_mqtt_subscriber.hpp
 * @brief Declares the MosquittoMqttSubscriber class for handling MQTT messages using Mosquitto.
 */

#pragma once

#include <mosquittopp.h>
#include <memory>
#include <string>
#include "inc/request_executor.hpp"

/// @class MosquittoMqttSubscriber
/// @brief Subscribes to an MQTT topic and dispatches received messages to a RequestExecutor.
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
                            int broker_port,
                            const std::string& topic,
                            std::shared_ptr<RequestExecutor> executor);

    /**
     * @brief Destructor.
     */
    ~MosquittoMqttSubscriber();

    /**
     * @brief Starts the MQTT client loop.
     */
    void Start();

    /**
     * @brief Stops the MQTT client loop.
     */
    void Stop();

protected:
    void on_connect(int rc) override;
    void on_message(const struct mosquitto_message* message) override;

private:
    std::string topic_;
    std::shared_ptr<RequestExecutor> executor_;
    bool running_;
};