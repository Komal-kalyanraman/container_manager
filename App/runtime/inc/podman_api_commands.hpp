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
    std::string container_name_;
    std::string image_name_;
public:
    PodmanApiCreateContainerCommand(const std::string& container_name, const std::string& image_name);
    ~PodmanApiCreateContainerCommand() = default;

    bool Execute() const override;
};

class PodmanApiStartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanApiStartContainerCommand(const std::string& container_name_);
    ~PodmanApiStartContainerCommand() = default;

    bool Execute() const override;
};

class PodmanApiStopContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanApiStopContainerCommand(const std::string& container_name_);
    ~PodmanApiStopContainerCommand() = default;

    bool Execute() const override;
};
