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

/// @brief Sleep interval (in milliseconds) for main thread shutdown polling.
constexpr int kMainShutdownPollMs = 200;

/// @brief Docker Unix socket path and API base URL for HTTP API communication.
inline constexpr char kDockerUnixSocketPath[] = "/var/run/docker.sock";
inline constexpr char kDockerApiBaseUrl[] = "http://localhost/v1.49/";

/// @brief Podman Unix socket path and API base URL for HTTP API communication.
inline constexpr char kPodmanUnixSocketPath[] = "/run/user/1000/podman/podman.sock";
inline constexpr char kPodmanApiBaseUrl[] = "http://d/v3.0.0/";

/// @struct ServerConfig
/// @brief Holds constant configuration for the HTTP server.
struct ServerConfig {
    const int Port = 5000;                ///< HTTP server port.
    const std::string Host = "0.0.0.0";   ///< HTTP server host address.
    const int ThreadPoolSize = 10;        ///< Number of threads in the thread pool.
};

/// @struct MqttConfig
/// @brief Holds constant configuration for the MQTT subscriber.
struct MqttConfig {
    const std::string BrokerAddress = "localhost";                ///< MQTT broker address.
    const int BrokerPort = 1883;                                  ///< MQTT broker port.
    const std::string Topic = "container/execute";                ///< MQTT topic to subscribe to.
    const std::string ClientId = "container_manager_mqtt_subscriber"; ///< MQTT client ID.
    const std::string ClearRetainedClientId = "cm_clear_retained";    ///< MQTT client ID for clearing retained messages.
};

/// @struct MessageQueueConfig
/// @brief Holds constant configuration for the POSIX message queue consumer.
struct MessageQueueConfig {
    const std::string QueueName = "/container_manager_queue"; ///< Name of the POSIX message queue.
    const long MaxMsgSize = 8192;      ///< Maximum size of each message (bytes).
    const long MaxMsgCount = 10;       ///< Maximum number of messages in the queue.
    const long SleepTime = 10000;      ///< Sleep time in microseconds.
    const int ReceiveTimeoutSec = 1;   ///< Timeout for mq_timedreceive in seconds.
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
    inline constexpr std::string_view Create =
        "{runtime} create --name {name} "
        "--cpus {cpus} --memory {memory} --pids-limit {pids} "
        "--restart {restart_policy} {image}";
    inline constexpr std::string_view Start    = "{runtime} start {name}";
    inline constexpr std::string_view Stop     = "{runtime} stop {name}";
    inline constexpr std::string_view Remove   = "{runtime} rm -f {name}";
    inline constexpr std::string_view Restart  = "{runtime} restart {name}";
}

/// @brief Utility function to format command templates by replacing placeholders with actual values.
/// @param tmpl The command template string with placeholders.
/// @param values A map of placeholder names to their replacement values.
/// @return The formatted command string.
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

namespace ApiEndpoint {
    inline constexpr std::string_view Containers = "containers/";
    inline constexpr std::string_view Create     = "containers/create?name=";
    inline constexpr std::string_view Start      = "/start";
    inline constexpr std::string_view Stop       = "/stop";
    inline constexpr std::string_view Restart    = "/restart";
    inline constexpr std::string_view Remove     = "?force=true";
    inline constexpr std::string_view Ping       = "_ping";
}

/// @brief Compose a container API endpoint for Docker or Podman API.
/// @param baseUrl The API base URL (e.g., kDockerApiBaseUrl or kPodmanApiBaseUrl).
/// @param containerName The container name.
/// @param action The action endpoint (e.g., ApiEndpoint::Start, ApiEndpoint::Stop).
/// @return The full endpoint URL as a std::string.
inline std::string ComposeContainerApiEndpoint(
    std::string_view baseUrl,
    std::string_view containerName,
    std::string_view action)
{
    return std::string(baseUrl) + std::string(ApiEndpoint::Containers) +
           std::string(containerName) + std::string(action);
}