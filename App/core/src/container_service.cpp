/**
 * @file container_service.cpp
 * @brief Implements the ContainerServiceHandler class for business logic related to containers.
 */

#include "inc/container_service.hpp"

#include <iostream>
#include <nlohmann/json.hpp>
#include "inc/common.hpp"
#include "inc/command_factory.hpp"
#include "inc/container_request.hpp"
#include "inc/database_interface.hpp"
#include "inc/redis_database.hpp"

/**
 * @brief Handles a container request by dispatching to the appropriate operation and updating the database.
 * @param req The container request containing operation, runtime, and container details.
 * @return A JSON object with the result of the operation.
 */
nlohmann::json ContainerServiceHandler::HandleRequest(const ContainerRequest& req) {
    bool op_result = ContainerServiceHandler::ContainerOperations(req);

    IDatabaseHandler& db = RedisDatabaseHandler::GetInstance();
    if (req.operation == CommandName::CreateContainer) {
        if (op_result) db.UpdateField(req.container_name, "status", "created");
        else db.UpdateField(req.container_name, "status", "Error creating container");
    } else if (req.operation == CommandName::StartContainer) {
        if (op_result) db.UpdateField(req.container_name, "status", "running");
        else db.UpdateField(req.container_name, "status", "Error running container");
    } else if (req.operation == CommandName::StopContainer) {
        if (op_result) db.UpdateField(req.container_name, "status", "stopped");
        else db.UpdateField(req.container_name, "status", "Error stopping container");
    } else if (req.operation == CommandName::RestartContainer) {
        if (op_result) db.UpdateField(req.container_name, "status", "running");
        else db.UpdateField(req.container_name, "status", "Error restarting container");
    } else if (req.operation == CommandName::RemoveContainer) {
        if (op_result) db.RemoveKey(req.container_name);
        else db.UpdateField(req.container_name, "status", "Error removing container");
    }

    if (!op_result) {
        return {{"status", "error"}, {"message", "Operation failed"}, {"operation", req.operation}, {"container", req.container_name}};
    }
    return {{"status", "success"}, {"operation", req.operation}, {"container", req.container_name}};
}

/**
 * @brief Performs the specified container operation using the request structure.
 * @param req The container request containing all necessary container details.
 * @return True if the operation succeeded, false otherwise.
 */
bool ContainerServiceHandler::ContainerOperations(const ContainerRequest& req) {
    Invoker invoker;
    invoker.SetCommand(CommandFactory::CreateCommand(req));
    return invoker.Invoke();
}