/**
 * @file docker_commands.hpp
 * @brief Declares Docker command classes for container operations.
 *
 * These classes encapsulate Docker CLI operations such as create, start, stop, remove, and restart,
 * using parameters provided in the ContainerRequest structure. Each command provides standardized
 * error handling and logging, and returns a Status object indicating the result of the operation.
 */

#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

/**
 * @class DockerRuntimeAvailableCommand
 * @brief Command to check if Docker runtime is available.
 */
class DockerRuntimeAvailableCommand : public Command {
public:
    /**
     * @brief Constructs a DockerRuntimeAvailableCommand.
     */
    explicit DockerRuntimeAvailableCommand();

    /**
     * @brief Destructor.
     */
    ~DockerRuntimeAvailableCommand() = default;

    /**
     * @brief Executes the command to check Docker runtime availability.
     * @return Status indicating if Docker is running.
     */
    Status Execute() const override;
};

/**
 * @class DockerCreateContainerCommand
 * @brief Command to create a Docker container with user-defined parameters.
 */
class DockerCreateContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container creation parameters.
public:
    /**
     * @brief Constructs a DockerCreateContainerCommand.
     * @param req The container request containing all parameters for creation.
     */
    explicit DockerCreateContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Docker container.
     * @return Status indicating if the container was created successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerStartContainerCommand
 * @brief Command to start a Docker container.
 */
class DockerStartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container start parameters.
public:
    /**
     * @brief Constructs a DockerStartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerStartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Docker container.
     * @return Status indicating if the container was started successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerStopContainerCommand
 * @brief Command to stop a Docker container.
 */
class DockerStopContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container stop parameters.
public:
    /**
     * @brief Constructs a DockerStopContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerStopContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Docker container.
     * @return Status indicating if the container was stopped successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerRemoveContainerCommand
 * @brief Command to remove a Docker container.
 */
class DockerRemoveContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container remove parameters.
public:
    /**
     * @brief Constructs a DockerRemoveContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerRemoveContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerRemoveContainerCommand() = default;

    /**
     * @brief Executes the command to remove a Docker container.
     * @return Status indicating if the container was removed successfully.
     */
    Status Execute() const override;
};

/**
 * @class DockerRestartContainerCommand
 * @brief Command to restart a Docker container.
 */
class DockerRestartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container restart parameters.
public:
    /**
     * @brief Constructs a DockerRestartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit DockerRestartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~DockerRestartContainerCommand() = default;

    /**
     * @brief Executes the command to restart a Docker container.
     * @return Status indicating if the container was restarted successfully.
     */
    Status Execute() const override;
};