#include "inc/cm_docker_commands.hpp"

#include <cstdlib>

#include "inc/cm_common.hpp"
#include "inc/cm_logging.hpp"

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

DockerCreateContainerCommand::DockerCreateContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool DockerCreateContainerCommand::Execute() const {
    // Start the Docker container
    std::string command = "docker create " + app_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container created successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to create Docker container";
        return false;
    }
}

DockerStartContainerCommand::DockerStartContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool DockerStartContainerCommand::Execute() const {
    // Start the Docker container
    std::string command = "docker start " + app_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Docker container";
        return false;
    }
}

DockerStopContainerCommand::DockerStopContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool DockerStopContainerCommand::Execute() const {
    // Stop the Docker container
    std::string command = "docker stop " + app_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Docker container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Docker container";
        return false;
    }
}
