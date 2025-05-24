#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

/**
 * @file podman_api_commands.hpp
 * @brief Declares command classes for managing containers via the Podman HTTP API over a Unix socket.
 *
 * Each command class encapsulates a specific Podman API operation (e.g., check runtime, create, start, stop container).
 * These classes use CurlHandler to send HTTP requests to the Podman REST API, using parameters from the ContainerRequest structure.
 */

/**
 * @class PodmanApiRuntimeAvailableCommand
 * @brief Command to check if Podman daemon is available via the API.
 */
class PodmanApiRuntimeAvailableCommand : public Command {
public:
    /**
     * @brief Constructs a PodmanApiRuntimeAvailableCommand.
     */
    explicit PodmanApiRuntimeAvailableCommand();

    /**
     * @brief Destructor.
     */
    ~PodmanApiRuntimeAvailableCommand() = default;

    /**
     * @brief Executes the command to check Podman API runtime availability.
     * @return True if Podman is running, false otherwise.
     */
    bool Execute() const override;
};

/**
 * @class PodmanApiCreateContainerCommand
 * @brief Command to create a Podman container via the Podman API.
 */
class PodmanApiCreateContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container creation parameters.
public:
    /**
     * @brief Constructs a PodmanApiCreateContainerCommand.
     * @param req The container request containing all parameters for creation.
     */
    explicit PodmanApiCreateContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanApiCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Podman container via the API.
     * @return True if the container was created successfully, false otherwise.
     */
    bool Execute() const override;
};

/**
 * @class PodmanApiStartContainerCommand
 * @brief Command to start a Podman container via the Podman API.
 */
class PodmanApiStartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container start parameters.
public:
    /**
     * @brief Constructs a PodmanApiStartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanApiStartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanApiStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Podman container via the API.
     * @return True if the container was started successfully, false otherwise.
     */
    bool Execute() const override;
};

/**
 * @class PodmanApiStopContainerCommand
 * @brief Command to stop a Podman container via the Podman API.
 */
class PodmanApiStopContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container stop parameters.
public:
    /**
     * @brief Constructs a PodmanApiStopContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanApiStopContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanApiStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Podman container via the API.
     * @return True if the container was stopped successfully, false otherwise.
     */
    bool Execute() const override;
};