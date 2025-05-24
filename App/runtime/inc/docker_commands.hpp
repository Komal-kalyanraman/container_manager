#pragma once

#include <string>
#include <memory>
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

class DockerRuntimeAvailableCommand : public Command {
public:
    explicit DockerRuntimeAvailableCommand();
    ~DockerRuntimeAvailableCommand() = default;
    bool Execute() const override;
};

class DockerCreateContainerCommand : public Command {
private:
    ContainerRequest req_;
public:
    explicit DockerCreateContainerCommand(const ContainerRequest& req);
    ~DockerCreateContainerCommand() = default;
    bool Execute() const override;
};

class DockerStartContainerCommand : public Command {
private:
    ContainerRequest req_;
public:
    explicit DockerStartContainerCommand(const ContainerRequest& req);
    ~DockerStartContainerCommand() = default;
    bool Execute() const override;
};

class DockerStopContainerCommand : public Command {
private:
    ContainerRequest req_;
public:
    explicit DockerStopContainerCommand(const ContainerRequest& req);
    ~DockerStopContainerCommand() = default;
    bool Execute() const override;
};

class DockerRemoveContainerCommand : public Command {
private:
    ContainerRequest req_;
public:
    explicit DockerRemoveContainerCommand(const ContainerRequest& req);
    ~DockerRemoveContainerCommand() = default;
    bool Execute() const override;
};

class DockerRestartContainerCommand : public Command {
private:
    ContainerRequest req_;
public:
    explicit DockerRestartContainerCommand(const ContainerRequest& req);
    ~DockerRestartContainerCommand() = default;
    bool Execute() const override;
};