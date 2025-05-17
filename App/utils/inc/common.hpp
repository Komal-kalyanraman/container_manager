/**
 * @file common.hpp
 * @brief Defines common constants, string views, and utility structures for the application.
 */

#pragma once

#include <string>
#include <string_view>

/// @brief Represents an empty string constant.
const std::string kEmptyString = "";

/// @brief Name used for container manager in google logging.
const std::string kContainerManagerLogName = "cm_log";

/// @struct ServerConfig
/// @brief Holds constant configuration for the HTTP server.
struct ServerConfig {
    const int Port = 5000;
    const std::string Host = "0.0.0.0";
    const int ThreadPoolSize = 4;
};

/// @struct MqttConfig
/// @brief Holds constant configuration for the MQTT subscriber.
struct MqttConfig {
    const std::string BrokerAddress = "localhost";
    const int BrokerPort = 1883;
    const std::string Topic = "container/execute";
    const std::string ClientId = "container_manager_mqtt_subscriber";
    const std::string ClearRetainedClientId = "cm_clear_retained";
};

/// @struct MessageQueueConfig
/// @brief Holds constant configuration for the POSIX message queue consumer.
struct MessageQueueConfig {
    const std::string QueueName = "/container_manager_queue";
    const long MaxMsgSize = 8192;      ///< Maximum size of each message (bytes)
    const long MaxMsgCount = 10;       ///< Maximum number of messages in the queue
    const long SleepTime = 10000;        ///< Sleep time in microseconds
};

/// @struct RuntimeName
/// @brief Contains string constants for supported container runtimes.
struct RuntimeName {
    static constexpr std::string_view Docker      = "docker";       ///< Docker runtime identifier.
    static constexpr std::string_view Podman      = "podman";       ///< Podman runtime identifier.
    static constexpr std::string_view DockerApi   = "docker-api";   ///< Docker API runtime identifier.
    static constexpr std::string_view PodmanApi   = "podman-api";   ///< Podman API runtime identifier.
};

/// @struct CommandName
/// @brief Contains string constants for supported container operations.
struct CommandName {
    static constexpr std::string_view RuntimeStatus      = "available"; ///< Command to check runtime status.
    static constexpr std::string_view CreateContainer    = "create";    ///< Command to create a container.
    static constexpr std::string_view StartContainer     = "start";     ///< Command to start a container.
    static constexpr std::string_view StopContainer      = "stop";      ///< Command to stop a container.
    static constexpr std::string_view RestartContainer   = "restart";   ///< Command to restart a container.
    static constexpr std::string_view RemoveContainer    = "remove";    ///< Command to remove a container.
};