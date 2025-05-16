/**
 * @file common.hpp
 * @brief Defines common constants, string views, and utility structures for the application.
 */

#pragma once

#include <string>
#include <string_view>

/// @brief Maximum number of threads in the thread pool.
const int kMaxThreadPoolSize = 4;

/// @brief Name used for container manager in google logging.
const std::string kContainerManagerLogName = "cm_log";

/// @brief Default HTTP server port.
const int kHttpServerPort = 5000;

/// @brief Default HTTP server host address.
const std::string kHttpServerHost = "0.0.0.0";

/// @brief Represents an empty string constant.
const std::string kEmptyString = "";

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