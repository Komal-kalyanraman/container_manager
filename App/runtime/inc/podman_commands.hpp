#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

class PodmanRuntimeAvailableCommand : public Command {
public:
    explicit PodmanRuntimeAvailableCommand();
    ~PodmanRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

class PodmanStartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanStartContainerCommand(const std::string& container_name_);
    ~PodmanStartContainerCommand() = default;

    bool Execute() const override;
};

class PodmanStopContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanStopContainerCommand(const std::string& container_name_);
    ~PodmanStopContainerCommand() = default;

    bool Execute() const override;
};
