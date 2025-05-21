/**
 * @file docker_commands.cpp
 * @brief Implements Docker command classes for container operations.
 */

#include "inc/docker_commands.hpp"

#include <cstdlib>
#include <nlohmann/json.hpp>

#include "inc/common.hpp"
#include "inc/logging.hpp"

DockerRuntimeAvailableCommand::DockerRuntimeAvailableCommand() {
    // Empty constructor
}

bool DockerRuntimeAvailableCommand::Execute() const {
    // Check if Docker is available
    int status = std::system("docker info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Docker is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Docker is not running";
        return false;
    }
}

DockerCreateContainerCommand::DockerCreateContainerCommand(const std::string& container_name, const std::string& image_name)
    : container_name_(container_name), image_name_(image_name) {}

bool DockerCreateContainerCommand::Execute() const {
    // Start the Docker container
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

DockerStartContainerCommand::DockerStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerStartContainerCommand::Execute() const {
    // Start the Docker container
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

DockerStopContainerCommand::DockerStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerStopContainerCommand::Execute() const {
    // Stop the Docker container
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

DockerRemoveContainerCommand::DockerRemoveContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerRemoveContainerCommand::Execute() const {
    // Stop the Docker container
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

DockerRestartContainerCommand::DockerRestartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool DockerRestartContainerCommand::Execute() const {
    // Stop the Docker container
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
