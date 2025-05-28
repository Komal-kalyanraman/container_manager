/**
 * @file docker_api_commands.hpp
 * @brief Declares command classes for managing containers via the Docker HTTP API over a Unix socket.
 *
 * Each command class encapsulates a specific Docker API operation (e.g., check runtime, create, start, stop, restart, remove container).
 * These classes use CurlHandler to send HTTP requests to the Docker REST API, using parameters from the ContainerRequest structure.
 * All commands return a Status object for standardized error handling.
 */

#pragma once

#include <string>
#include <memory>
#include "inc/status.hpp"
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

/**
 * @class DockerApiRuntimeAvailableCommand
 * @brief Command to check if Docker daemon is available via the API.
 */
class DockerApiRuntimeAvailableCommand : public Command {
public:
    /**
     * @brief Constructs a DockerApiRuntimeAvailableCommand.
     */
    explicit DockerApiRuntimeAvailableCommand();

    /**
     * @brief Destructor.
     */
    ~DockerApiRuntimeAvailableCommand() = default;

    /**
     * @brief Executes the command to check Docker API runtime availability.
     * @return Status indicating if Docker is running.
     */
    Status Execute() const override;
};

/**
 * @class DockerApiCreateContainerCommand
 * @brief Command to create a Docker container via the Docker API.
 */
class DockerApiCreateContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container creation parameters.
public:
    /**
     * @brief Constructs a DockerApiCreateContainerCommand.
     * @param req The container request containing all parameters for creation.
     */
    explicit DockerApiCreateContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerApiCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Docker container via the API.
     * @return Status indicating if the container was created successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerApiStartContainerCommand
 * @brief Command to start a Docker container via the Docker API.
 */
class DockerApiStartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container start parameters.
public:
    /**
     * @brief Constructs a DockerApiStartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerApiStartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerApiStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Docker container via the API.
     * @return Status indicating if the container was started successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerApiStopContainerCommand
 * @brief Command to stop a Docker container via the Docker API.
 */
class DockerApiStopContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container stop parameters.
public:
    /**
     * @brief Constructs a DockerApiStopContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerApiStopContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerApiStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Docker container via the API.
     * @return Status indicating if the container was stopped successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerApiRestartContainerCommand
 * @brief Command to restart a Docker container via the Docker API.
 */
class DockerApiRestartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container restart parameters.
public:
    /**
     * @brief Constructs a DockerApiRestartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerApiRestartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerApiRestartContainerCommand() = default;

    /**
     * @brief Executes the command to restart a Docker container via the API.
     * @return Status indicating if the container was restarted successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerApiRemoveContainerCommand
 * @brief Command to remove a Docker container via the Docker API.
 */
class DockerApiRemoveContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container remove parameters.
public:
    /**
     * @brief Constructs a DockerApiRemoveContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerApiRemoveContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerApiRemoveContainerCommand() = default;

    /**
     * @brief Executes the command to remove a Docker container via the API.
     * @return Status indicating if the container was removed successfully.
     */
    Status Execute() const override;
};