#include "inc/cm_container_service_handler.hpp"

#include <iostream>

#include <nlohmann/json.hpp>

#include "inc/cm_common.hpp"
#include "inc/cm_command_factory.hpp"

nlohmann::json ContainerServiceHandler::HandleRequest(const ContainerRequest& req) {
    if(req.operation == "Available") {
        bool isRuntimeAvailable = ContainerServiceHandler::CheckRuntimeAvailable(req.runtime, req.operation);
        if (!isRuntimeAvailable) {
            return {{"status", "error"}, {"message", "Runtime is not running"}};
        }
    } 
    // else if (req.operation == CommandType::StartContainer) {
    //     // Start the container
    //     auto command = CommandFactory::CreateCommand(req.runtime, CommandType::StartContainer, req.container_name);
    //     if (!command || !command->Execute()) {
    //         return {{"status", "error"}, {"message", "Failed to start container"}};
    //     }
    // } else if (req.operation == CommandType::StopContainer) {
    //     // Stop the container
    //     auto command = CommandFactory::CreateCommand(req.runtime, CommandType::StopContainer, req.container_name);
    //     if (!command || !command->Execute()) {
    //         return {{"status", "error"}, {"message", "Failed to stop container"}};
    //     }
    // } 
    else {
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
    return true;
}