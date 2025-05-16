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

class PodmanCreateContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanCreateContainerCommand(const std::string& container_name_);
    ~PodmanCreateContainerCommand() = default;

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

class PodmanRemoveContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanRemoveContainerCommand(const std::string& container_name_);
    ~PodmanRemoveContainerCommand() = default;

    bool Execute() const override;
};

class PodmanRestartContainerCommand : public Command {
private:
    std::string container_name_;
public:
    explicit PodmanRestartContainerCommand(const std::string& container_name_);
    ~PodmanRestartContainerCommand() = default;

    bool Execute() const override;
};
