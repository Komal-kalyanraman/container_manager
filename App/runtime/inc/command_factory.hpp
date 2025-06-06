/**
 * @file command_factory.hpp
 * @brief Declares the CommandFactory class for creating command objects based on container requests.
 */

#pragma once
#include <memory>
#include <string>
#include "inc/commands.hpp"
#include "inc/common.hpp"
#include "inc/container_request.hpp"

/**
 * @class CommandFactory
 * @brief Factory class to create command objects based on the request structure.
 */
class CommandFactory {
public:
    /**
     * @brief Factory method to create a command based on the container request structure.
     * @param req The container request containing all necessary details.
     * @return A unique pointer to a Command object appropriate for the request.
     */
    static std::unique_ptr<Command> CreateCommand(const ContainerRequest& req);
};