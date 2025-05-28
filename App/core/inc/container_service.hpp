/**
 * @file container_service.hpp
 * @brief Declares the ContainerServiceHandler class for business logic related to containers.
 *
 * This class acts as the service layer for container management, coordinating
 * container lifecycle operations (create, start, stop, restart, remove) and updating
 * the database with container status. It uses dependency injection for database access
 * and provides standardized error handling using the Status structure.
 */

#pragma once

#include <nlohmann/json.hpp>
#include "inc/status.hpp"
#include "inc/database_interface.hpp"
#include "inc/container_request.hpp"

/**
 * @class ContainerServiceHandler
 * @brief Handles container operations and updates status in the database.
 *
 * Provides methods for handling container requests, dispatching to the appropriate
 * command, and updating the status of containers in the database. Uses dependency
 * injection for the database handler to improve testability and flexibility.
 */
class ContainerServiceHandler {
public:
    /**
     * @brief Constructs a ContainerServiceHandler with a database handler.
     * @param db Reference to an IDatabaseHandler implementation.
     */
    explicit ContainerServiceHandler(IDatabaseHandler& db);
    
    /**
     * @brief Handles a container request by dispatching to the appropriate operation.
     *        Updates the container status in the database based on the operation result.
     * @param req The container request containing operation, runtime, and container details.
     * @return A JSON object with the result of the operation, including status and error message if any.
     *
     * Steps:
     * 1. Perform the requested container operation.
     * 2. Update the container status in the database based on the operation and result.
     * 3. Return a JSON object indicating success or error, with operation and container details.
     */
    nlohmann::json HandleRequest(const ContainerRequest& req);

    /**
     * @brief Performs the specified container operation using the request structure.
     *        Invokes the appropriate command via the command factory.
     * @param req The container request containing all necessary details.
     * @return Status indicating the result of the operation.
     */
    Status ContainerOperations(const ContainerRequest& req);

private:
    IDatabaseHandler& db_; ///< Reference to the injected database handler.
};