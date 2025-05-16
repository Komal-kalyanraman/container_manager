#include "inc/container_service.hpp"

#include <iostream>

#include <nlohmann/json.hpp>

#include "inc/common.hpp"
#include "inc/command_factory.hpp"

nlohmann::json ContainerServiceHandler::HandleRequest(const ContainerRequest& req) {
    if(req.operation == CommandName::RuntimeStatus) {
        bool isRuntimeAvailable = ContainerServiceHandler::CheckRuntimeAvailable(req.runtime, req.operation);
        if (!isRuntimeAvailable) {
            return {{"status", "error"}, {"message", "Runtime is not running"}};
        }
    } else if (req.operation == CommandName::CreateContainer) {
        bool isContainerCreated = ContainerServiceHandler::ContainerOperations(req.runtime, req.operation, req.container_name);
        if (!isContainerCreated) {
            return {{"status", "error"}, {"message", "Failed to create container"}};
        }
    } else if (req.operation == CommandName::StartContainer) {
        // Start the container
        bool isContainerStarted = ContainerServiceHandler::ContainerOperations(req.runtime, req.operation, req.container_name);
        if (!isContainerStarted) {
            return {{"status", "error"}, {"message", "Failed to start container"}};
        }
    } else if (req.operation == CommandName::StopContainer) {
        // Stop the container
        bool isContainerStopped = ContainerServiceHandler::ContainerOperations(req.runtime, req.operation, req.container_name);
        if (!isContainerStopped) {
            return {{"status", "error"}, {"message", "Failed to stop container"}};
        }
    } else if (req.operation == CommandName::RestartContainer) {
        // Stop the container
        bool isContainerRestarted = ContainerServiceHandler::ContainerOperations(req.runtime, req.operation, req.container_name);
        if (!isContainerRestarted) {
            return {{"status", "error"}, {"message", "Failed to restart container"}};
        }
    } else if (req.operation == CommandName::RemoveContainer) {
        // Stop the container
        bool isContainerRemoved = ContainerServiceHandler::ContainerOperations(req.runtime, req.operation, req.container_name);
        if (!isContainerRemoved) {
            return {{"status", "error"}, {"message", "Failed to remove container"}};
        }
    } else {
        return {{"status", "error"}, {"message", "Invalid operation"}};
    }
    // Example: Just echo the request for now
    std::cout << "Handling operation: " << req.operation << " for container: " << req.container_name << std::endl;
    // TODO: Integrate with your command pattern and actual logic
    return {{"status", "success"}, {"operation", req.operation}, {"container", req.container_name}};
}

bool ContainerServiceHandler::CheckRuntimeAvailable(const std::string& runtime, const std::string& operation) {
    Invoker invoker;

    invoker.SetCommand(CommandFactory::CreateCommand(runtime, operation));
        if (!invoker.Invoke()) {
            return false;
        }
    std::cout << "Runtime is available" << std::endl;
    return true;
}

bool ContainerServiceHandler::ContainerOperations(const std::string& runtime, const std::string& operation, const std::string& container_name) {
    Invoker invoker;

    invoker.SetCommand(CommandFactory::CreateCommand(runtime, operation, container_name));
    if (!invoker.Invoke()) {
        return false;
    }
    std::cout << "Container created successfully" << std::endl;
    return true;
}