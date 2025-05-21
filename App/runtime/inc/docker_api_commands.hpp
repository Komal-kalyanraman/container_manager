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
    std::string image_name_;
public:
    DockerApiCreateContainerCommand(const std::string& container_name, const std::string& image_name);
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
