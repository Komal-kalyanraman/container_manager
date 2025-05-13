#include "inc/cm_podman_api_commands.hpp"

#include "inc/cm_common.hpp"
#include "inc/cm_logging.hpp"

PodmanApiRuntimeAvailableCommand::PodmanApiRuntimeAvailableCommand() {
    // Empty constructor
}
bool PodmanApiRuntimeAvailableCommand::Execute() const {
    return true;
}

PodmanApiCreateContainerCommand::PodmanApiCreateContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool PodmanApiCreateContainerCommand::Execute() const {
    return true;
}

PodmanApiStartContainerCommand::PodmanApiStartContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool PodmanApiStartContainerCommand::Execute() const {
    return true;
}

PodmanApiStopContainerCommand::PodmanApiStopContainerCommand(const std::string& app_name) : app_name_(app_name) {
    // Empty constructor
}

bool PodmanApiStopContainerCommand::Execute() const {
    return true;
}
