#pragma once
#include <memory>
#include <string>
#include "inc/commands.hpp"
#include "inc/common.hpp"

/// @brief CommandFactory class to create command objects based on runtime type and command type with optional paarameter
class CommandFactory {
public:
    /// @brief Factory method to create a command based on the runtime type and command type
    /// @param runtime_type Container runtime type
    /// @param command_type Requested command type
    /// @return Command object
    static std::unique_ptr<Command> CreateCommand(const std::string& runtime_type, const std::string& command_type);

    /// @brief Factory method to create a command based on the runtime type, command type, and app name
    /// @param runtime_type Container runtime type
    /// @param command_type Requested command type
    /// @param container_name Container name
    /// @return Command object
    static std::unique_ptr<Command> CreateCommand(const std::string& runtime_type, const std::string& command_type, const std::string& container_name);
};