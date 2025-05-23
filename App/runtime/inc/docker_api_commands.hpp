#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

/**
 * @file docker_api_commands.hpp
 * @brief Declares command classes for managing containers via the Docker HTTP API over a Unix socket.
 *
 * Each command class encapsulates a specific Docker API operation (e.g., check runtime, create, start, stop, restart, remove container).
 * These classes use CurlHandler to send HTTP requests to the Docker REST API.
 *
 * Usage:
 *   - Instantiate the desired command class with required parameters.
 *   - Call Execute() to perform the operation via the Docker API.
 */

/**
 * @class DockerApiRuntimeAvailableCommand
 * @brief Command to check if the Docker daemon is available via the /_ping endpoint.
 */
class DockerApiRuntimeAvailableCommand : public Command {
public:
    explicit DockerApiRuntimeAvailableCommand();
    ~DockerApiRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

/**
 * @class DockerApiCreateContainerCommand
 * @brief Command to create a new Docker container using the specified image and name.
 */
class DockerApiCreateContainerCommand : public Command {
private:
    std::string container_name_;
    std::string image_name_;
public:
    explicit DockerApiCreateContainerCommand(const std::string& container_name, const std::string& image_name);
    ~DockerApiCreateContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class DockerApiStartContainerCommand
 * @brief Command to start a Docker container by name.
 */
class DockerApiStartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiStartContainerCommand(const std::string& container_name_);
    ~DockerApiStartContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class DockerApiStopContainerCommand
 * @brief Command to stop a Docker container by name.
 */
class DockerApiStopContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiStopContainerCommand(const std::string& container_name_);
    ~DockerApiStopContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class DockerApiRestartContainerCommand
 * @brief Command to restart a Docker container by name.
 */
class DockerApiRestartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiRestartContainerCommand(const std::string& container_name_);
    ~DockerApiRestartContainerCommand() = default;

    bool Execute() const override;
};

/**
 * @class DockerApiRemoveContainerCommand
 * @brief Command to remove a Docker container by name, using force removal.
 */
class DockerApiRemoveContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiRemoveContainerCommand(const std::string& container_name_);
    ~DockerApiRemoveContainerCommand() = default;

    bool Execute() const override;
};