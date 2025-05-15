#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

class PodmanApiRuntimeAvailableCommand : public Command {
public:
    explicit PodmanApiRuntimeAvailableCommand();
    ~PodmanApiRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

class PodmanApiCreateContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit PodmanApiCreateContainerCommand(const std::string& app_name_);
    ~PodmanApiCreateContainerCommand() = default;

    bool Execute() const override;
};

class PodmanApiStartContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit PodmanApiStartContainerCommand(const std::string& app_name_);
    ~PodmanApiStartContainerCommand() = default;

    bool Execute() const override;
};

class PodmanApiStopContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit PodmanApiStopContainerCommand(const std::string& app_name_);
    ~PodmanApiStopContainerCommand() = default;

    bool Execute() const override;
};
