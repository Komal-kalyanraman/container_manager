/**
 * @file docker_commands.hpp
 * @brief Declares Docker command classes for container operations.
 */

#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

/// @class DockerRuntimeAvailableCommand
/// @brief Command to check if Docker runtime is available.
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
     * @return True if Docker is available, false otherwise.
     */
    bool Execute() const override;
};

/// @class DockerCreateContainerCommand
/// @brief Command to create a Docker container with specified parameters.
class DockerCreateContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to create.
public:
    /**
     * @brief Constructs a DockerCreateContainerCommand.
     * @param container_name_ Name of the container to create.
     */
    explicit DockerCreateContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~DockerCreateContainerCommand() = default;

    /**
     * @brief Executes the command to create a Docker container.
     * @return True if the container was created successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class DockerStartContainerCommand
/// @brief Command to start a Docker container.
class DockerStartContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to start.
public:
    /**
     * @brief Constructs a DockerStartContainerCommand.
     * @param container_name_ Name of the container to start.
     */
    explicit DockerStartContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~DockerStartContainerCommand() = default;

    /**
     * @brief Executes the command to start a Docker container.
     * @return True if the container was started successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class DockerStopContainerCommand
/// @brief Command to stop a Docker container.
class DockerStopContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to stop.
public:
    /**
     * @brief Constructs a DockerStopContainerCommand.
     * @param container_name_ Name of the container to stop.
     */
    explicit DockerStopContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~DockerStopContainerCommand() = default;

    /**
     * @brief Executes the command to stop a Docker container.
     * @return True if the container was stopped successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class DockerRemoveContainerCommand
/// @brief Command to remove a Docker container and clean up database state.
class DockerRemoveContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to remove.
public:
    /**
     * @brief Constructs a DockerRemoveContainerCommand.
     * @param container_name_ Name of the container to remove.
     */
    explicit DockerRemoveContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~DockerRemoveContainerCommand() = default;

    /**
     * @brief Executes the command to remove a Docker container and clean up database state.
     * @return True if the container was removed successfully, false otherwise.
     */
    bool Execute() const override;
};

/// @class DockerRestartContainerCommand
/// @brief Command to restart a Docker container.
class DockerRestartContainerCommand : public Command {
private:
    std::string container_name_; ///< Name of the container to restart.
public:
    /**
     * @brief Constructs a DockerRestartContainerCommand.
     * @param container_name_ Name of the container to restart.
     */
    explicit DockerRestartContainerCommand(const std::string& container_name_);

    /**
     * @brief Destructor.
     */
    ~DockerRestartContainerCommand() = default;

    /**
     * @brief Executes the command to restart a Docker container.
     * @return True if the container was restarted successfully, false otherwise.
     */
    bool Execute() const override;
};