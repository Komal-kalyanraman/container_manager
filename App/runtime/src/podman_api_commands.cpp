#include "inc/podman_api_commands.hpp"

#include "inc/common.hpp"
#include "inc/logging.hpp"

PodmanApiRuntimeAvailableCommand::PodmanApiRuntimeAvailableCommand() {
    // Empty constructor
}
bool PodmanApiRuntimeAvailableCommand::Execute() const {
    return true;
}

PodmanApiCreateContainerCommand::PodmanApiCreateContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanApiCreateContainerCommand::Execute() const {
    return true;
}

PodmanApiStartContainerCommand::PodmanApiStartContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanApiStartContainerCommand::Execute() const {
    return true;
}

PodmanApiStopContainerCommand::PodmanApiStopContainerCommand(const std::string& container_name) : container_name_(container_name) {
    // Empty constructor
}

bool PodmanApiStopContainerCommand::Execute() const {
    return true;
}
