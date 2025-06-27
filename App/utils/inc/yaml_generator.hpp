#pragma once

#include <string>
#include "inc/container_request.hpp"

/// @brief Generates a Kubernetes Pod YAML file based on container request.
/// @param req The container request containing runtime parameters.
/// @param include_security_context Whether to include securityContext (for Podman compatibility).
/// @return Path to the generated YAML file.
std::string GenerateKubernetesPodYaml(const ContainerRequest& req, bool include_security_context = true);