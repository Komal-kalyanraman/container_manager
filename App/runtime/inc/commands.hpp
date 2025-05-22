#pragma once

#include <string>
#include <memory>

/**
 * @class Command
 * @brief Abstract base class for all commands.
 * @details This class defines the interface for command objects.
 * @note All command classes should inherit from this class and implement the Execute method.
 * @warning Do not instantiate this class directly.
 */
class Command {
public:
    /**
     * @brief Execute the command.
     * @return true if the command was executed successfully, false otherwise.
     */
    virtual bool Execute() const = 0;

    /**
     * @brief Virtual destructor.
     */
    virtual ~Command() = default;
};

/**
 * @class Invoker
 * @brief Invoker class to invoke the command.
 * @details This class holds a command and invokes it.
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
     * @return true if the command was executed successfully, false otherwise.
     */
    bool Invoke() {
        if (command_) {
            return command_->Execute();
        }
        return false;
    }
};