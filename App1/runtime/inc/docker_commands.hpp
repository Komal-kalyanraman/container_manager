#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

class DockerRuntimeAvailableCommand : public Command {
public:
    explicit DockerRuntimeAvailableCommand();
    ~DockerRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

class DockerCreateContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit DockerCreateContainerCommand(const std::string& app_name_);
    ~DockerCreateContainerCommand() = default;

    bool Execute() const override;
};

class DockerStartContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit DockerStartContainerCommand(const std::string& app_name_);
    ~DockerStartContainerCommand() = default;

    bool Execute() const override;
};

class DockerStopContainerCommand : public Command {
private:
    std::string app_name_;
public:
    explicit DockerStopContainerCommand(const std::string& app_name_);
    ~DockerStopContainerCommand() = default;

    bool Execute() const override;
};
