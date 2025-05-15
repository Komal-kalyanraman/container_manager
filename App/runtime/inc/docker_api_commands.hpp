#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"

class DockerApiRuntimeAvailableCommand : public Command {
public:
    explicit DockerApiRuntimeAvailableCommand();
    ~DockerApiRuntimeAvailableCommand() = default;

    bool Execute() const override;
};

class DockerApiCreateContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiCreateContainerCommand(const std::string& container_name_);
    ~DockerApiCreateContainerCommand() = default;

    bool Execute() const override;
};

class DockerApiStartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiStartContainerCommand(const std::string& container_name_);
    ~DockerApiStartContainerCommand() = default;

    bool Execute() const override;
};

class DockerApiStopContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit DockerApiStopContainerCommand(const std::string& container_name_);
    ~DockerApiStopContainerCommand() = default;

    bool Execute() const override;
};
