#pragma once

#include <string>
#include "inc/status.hpp"
#include "inc/commands.hpp"
#include "inc/container_request.hpp"

/**
 * @class PodmanYamlPlayCommand
 * @brief Command to play a Podman YAML file (create resources).
 */
class PodmanYamlPlayCommand : public Command {
private:
    ContainerRequest req_; ///< Incoming container request.
public:
    /**
     * @brief Constructs a PodmanYamlPlayCommand.
     * @param req The container request containing runtime parameters.
     */
    explicit PodmanYamlPlayCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanYamlPlayCommand() = default;

    /**
     * @brief Executes the command to play the YAML file.
     * @return Status indicating if the operation was successful.
     */
    Status Execute() const override;

    /**
     * @brief Generates a YAML file based on the container request.
     * @return Path to the generated YAML file.
     */
    std::string GenerateYamlFile() const;
};

/**
 * @class PodmanYamlDeleteCommand
 * @brief Command to delete resources defined in a Podman YAML file.
 */
class PodmanYamlDeleteCommand : public Command {
private:
    ContainerRequest req_; ///< Incoming container request.
public:
    /**
     * @brief Constructs a PodmanYamlDeleteCommand.
     * @param req The container request containing runtime parameters.
     */
    explicit PodmanYamlDeleteCommand(const ContainerRequest& req);

    /**
     * @brief Destructor.
     */
    ~PodmanYamlDeleteCommand() = default;

    /**
     * @brief Executes the command to delete resources defined in the YAML file.
     * @return Status indicating if the operation was successful.
     */
    Status Execute() const override;

    /**
     * @brief Generates a YAML file based on the container request.
     * @return Path to the generated YAML file.
     */
    std::string GenerateYamlFile() const;
};