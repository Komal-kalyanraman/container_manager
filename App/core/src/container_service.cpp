/**
 * @file container_service.cpp
 * @brief Implements the ContainerServiceHandler class for business logic related to containers.
 *
 * This file defines the methods for handling container lifecycle operations (create, start, stop, restart, remove),
 * updating the database with container status, and invoking the appropriate command via the command factory.
 * The class uses dependency injection for database access.
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
 * @brief Constructs a ContainerServiceHandler with a database handler.
 * @param db Reference to an IDatabaseHandler implementation.
 */
ContainerServiceHandler::ContainerServiceHandler(IDatabaseHandler& db) : db_(db) {}

/**
 * @brief Handles a container request by dispatching to the appropriate operation and updating the database.
 *        Updates the container status in the database based on the operation result.
 * @param req The container request containing operation, runtime, and container details.
 * @return A JSON object with the result of the operation.
 */
nlohmann::json ContainerServiceHandler::HandleRequest(const ContainerRequest& req) {
    bool op_result = ContainerOperations(req);

    if (req.operation == CommandName::CreateContainer) {
        if (op_result) db_.UpdateField(req.container_name, "status", "created");
        else db_.UpdateField(req.container_name, "status", "Error creating container");
    } else if (req.operation == CommandName::StartContainer) {
        if (op_result) db_.UpdateField(req.container_name, "status", "running");
        else db_.UpdateField(req.container_name, "status", "Error running container");
    } else if (req.operation == CommandName::StopContainer) {
        if (op_result) db_.UpdateField(req.container_name, "status", "stopped");
        else db_.UpdateField(req.container_name, "status", "Error stopping container");
    } else if (req.operation == CommandName::RestartContainer) {
        if (op_result) db_.UpdateField(req.container_name, "status", "running");
        else db_.UpdateField(req.container_name, "status", "Error restarting container");
    } else if (req.operation == CommandName::RemoveContainer) {
        if (op_result) db_.RemoveKey(req.container_name);
        else db_.UpdateField(req.container_name, "status", "Error removing container");
    }

    if (!op_result) {
        return {{"status", "error"}, {"message", "Operation failed"}, {"operation", req.operation}, {"container", req.container_name}};
    }
    return {{"status", "success"}, {"operation", req.operation}, {"container", req.container_name}};
}

/**
 * @brief Performs the specified container operation using the request structure.
 *        Invokes the appropriate command via the command factory.
 * @param req The container request containing all necessary container details.
 * @return True if the operation succeeded, false otherwise.
 */
bool ContainerServiceHandler::ContainerOperations(const ContainerRequest& req) {
    Invoker invoker;
    invoker.SetCommand(CommandFactory::CreateCommand(req));
    return invoker.Invoke();
}