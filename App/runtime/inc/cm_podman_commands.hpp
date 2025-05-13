#pragma once

#include <string>
#include <memory>
#include "inc/cm_commands.hpp"

class PodmanRuntimeAvailableCommand : public Command {
public:
    explicit PodmanRuntimeAvailableCommand();
    ~PodmanRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

class PodmanStartContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit PodmanStartContainerCommand(const std::string& app_name_);
    ~PodmanStartContainerCommand() = default;

    bool Execute() const override;
};

class PodmanStopContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit PodmanStopContainerCommand(const std::string& app_name_);
    ~PodmanStopContainerCommand() = default;

    bool Execute() const override;
};
