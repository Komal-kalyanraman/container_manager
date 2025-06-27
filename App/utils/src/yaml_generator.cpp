#include "inc/yaml_generator.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <regex>

#include "inc/common.hpp"
#include "inc/logging.hpp"

Status GeneratePodYaml(const ContainerRequest& req, 
                                YamlRuntime runtime_type, 
                                std::string& output_path) {
    try {
        // Determine template and output paths based on runtime type
        std::string template_base_path;
        std::string output_base_path;
        
        switch (runtime_type) {
            case YamlRuntime::Podman:
                template_base_path = std::string(kPodmanYamlStoragePath);
                output_base_path = std::string(kPodmanYamlStoragePath);
                break;
            case YamlRuntime::Kubernetes:
                template_base_path = std::string(kK3sYamlStoragePath);
                output_base_path = std::string(kK3sYamlStoragePath);
                break;
            default:
                std::string msg = "Unsupported YAML runtime type: " + std::to_string(static_cast<int>(runtime_type));
                CM_LOG_ERROR << msg << std::endl;
                return Status::Error(StatusCode::InvalidArgument, msg);
        }
        
        std::string template_path = template_base_path + kMasterTemplateFile;
        output_path = output_base_path + req.container_name + kPodFileSuffix;
        
        // Prepare replacement map
        std::unordered_map<std::string, std::string> replacements = {
            {YamlPlaceholder::kPodName, req.container_name + kPodNameSuffix},
            {YamlPlaceholder::kContainerName, req.container_name},
            {YamlPlaceholder::kImageName, req.image_name},
            {YamlPlaceholder::kCpuLimit, req.cpus},
            {YamlPlaceholder::kMemoryLimit, req.memory},
            {YamlPlaceholder::kPidsLimit, req.pids},
            {YamlPlaceholder::kRestartPolicy, req.restart_policy}
        };
        
        // Process the template
        Status process_status = ProcessYamlTemplate(template_path, replacements, output_path);
        if (!process_status.ok()) {
            return process_status;
        }
        
        CM_LOG_INFO << "Generated YAML file: " << output_path 
                   << " from template: " << template_path << std::endl;
        
        return Status::Ok();
        
    } catch (const std::exception& e) {
        std::string error_msg = "Failed to generate YAML file: " + std::string(e.what());
        CM_LOG_ERROR << error_msg << std::endl;
        return Status::Error(StatusCode::InternalError, error_msg);
    }
}

Status ProcessYamlTemplate(const std::string& template_path, 
                          const std::unordered_map<std::string, std::string>& replacements,
                          const std::string& output_path) {
    
    // Read template file
    std::ifstream template_file(template_path);
    if (!template_file.is_open()) {
        std::string msg = "Cannot open template file: " + template_path;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::NotFound, msg);
    }
    
    std::stringstream buffer;
    buffer << template_file.rdbuf();
    template_file.close();
    
    std::string content = buffer.str();
    
    if (content.empty()) {
        std::string msg = "Template file is empty: " + template_path;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InvalidArgument, msg);
    }
    
    // Replace all placeholders
    for (const auto& [placeholder, value] : replacements) {
        if (value.empty()) {
            CM_LOG_WARN << "Empty value for placeholder: " << placeholder << std::endl;
            continue;
        }
        
        size_t pos = 0;
        while ((pos = content.find(placeholder, pos)) != std::string::npos) {
            content.replace(pos, placeholder.length(), value);
            pos += value.length();
        }
    }
    
    // Validate that all placeholders are replaced
    Status validation_status = ValidateYamlContent(content);
    if (!validation_status.ok()) {
        std::string msg = "Template validation failed for: " + template_path + 
                         ". " + validation_status.message;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InvalidArgument, msg);
    }
    
    // Write output file
    std::ofstream output_file(output_path);
    if (!output_file.is_open()) {
        std::string msg = "Cannot create output file: " + output_path;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::PermissionDenied, msg);
    }
    
    output_file << content;
    output_file.close();
    
    if (output_file.fail()) {
        std::string msg = "Failed to write to output file: " + output_path;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
    
    return Status::Ok();
}

Status ValidateYamlContent(const std::string& content) {
    // Check for any remaining placeholders ({{...}})
    std::regex placeholder_pattern(R"(\{\{[^}]+\}\})");
    std::smatch match;
    
    if (std::regex_search(content, match, placeholder_pattern)) {
        std::string msg = "Found unreplaced placeholder: " + match.str();
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InvalidArgument, msg);
    }
    
    // Basic YAML structure validation
    if (content.find("apiVersion:") == std::string::npos ||
        content.find("kind:") == std::string::npos ||
        content.find("metadata:") == std::string::npos ||
        content.find("spec:") == std::string::npos) {
        std::string msg = "YAML content missing required fields (apiVersion, kind, metadata, or spec)";
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InvalidArgument, msg);
    }
    
    return Status::Ok();
}