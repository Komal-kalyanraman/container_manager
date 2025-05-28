set(APP_NAME cm_runtime)

add_library(${APP_NAME} STATIC
    src/command_factory.cpp
    src/podman_commands.cpp
    src/podman_api_commands.cpp
    src/docker_commands.cpp
    src/docker_api_commands.cpp
)

target_link_libraries(cm_runtime PUBLIC glog::glog cm_utils)