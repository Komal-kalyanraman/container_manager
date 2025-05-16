#pragma once

#include <string>
#include <string_view>

const int kMaxThreadPoolSize = 4;
const std::string kContainerManagerLogName = "cm_log";


const int kHttpServerPort = 5000;
const std::string kHttpServerHost = "0.0.0.0";

const std::string kEmptyString = "";

struct RuntimeName {
    static constexpr std::string_view Docker      = "docker";
    static constexpr std::string_view Podman      = "podman";
    static constexpr std::string_view DockerApi   = "docker-api";
    static constexpr std::string_view PodmanApi   = "podman-api";
};

struct CommandName {
    static constexpr std::string_view RuntimeStatus      = "available";
    static constexpr std::string_view CreateContainer    = "create";
    static constexpr std::string_view StartContainer     = "start";
    static constexpr std::string_view StopContainer      = "stop";
    static constexpr std::string_view RestartContainer   = "restart";
    static constexpr std::string_view RemoveContainer    = "remove";
};
