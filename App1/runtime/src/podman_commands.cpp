#include "inc/podman_commands.hpp"

#include <cstdlib>

#include "inc/common.hpp"
#include "inc/cm_logging.hpp"

PodmanRuntimeAvailableCommand::PodmanRuntimeAvailableCommand() {
    // Empty constructor
}

bool PodmanRuntimeAvailableCommand::Execute() const {
    // Check if Docker is available
    int status = std::system("podman info > /dev/null 2>&1");
    if (status == 0) {
        CM_LOG_INFO << "Podman is running ";
        return true;
    } else {
        CM_LOG_ERROR << "Podman is not running";
        return false;
    }
}

PodmanStartContainerCommand::PodmanStartContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool PodmanStartContainerCommand::Execute() const {
    // Start the Podman container
    std::string command = "podman start " + app_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container started successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to start Podman container";
        return false;
    }
}

PodmanStopContainerCommand::PodmanStopContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool PodmanStopContainerCommand::Execute() const {
    // Stop the Podman container
    std::string command = "podman stop " + app_name_;
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman container stopped successfully";
        return true;
    } else {
        CM_LOG_ERROR << "Failed to stop Podman container";
        return false;
    }
}
