/**
 * @file podman_commands.hpp
 * @brief Declares Podman command classes for container operations.
 *
 * These classes encapsulate Podman CLI operations such as create, start, stop, remove, and restart,
 * using parameters provided in the ContainerRequest structure. Each command provides standardized
 * error handling and logging, and returns a Status object indicating the result of the operation.
 */

#pragma once

#include <string>
#include <memory>
#include "inc/status.hpp"
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

/**
 * @class PodmanRuntimeAvailableCommand
 * @brief Command to check if Podman runtime is available.
 */
class PodmanRuntimeAvailableCommand : public Command {
public:
    /**
     * @brief Constructs a PodmanRuntimeAvailableCommand.
     */
    explicit PodmanRuntimeAvailableCommand();

    /**
     * @brief Destructor.
     */
    ~PodmanRuntimeAvailableCommand() = default;

    /**
     * @brief Executes the command to check Podman runtime availability.
     * @return Status indicating if Podman is running.
     */
    Status Execute() const override;
};

/**
 * @class PodmanCreateContainerCommand
 * @brief Command to create a Podman container with user-defined parameters.
 */
class PodmanCreateContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container creation parameters.
public:
    /**
     * @brief Constructs a PodmanCreateContainerCommand.
     * @param req The container request containing all parameters for creation.
     */
    explicit PodmanCreateContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Podman container.
     * @return Status indicating if the container was created successfully.
     */
    Status Execute() const override;
};

/**
 * @class PodmanStartContainerCommand
 * @brief Command to start a Podman container.
 */
class PodmanStartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container start parameters.
public:
    /**
     * @brief Constructs a PodmanStartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanStartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Podman container.
     * @return Status indicating if the container was started successfully.
     */
    Status Execute() const override;
};

/**
 * @class PodmanStopContainerCommand
 * @brief Command to stop a Podman container.
 */
class PodmanStopContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container stop parameters.
public:
    /**
     * @brief Constructs a PodmanStopContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanStopContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Podman container.
     * @return Status indicating if the container was stopped successfully.
     */
    Status Execute() const override;
};

/**
 * @class PodmanRemoveContainerCommand
 * @brief Command to remove a Podman container.
 */
class PodmanRemoveContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container remove parameters.
public:
    /**
     * @brief Constructs a PodmanRemoveContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanRemoveContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanRemoveContainerCommand() = default;

    /**
     * @brief Executes the command to remove a Podman container.
     * @return Status indicating if the container was removed successfully.
     */
    Status Execute() const override;
};

/**
 * @class PodmanRestartContainerCommand
 * @brief Command to restart a Podman container.
 */
class PodmanRestartContainerCommand : public Command {
private:
    ContainerRequest req_; ///< Container restart parameters.
public:
    /**
     * @brief Constructs a PodmanRestartContainerCommand.
     * @param req The container request containing the container name.
     */
    explicit PodmanRestartContainerCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanRestartContainerCommand() = default;

    /**
     * @brief Executes the command to restart a Podman container.
     * @return Status indicating if the container was restarted successfully.
     */
    Status Execute() const override;
};