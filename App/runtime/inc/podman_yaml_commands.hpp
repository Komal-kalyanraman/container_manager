#pragma once

#include <string>
#include "inc/status.hpp"
#include "inc/commands.hpp"

/**
 * @class PodmanYamlPlayCommand
 * @brief Command to play a Podman YAML file (create resources).
 */
class PodmanYamlPlayCommand : public Command {
private:
    std::string yaml_file_path_; ///< Path to the YAML file.
public:
    /**
     * @brief Constructs a PodmanYamlPlayCommand.
     * @param yaml_file_path Path to the YAML file to play.
     */
    explicit PodmanYamlPlayCommand(const std::string& yaml_file_path);

    /**
     * @brief Destructor.
     */
    ~PodmanYamlPlayCommand() = default;

    /**
     * @brief Executes the command to play the YAML file.
     * @return Status indicating if the operation was successful.
     */
    Status Execute() const override;
};

/**
 * @class PodmanYamlDeleteCommand
 * @brief Command to delete resources defined in a Podman YAML file.
 */
class PodmanYamlDeleteCommand : public Command {
private:
    std::string yaml_file_path_; ///< Path to the YAML file.
public:
    /**
     * @brief Constructs a PodmanYamlDeleteCommand.
     * @param yaml_file_path Path to the YAML file to delete resources.
     */
    explicit PodmanYamlDeleteCommand(const std::string& yaml_file_path);

    /**
     * @brief Destructor.
     */
    ~PodmanYamlDeleteCommand() = default;

    /**
     * @brief Executes the command to delete resources defined in the YAML file.
     * @return Status indicating if the operation was successful.
     */
    Status Execute() const override;
};