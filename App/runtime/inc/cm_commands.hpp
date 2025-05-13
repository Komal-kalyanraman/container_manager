#pragma once

#include <string>
#include <memory>

/// @brief Abstract base class for all commands
/// @details This class defines the interface for command objects.
/// @note All command classes should inherit from this class and implement the Execute method.
/// @warning Do not instantiate this class directly.
class Command {
public:
    /// @brief Execute member function to 
    /// @return  
    virtual bool Execute() const = 0;

    /// @brief 

    virtual ~Command() = default;
};

/// @brief Invoker class to invoke the command
class Invoker {
    std::unique_ptr<Command> command_;
public:
    void SetCommand(std::unique_ptr<Command> cmd) {
        command_ = std::move(cmd);
    }
    bool Invoke() {
        if (command_) {
            return command_->Execute();
        }
        return false;
    }
};