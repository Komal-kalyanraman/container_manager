#pragma once

#include <string>
#include <memory>

/// @brief Abstract base class for all commands
/// @details This class defines the interface for command objects.
/// @note All command classes should inherit from this class and implement the Execute method.
/// @warning Do not instantiate this class directly.
class Command {
public:
    /// @brief Execute the command
    /// @return true if the command was executed successfully, false otherwise
    virtual bool Execute() const = 0;

    /// @brief Destructor
    virtual ~Command() = default;
};

/// @brief Invoker class to invoke the command
/// @details This class holds a command and invokes it.
class Invoker {
private:
    /// @brief Command to be executed
    std::unique_ptr<Command> command_;
public:
    /// @brief Set the command to be executed
    void SetCommand(std::unique_ptr<Command> cmd) {
        command_ = std::move(cmd);
    }
    /// @brief Execute the command
    bool Invoke() {
        if (command_) {
            return command_->Execute();
        }
        return false;
    }
};