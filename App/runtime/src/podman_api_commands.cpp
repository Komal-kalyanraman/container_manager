/**
 * @file podman_api_commands.cpp
 * @brief Implements Podman API command classes for managing containers via the Podman HTTP API over a Unix socket.
 *
 * This file provides implementations for commands such as checking Podman daemon availability,
 * creating, starting, and stopping containers using the Podman REST API.
 * All HTTP requests are performed over the Unix socket using the CurlHandler utility.
 */

#include "inc/podman_api_commands.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

/**
 * @class PodmanApiRuntimeAvailableCommand
 * @brief Command to check if Podman daemon is available via the API.
 */
PodmanApiRuntimeAvailableCommand::PodmanApiRuntimeAvailableCommand() {}

/// Executes the command to check Podman API runtime availability.
/// @return True if Podman is running, false otherwise.
bool PodmanApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + std::string(ApiEndpoint::Ping);
    bool success = curl.GetUnix(url, kPodmanUnixSocketPath, response);
    if (success && response == "OK") {
        std::cout << "[Podman API] Podman daemon is running (/_ping returned OK)." << std::endl;
        return true;
    } else if (success) {
        std::cerr << "[Podman API] Podman daemon responded, but not OK: " << response << std::endl;
        return false;
    } else {
        std::cerr << "[Podman API] Failed to contact Podman daemon." << std::endl;
        return false;
    }
}

/**
 * @class PodmanApiCreateContainerCommand
 * @brief Command to create a Podman container via the Podman API.
 * @param req The container request containing all parameters for creation.
 */
PodmanApiCreateContainerCommand::PodmanApiCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Podman container via the API.
/// @return True if the container was created successfully, false otherwise.
bool PodmanApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) +
                      std::string(ApiEndpoint::Create) +
                      req_.container_name;

    // Build JSON body with all relevant fields from the request
    std::string body = "{";
    body += "\"Image\": \"" + req_.image_name + "\"";
    // Add more HostConfig fields as needed, similar to Docker API
    if (!req_.cpus.empty()) body += ", \"HostConfig\": {\"NanoCpus\": " + std::to_string(static_cast<long long>(std::stod(req_.cpus) * 1e9));
    if (!req_.memory.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"Memory\": " + std::to_string(std::stoll(req_.memory) * 1024 * 1024);
    }
    if (!req_.pids.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"PidsLimit\": " + req_.pids;
    }
    if (!req_.restart_policy.empty()) {
        if (body.find("HostConfig") == std::string::npos)
            body += ", \"HostConfig\": {";
        else
            body += ", ";
        body += "\"RestartPolicy\": {\"Name\": \"" + req_.restart_policy + "\"}";
    }
    if (body.find("HostConfig") != std::string::npos)
        body += "}";
    body += "}";

    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, body, response);
    if (success) {
        std::cout << "[Podman API] Create container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to create container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class PodmanApiStartContainerCommand
 * @brief Command to start a Podman container via the Podman API.
 * @param req The container request containing the container name.
 */
PodmanApiStartContainerCommand::PodmanApiStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Podman container via the API.
/// @return True if the container was started successfully, false otherwise.
bool PodmanApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kPodmanApiBaseUrl, req_.container_name, ApiEndpoint::Start
    );
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Podman API] Start container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to start container. Response: " << response << std::endl;
        return false;
    }
}

/**
 * @class PodmanApiStopContainerCommand
 * @brief Command to stop a Podman container via the Podman API.
 * @param req The container request containing the container name.
 */
PodmanApiStopContainerCommand::PodmanApiStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Podman container via the API.
/// @return True if the container was stopped successfully, false otherwise.
bool PodmanApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kPodmanApiBaseUrl, req_.container_name, ApiEndpoint::Stop
    );
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        std::cout << "[Podman API] Stop container response: " << response << std::endl;
        return true;
    } else {
        std::cerr << "[Podman API] Failed to stop container. Response: " << response << std::endl;
        return false;
    }
}