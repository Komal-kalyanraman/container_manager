set(APP_NAME cm_runtime)

add_library(${APP_NAME} STATIC
    src/cm_command_factory.cpp
    src/cm_podman_commands.cpp
    src/cm_podman_api_commands.cpp
    src/cm_docker_commands.cpp
    src/cm_docker_api_commands.cpp
)
