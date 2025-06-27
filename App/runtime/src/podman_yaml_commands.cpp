#include "inc/podman_yaml_commands.hpp"
#include "inc/yaml_generator.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include <cstdlib>

/**
 * @class PodmanYamlPlayCommand
 * @brief Command to play a Podman YAML file (create resources).
 */
PodmanYamlPlayCommand::PodmanYamlPlayCommand(const ContainerRequest& req)
    : req_(req) {}

/// Executes the command to play the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlPlayCommand::Execute() const {
    // Generate YAML from template
    std::string yaml_file_path;
    Status generation_status = GeneratePodYaml(req_, YamlRuntime::Podman, yaml_file_path);
    if (!generation_status.ok()) {
        CM_LOG_ERROR << "Failed to generate YAML file: " << generation_status.message << std::endl;
        return generation_status;
    }
    
    // Use CommandTemplate with FormatCommand
    std::unordered_map<std::string, std::string> values = {
        {"yaml_file", yaml_file_path}
    };
    std::string command = FormatCommand(CommandTemplate::PlayKube, values);
    
    CM_LOG_INFO << "Executing command: " << command << std::endl;
    
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file played successfully: " << yaml_file_path << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to play Podman YAML file, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanYamlDeleteCommand
 * @brief Command to delete resources defined in a Podman YAML file.
 */
PodmanYamlDeleteCommand::PodmanYamlDeleteCommand(const ContainerRequest& req)
    : req_(req) {}

/// Executes the command to delete resources defined in the YAML file.
/// @return Status indicating if the operation was successful.
Status PodmanYamlDeleteCommand::Execute() const {
    // Generate YAML from template (same file used for deletion)
    std::string yaml_file_path;
    Status generation_status = GeneratePodYaml(req_, YamlRuntime::Podman, yaml_file_path);
    if (!generation_status.ok()) {
        CM_LOG_ERROR << "Failed to generate YAML file: " << generation_status.message << std::endl;
        return generation_status;
    }
    
    // Use CommandTemplate with FormatCommand
    std::unordered_map<std::string, std::string> values = {
        {"yaml_file", yaml_file_path}
    };
    std::string command = FormatCommand(CommandTemplate::DeleteKube, values);
    
    CM_LOG_INFO << "Executing command: " << command << std::endl;
    
    int status = std::system(command.c_str());
    if (status == 0) {
        CM_LOG_INFO << "Podman YAML file resources deleted successfully: " << yaml_file_path << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "Failed to delete Podman YAML file resources, exit code: " + std::to_string(status);
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}