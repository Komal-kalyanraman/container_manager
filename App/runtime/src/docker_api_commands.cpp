/**
 * @file docker_api_commands.cpp
 * @brief Implements Docker API command classes for managing containers via the Docker HTTP API over a Unix socket.
 *
 * This file defines the logic for DockerApiRuntimeAvailableCommand, DockerApiCreateContainerCommand,
 * DockerApiStartContainerCommand, DockerApiStopContainerCommand, DockerApiRestartContainerCommand,
 * and DockerApiRemoveContainerCommand. Each command executes the corresponding Docker REST API operation
 * using parameters from the ContainerRequest structure and CurlHandler for HTTP communication.
 * All commands return a Status object for standardized error handling.
 */

#include "inc/docker_api_commands.hpp"
#include "inc/common.hpp"
#include "inc/logging.hpp"
#include "inc/curl_handler.hpp"
#include <iostream>
#include <curl/curl.h>

/**
 * @class DockerApiRuntimeAvailableCommand
 * @brief Command to check if Docker daemon is available via the API.
 */
DockerApiRuntimeAvailableCommand::DockerApiRuntimeAvailableCommand() {}

/// Executes the command to check Docker API runtime availability.
/// @return Status indicating if Docker is running.
Status DockerApiRuntimeAvailableCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) + std::string(ApiEndpoint::Ping);
    bool success = curl.GetUnix(url, kDockerUnixSocketPath, response);
    if (success && response == "OK") {
        CM_LOG_INFO << "[Docker API] Docker daemon is running (/_ping returned OK).";
        return Status::Ok();
    } else if (success) {
        std::string msg = "[Docker API] Docker daemon responded, but not OK: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::Unavailable, msg);
    } else {
        std::string msg = "[Docker API] Failed to contact Docker daemon.";
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::Unavailable, msg);
    }
}

/**
 * @class DockerApiCreateContainerCommand
 * @brief Command to create a Docker container via the Docker API.
 * @param req The container request containing all parameters for creation.
 */
DockerApiCreateContainerCommand::DockerApiCreateContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to create a Docker container via the API.
/// @return Status indicating if the container was created successfully.
Status DockerApiCreateContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = std::string(kDockerApiBaseUrl) +
                      std::string(ApiEndpoint::Create) +
                      req_.container_name;

    // Build JSON body with all relevant fields
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

    bool success = curl.PostUnix(url, kDockerUnixSocketPath, body, response);
    if (success) {
        CM_LOG_INFO << "[Docker API] Create container response: " << response;
        return Status::Ok();
    } else {
        std::string msg = "[Docker API] Failed to create container. Response: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerApiStartContainerCommand
 * @brief Command to start a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiStartContainerCommand::DockerApiStartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to start a Docker container via the API.
/// @return Status indicating if the container was started successfully.
Status DockerApiStartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Start
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        CM_LOG_INFO << "[Docker API] Start container response: " << response;
        return Status::Ok();
    } else {
        std::string msg = "[Docker API] Failed to start container. Response: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerApiStopContainerCommand
 * @brief Command to stop a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiStopContainerCommand::DockerApiStopContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to stop a Docker container via the API.
/// @return Status indicating if the container was stopped successfully.
Status DockerApiStopContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Stop
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        CM_LOG_INFO << "[Docker API] Stop container response: " << response;
        return Status::Ok();
    } else {
        std::string msg = "[Docker API] Failed to stop container. Response: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerApiRestartContainerCommand
 * @brief Command to restart a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiRestartContainerCommand::DockerApiRestartContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to restart a Docker container via the API.
/// @return Status indicating if the container was restarted successfully.
Status DockerApiRestartContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Restart
    );
    bool success = curl.PostUnix(url, kDockerUnixSocketPath, "", response, "application/json");
    if (success) {
        CM_LOG_INFO << "[Docker API] Restart container response: " << response;
        return Status::Ok();
    } else {
        std::string msg = "[Docker API] Failed to restart container. Response: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::InternalError, msg);
    }
}

/**
 * @class DockerApiRemoveContainerCommand
 * @brief Command to remove a Docker container via the Docker API.
 * @param req The container request containing the container name.
 */
DockerApiRemoveContainerCommand::DockerApiRemoveContainerCommand(const ContainerRequest& req) : req_(req) {}

/// Executes the command to remove a Docker container via the API.
/// @return Status indicating if the container was removed successfully.
Status DockerApiRemoveContainerCommand::Execute() const {
    CurlHandler curl;
    std::string response;
    std::string url = ComposeContainerApiEndpoint(
        kDockerApiBaseUrl, req_.container_name, ApiEndpoint::Remove
    );
    bool success = curl.DeleteUnix(url, kDockerUnixSocketPath, response);
    if (success) {
        CM_LOG_INFO << "[Docker API] Remove container response: " << response;
        return Status::Ok();
    } else {
        std::string msg = "[Docker API] Failed to remove container. Response: " + response;
        CM_LOG_ERROR << msg;
        return Status::Error(StatusCode::InternalError, msg);
    }
}