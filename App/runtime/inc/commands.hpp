#pragma once

#include <string>
#include <memory>
#include "inc/status.hpp"

/**
 * @file commands.hpp
 * @brief Declares the Command interface and Invoker class for the command pattern.
 *
 * This file defines the abstract Command base class, which all concrete command classes must inherit from,
 * and the Invoker class, which is responsible for holding and executing commands. The command pattern
 * enables encapsulation of request parameters and standardized error handling using the Status structure.
 */

/**
 * @class Command
 * @brief Abstract base class for all commands.
 * @details This class defines the interface for command objects.
 *          All command classes should inherit from this class and implement the Execute method.
 * @warning Do not instantiate this class directly.
 */
class Command {
public:
    /**
     * @brief Execute the command.
     * @return Status indicating the result of the command execution.
     */
    virtual Status Execute() const = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Command() = default;
};

/**
 * @class Invoker
 * @brief Invoker class to invoke the command.
 * @details This class holds a command and invokes it. It manages the command's lifetime using a unique pointer.
 */
class Invoker {
private:
    std::unique_ptr<Command> command_; ///< Command to be executed.
public:
    /**
     * @brief Set the command to be executed.
     * @param cmd Unique pointer to the command object.
     */
    void SetCommand(std::unique_ptr<Command> cmd) {
        command_ = std::move(cmd);
    }

    /**
     * @brief Execute the command.
     * @return Status indicating the result of the command execution. Returns an error status if no command is set.
     */
    Status Invoke() {
        if (command_) {
            return command_->Execute();
        }
        return Status(StatusCode::InvalidArgument, "No command set");
    }
};