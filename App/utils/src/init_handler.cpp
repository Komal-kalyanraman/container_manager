/**
 * @file init_handler.cpp
 * @brief Implements project-wide initialization functions.
 */

#include "inc/init_handler.hpp"

#include <glog/logging.h>
#include <mqueue.h>
#if ENABLE_MQTT
#include <mosquitto.h>
#endif

#include "inc/common.hpp"
#include "inc/redis_database.hpp"

void InitLogging() {
    google::InitGoogleLogging(kContainerManagerLogName.c_str());
}

void InitDatabase() {
    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    db.ClearDatabase();
}

void InitMessageQueue() {
    MessageQueueConfig mq_cfg;
    mq_unlink(mq_cfg.QueueName.c_str());
}

#if ENABLE_MQTT
void InitMqttRetainedMessages() {
    MqttConfig mqtt_cfg;
    mosquitto_lib_init();
    struct mosquitto *mosq = mosquitto_new(mqtt_cfg.ClearRetainedClientId.c_str(), true, nullptr);
    if (mosq) {
        mosquitto_connect(mosq, mqtt_cfg.BrokerAddress.c_str(), mqtt_cfg.BrokerPort, 60);
        mosquitto_publish(mosq, nullptr, mqtt_cfg.Topic.c_str(), 0, nullptr, 0, true);
        mosquitto_disconnect(mosq);
        mosquitto_destroy(mosq);
    }
    mosquitto_lib_cleanup();
}
#else
void InitMqttRetainedMessages() {}
#endif

void InitProject() {
    InitLogging();
    InitDatabase();
    InitMessageQueue();
    InitMqttRetainedMessages();
}