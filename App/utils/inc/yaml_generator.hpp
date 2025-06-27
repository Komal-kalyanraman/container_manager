#pragma once

#include <string>
#include <unordered_map>
#include "inc/status.hpp"
#include "inc/container_request.hpp"

/// @brief Runtime types for YAML generation.
enum class YamlRuntime {
    Podman,     ///< Generate YAML for Podman (includes Podman-specific fields)
    Kubernetes  ///< Generate YAML for Kubernetes/k3s (standard K8s format)
};

/// @brief Generates a Pod YAML file from a template.
/// @param req The container request containing runtime parameters.
/// @param runtime_type The target runtime (Podman or Kubernetes).
/// @param output_path [out] Path to the generated YAML file (if successful).
/// @return Status indicating success or failure with error details.
Status GeneratePodYaml(const ContainerRequest& req, 
                                YamlRuntime runtime_type, 
                                std::string& output_path);

/// @brief Reads a template file and replaces placeholders with actual values.
/// @param template_path Path to the template file.
/// @param replacements Map of placeholder -> replacement value pairs.
/// @param output_path Path where the processed file should be written.
/// @return Status indicating success or failure with error details.
Status ProcessYamlTemplate(const std::string& template_path, 
                          const std::unordered_map<std::string, std::string>& replacements,
                          const std::string& output_path);

/// @brief Validates that all required placeholders are replaced in the YAML content.
/// @param content The YAML content to validate.
/// @return Status indicating success or failure with validation details.
Status ValidateYamlContent(const std::string& content);