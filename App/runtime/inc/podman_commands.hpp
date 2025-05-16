/**
 * @file podman_commands.hpp
 * @brief Declares Podman command classes for container operations.
 *
 * @details This file contains the declarations of command classes for Podman container operations,
 * including creation, removal, starting, stopping, restarting, and runtime availability checks.
 */

#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

/// @class PodmanRuntimeAvailableCommand
/// @brief Command to check if Podman runtime is available.
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
     * @return True if Podman is available, false otherwise.
     */
    bool Execute() const override;
};

/// @class PodmanCreateContainerCommand
/// @brief Command to create a Podman container with specified parameters.
class PodmanCreateContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to create.
public:
    /**
     * @brief Constructs a PodmanCreateContainerCommand.
     * @param container_name_ Name of the container to create.
     */
    explicit PodmanCreateContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~PodmanCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Podman container.
     * @return True if the container was created successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class PodmanStartContainerCommand
/// @brief Command to start a Podman container.
class PodmanStartContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to start.
public:
    /**
     * @brief Constructs a PodmanStartContainerCommand.
     * @param container_name_ Name of the container to start.
     */
    explicit PodmanStartContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~PodmanStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Podman container.
     * @return True if the container was started successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class PodmanStopContainerCommand
/// @brief Command to stop a Podman container.
class PodmanStopContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to stop.
public:
    /**
     * @brief Constructs a PodmanStopContainerCommand.
     * @param container_name_ Name of the container to stop.
     */
    explicit PodmanStopContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~PodmanStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Podman container.
     * @return True if the container was stopped successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class PodmanRemoveContainerCommand
/// @brief Command to remove a Podman container and clean up database state.
class PodmanRemoveContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to remove.
public:
    /**
     * @brief Constructs a PodmanRemoveContainerCommand.
     * @param container_name_ Name of the container to remove.
     */
    explicit PodmanRemoveContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~PodmanRemoveContainerCommand() = default;

    /**
     * @brief Executes the command to remove a Podman container and clean up database state.
     * @return True if the container was removed successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class PodmanRestartContainerCommand
/// @brief Command to restart a Podman container.
class PodmanRestartContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to restart.
public:
    /**
     * @brief Constructs a PodmanRestartContainerCommand.
     * @param container_name_ Name of the container to restart.
     */
    explicit PodmanRestartContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~PodmanRestartContainerCommand() = default;

    /**
     * @brief Executes the command to restart a Podman container.
     * @return True if the container was restarted successfully, false otherwise.
     */
    bool Execute() const override;
};