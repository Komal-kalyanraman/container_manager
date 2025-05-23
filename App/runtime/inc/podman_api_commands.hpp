#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

/**
 * @file podman_api_commands.hpp
 * @brief Declares command classes for managing containers via the Podman HTTP API over a Unix socket.
 *
 * Each command class encapsulates a specific Podman API operation (e.g., check runtime, create, start, stop container).
 * These classes use CurlHandler to send HTTP requests to the Podman REST API.
 *
 * Usage:
 *   - Instantiate the desired command class with required parameters.
 *   - Call Execute() to perform the operation via the Podman API.
 */

/**
 * @class PodmanApiRuntimeAvailableCommand
 * @brief Command to check if the Podman daemon is available via the /_ping endpoint.
 */
class PodmanApiRuntimeAvailableCommand : public Command {
public:
    explicit PodmanApiRuntimeAvailableCommand();
    ~PodmanApiRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

/**
 * @class PodmanApiCreateContainerCommand
 * @brief Command to create a new Podman container using the specified image and name.
 */
class PodmanApiCreateContainerCommand : public Command {
private:
    std::string container_name_;
    std::string image_name_;
public:
    PodmanApiCreateContainerCommand(const std::string& container_name, const std::string& image_name);
    ~PodmanApiCreateContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class PodmanApiStartContainerCommand
 * @brief Command to start a Podman container by name.
 */
class PodmanApiStartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanApiStartContainerCommand(const std::string& container_name_);
    ~PodmanApiStartContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class PodmanApiStopContainerCommand
 * @brief Command to stop a Podman container by name.
 */
class PodmanApiStopContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanApiStopContainerCommand(const std::string& container_name_);
    ~PodmanApiStopContainerCommand() = default;

    bool Execute() const override;
};