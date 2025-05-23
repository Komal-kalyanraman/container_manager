/**
 * @file docker_commands.cpp
 * @brief Implements Docker command classes for container operations.
 */

#include "inc/docker_commands.hpp"

#include <cstdlib>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/logging.hpp"

/**
 * @brief Constructs a DockerRuntimeAvailableCommand.
 */
DockerRuntimeAvailableCommand::DockerRuntimeAvailableCommand() {}

/**
 * @brief Executes the command to check Docker runtime availability.
 * @return True if Docker is available, false otherwise.
 */
bool DockerRuntimeAvailableCommand::Execute() const {
    int status = std::system("docker info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Docker is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Docker is not running";
        return false;
    }
}

/**
 * @brief Constructs a DockerCreateContainerCommand.
 * @param container_name Name of the container to create.
 * @param image_name Name of the image to use for the container.
 */
DockerCreateContainerCommand::DockerCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

/**
 * @brief Executes the command to create a Docker container.
 * @return True if the container was created successfully, false otherwise.
 */
bool DockerCreateContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Create,
        {{"runtime", "docker"}, {"name", container_name_}, {"image", image_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container created successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Docker container";
        return false;
    }
}

/**
 * @brief Constructs a DockerStartContainerCommand.
 * @param container_name Name of the container to start.
 */
DockerStartContainerCommand::DockerStartContainerCommand(const std::string& container_name) : container_name_(container_name) {}

/**
 * @brief Executes the command to start a Docker container.
 * @return True if the container was started successfully, false otherwise.
 */
bool DockerStartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Start,
        {{"runtime", "docker"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Docker container";
        return false;
    }
}

/**
 * @brief Constructs a DockerStopContainerCommand.
 * @param container_name Name of the container to stop.
 */
DockerStopContainerCommand::DockerStopContainerCommand(const std::string& container_name) : container_name_(container_name) {}

/**
 * @brief Executes the command to stop a Docker container.
 * @return True if the container was stopped successfully, false otherwise.
 */
bool DockerStopContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Stop,
        {{"runtime", "docker"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Docker container";
        return false;
    }
}

/**
 * @brief Constructs a DockerRemoveContainerCommand.
 * @param container_name Name of the container to remove.
 */
DockerRemoveContainerCommand::DockerRemoveContainerCommand(const std::string& container_name) : container_name_(container_name) {}

/**
 * @brief Executes the command to remove a Docker container.
 * @return True if the container was removed successfully, false otherwise.
 */
bool DockerRemoveContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Remove,
        {{"runtime", "docker"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container removed successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to remove Docker container";
        return false;
    }
}

/**
 * @brief Constructs a DockerRestartContainerCommand.
 * @param container_name Name of the container to restart.
 */
DockerRestartContainerCommand::DockerRestartContainerCommand(const std::string& container_name) : container_name_(container_name) {}

/**
 * @brief Executes the command to restart a Docker container.
 * @return True if the container was restarted successfully, false otherwise.
 */
bool DockerRestartContainerCommand::Execute() const {
    std::string command = FormatCommand(
        CommandTemplate::Restart,
        {{"runtime", "docker"}, {"name", container_name_}}
    );
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container restarted successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to restart Docker container";
        return false;
    }
}