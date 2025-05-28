/**
 * @file podman_api_commands.cpp
 * @brief Implements Podman API command classes for managing containers via the Podman HTTP API over a Unix socket.
 *
 * This file provides implementations for commands such as checking Podman daemon availability,
 * creating, starting, and stopping containers using the Podman REST API.
 * All HTTP requests are performed over the Unix socket using the CurlHandler utility.
 * All commands return a Status object for standardized error handling.
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
/// @return Status indicating if Podman is running.
Status PodmanApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) + std::string(ApiEndpoint::Ping);
    bool success = curl.GetUnix(url, kPodmanUnixSocketPath, response);
    if (success && response == "OK") {
        CM_LOG_INFO << "[Podman API] Podman daemon is running (/_ping returned OK)." << std::endl;
        return Status::Ok();
    } else if (success) {
        std::string msg = "[Podman API] Podman daemon responded, but not OK: " + response;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::Unavailable, msg);
    } else {
        std::string msg = "[Podman API] Failed to contact Podman daemon.";
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::Unavailable, msg);
    }
}

/**
 * @class PodmanApiCreateContainerCommand
 * @brief Command to create a Podman container via the Podman API.
 * @param req The container request containing all parameters for creation.
 */
PodmanApiCreateContainerCommand::PodmanApiCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Podman container via the API.
/// @return Status indicating if the container was created successfully.
Status PodmanApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kPodmanApiBaseUrl) +
                      std::string(ApiEndpoint::Create) +
                      req_.container_name;

    // Build JSON body with all relevant fields from the request
    std::string body = "{";
    body += "\"Image\": \"" + req_.image_name + "\"";
    bool hasHostConfig = false;
    if (!req_.cpus.empty()) {
        body += ", \"HostConfig\": {\"NanoCpus\": " + std::to_string(static_cast<long long>(std::stod(req_.cpus) * 1e9));
        hasHostConfig = true;
    }
    if (!req_.memory.empty()) {
        if (!hasHostConfig) {
            body += ", \"HostConfig\": {";
            hasHostConfig = true;
        } else {
            body += ", ";
        }
        body += "\"Memory\": " + std::to_string(std::stoll(req_.memory) * 1024 * 1024);
    }
    if (!req_.pids.empty()) {
        if (!hasHostConfig) {
            body += ", \"HostConfig\": {";
            hasHostConfig = true;
        } else {
            body += ", ";
        }
        body += "\"PidsLimit\": " + req_.pids;
    }
    if (!req_.restart_policy.empty()) {
        if (!hasHostConfig) {
            body += ", \"HostConfig\": {";
            hasHostConfig = true;
        } else {
            body += ", ";
        }
        body += "\"RestartPolicy\": {\"Name\": \"" + req_.restart_policy + "\"}";
    }
    if (hasHostConfig)
        body += "}";
    body += "}";

    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, body, response);
    if (success) {
        CM_LOG_INFO << "[Podman API] Create container response: " << response << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "[Podman API] Failed to create container. Response: " + response;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanApiStartContainerCommand
 * @brief Command to start a Podman container via the Podman API.
 * @param req The container request containing the container name.
 */
PodmanApiStartContainerCommand::PodmanApiStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Podman container via the API.
/// @return Status indicating if the container was started successfully.
Status PodmanApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kPodmanApiBaseUrl, req_.container_name, ApiEndpoint::Start
    );
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        CM_LOG_INFO << "[Podman API] Start container response: " << response << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "[Podman API] Failed to start container. Response: " + response;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class PodmanApiStopContainerCommand
 * @brief Command to stop a Podman container via the Podman API.
 * @param req The container request containing the container name.
 */
PodmanApiStopContainerCommand::PodmanApiStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Podman container via the API.
/// @return Status indicating if the container was stopped successfully.
Status PodmanApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kPodmanApiBaseUrl, req_.container_name, ApiEndpoint::Stop
    );
    bool success = curl.PostUnix(url, kPodmanUnixSocketPath, "", response, "application/json");
    if (success) {
        CM_LOG_INFO << "[Podman API] Stop container response: " << response << std::endl;
        return Status::Ok();
    } else {
        std::string msg = "[Podman API] Failed to stop container. Response: " + response;
        CM_LOG_ERROR << msg << std::endl;
        return Status::Error(StatusCode::InternalError, msg);
    }
}