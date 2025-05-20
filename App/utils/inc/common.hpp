/**
 * @file common.hpp
 * @brief Defines common constants, string views, and utility structures for the application.
 */

#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

/// @brief Represents an empty string constant.
const std::string kEmptyString = "";

/// @brief Name used for container manager in google logging.
const std::string kContainerManagerLogName = "cm_log";

/// @struct ServerConfig
/// @brief Holds constant configuration for the HTTP server.
struct ServerConfig {
    const int Port = 5000;
    const std::string Host = "0.0.0.0";
    const int ThreadPoolSize = 10;
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

/// @struct DbusConfig
/// @brief Holds constant configuration for the D-Bus consumer (session bus).
struct DbusConfig {
    const std::string BusName = "org.container.manager";         ///< D-Bus service name.
    const std::string ObjectPath = "/org/container/manager";     ///< D-Bus object path.
    const std::string Interface = "org.container.manager";       ///< D-Bus interface name.
    const std::string Method = "Execute";                        ///< D-Bus method name for JSON execution.
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

/// @namespace CommandTemplate
/// @brief Contains command templates for supported container operations.
///        Placeholders (e.g., {runtime}, {name}, {image}) are replaced at runtime.
namespace CommandTemplate {
    inline constexpr std::string_view Create   = "{runtime} create --name {name} {image}";
    inline constexpr std::string_view Start    = "{runtime} start {name}";
    inline constexpr std::string_view Stop     = "{runtime} stop {name}";
    inline constexpr std::string_view Remove   = "{runtime} rm -f {name}";
    inline constexpr std::string_view Restart  = "{runtime} restart {name}";
}

/// @brief Utility function to format command templates by replacing placeholders with actual values.
/// @param tmpl The command template string with placeholders.
/// @param values A map of placeholder names to their replacement values.
inline std::string FormatCommand(std::string_view tmpl, const std::unordered_map<std::string, std::string>& values) {
    std::string result(tmpl);
    for (const auto& [key, val] : values) {
        std::string placeholder = "{" + key + "}";
        size_t pos = 0;
        while ((pos = result.find(placeholder, pos)) != std::string::npos) {
            result.replace(pos, placeholder.length(), val);
            pos += val.length();
        }
    }
    return result;
}