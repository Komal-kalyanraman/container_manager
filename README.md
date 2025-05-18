# Container Manager

**Container Manager** is an open-source C++ library and service for managing containers across multiple runtimes such as Docker and Podman. It provides a unified API and extensible architecture for creating, starting, stopping, restarting, and removing containers, with support for both CLI command and HTTP-based control. The project is modular, production-ready, and designed for extensibility.

## Features

- **Unified Container Management:**  
  Manage Docker and Podman containers through a single interface.

- **Extensible Command Pattern:**  
  Easily add support for new runtimes or operations using the command pattern.

- **Multi-Protocol Support:**  
  Supports REST (HTTP/JSON), MQTT, POSIX Message Queue, and D-Bus (session bus) for remote management.  
  **Pluggable protocol architecture:** Easily extend to support gRPC (planned), or other IPC/RPC mechanisms.

- **Flexible Data Formats:**  
  Incoming data is currently JSON, but the architecture supports adding other formats such as Protocol Buffers (protobuf) for high-performance or strongly-typed APIs.  
  **gRPC/Protobuf support is planned and will be documented once implemented.**

- **HTTP API Server:**  
  Built-in HTTP server for remote container management via JSON requests.

- **MQTT Subscriber:**  
  Built-in MQTT subscriber for receiving and processing container management requests via MQTT topics over mosquitto mqtt broker.

- **POSIX Message Queue Consumer:**  
  Built-in consumer for POSIX message queues, allowing integration with other IPC systems.

- **D-Bus Consumer (Session Bus):**  
  Built-in D-Bus consumer using the **session bus** for user applications, allowing desktop or user-level apps to communicate with the backend without root privileges or system-wide D-Bus policies.

- **Thread Pool:**  
  Efficient request handling using a configurable thread pool.

- **Database Integration:**  
  Uses Redis for storing container metadata and state.  
  **Pluggable database backend:** Swap Redis for any other database by implementing the `IDatabaseHandler` interface.

- **Logging:**  
  Integrated with Google glog for robust logging.

- **Production-Grade Documentation:**  
  Doxygen-based documentation with diagrams and source browsing.

- **Open Source:**  
  Licensed under the MIT License.

## Architecture Overview

```

App/
├── api/ # HTTP server, MQTT subscriber, Message Queue, and D-Bus consumer (and future protocol handlers)
├── core/ # Business logic (service layer)
├── database/ # Database interface and Redis implementation (pluggable)
├── executor/ # Request executors (JSON, future: Protobuf, etc.)
├── runtime/ # Command pattern implementations for Docker, Podman, etc.
├── utils/ # Common utilities (thread pool, logging, etc.)
├── main.cpp # Application entry point
└── third_party/ # External dependencies (excluded from docs/build)

```

- **Command Pattern:**
  All container operations are encapsulated as command objects, making it easy to extend and maintain.

- **Service Layer:**
  The `ContainerServiceHandler` coordinates requests, runtime checks, and command execution.

- **API Layer:**
  The `HttpServer` exposes a RESTful API for remote management.
  The `MosquittoMqttSubscriber` subscribes to MQTT topics for remote management.
  The `MessageQueueConsumer` listens for requests on a POSIX message queue.
  The `DbusConsumer` listens for requests on the D-Bus **session bus** for user-level IPC.
  The architecture is designed to support additional protocols (gRPC, etc.) by adding new API handlers.

- **Executor Layer:**
  The `RequestExecutor` abstraction allows for different data formats (currently JSON, future: Protobuf, etc.).

- **Database Layer:**
  The `IDatabaseHandler` interface allows you to swap Redis for any other backend (SQL, NoSQL, in-memory, etc.) with minimal code changes.

## Quick Start

### Prerequisites

- C++17 compiler
- CMake 3.10+
- [glog](https://github.com/google/glog)
- [cpp_redis](https://github.com/Cylix/cpp_redis)
- [nlohmann/json](https://github.com/nlohmann/json)
- [mosquitto](https://github.com/eclipse-mosquitto/mosquitto)
- [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp) (for D-Bus support)
- Docker and/or Podman installed and running

#### (Planned) gRPC/Protobuf Support

> **Note:**
> gRPC support is planned but not yet implemented in the API layer.
> The build system is ready for gRPC/Protobuf integration.

- **Protobuf:**
  - Library: `libprotobuf-dev` (version 3.6.1 recommended for Ubuntu 20.04)
  - Compiler: `protobuf-compiler` (ensure the version matches the library, e.g., 3.6.1)
- **gRPC:**
  - Library: `libgrpc++-dev` (version 1.16.1 recommended for Ubuntu 20.04)
  - Plugin: `protobuf-compiler-grpc`
- **Install on Ubuntu:**

  ```sh
  sudo apt update
  sudo apt install libgrpc++-dev protobuf-compiler-grpc libprotobuf-dev protobuf-compiler
  ```

- **Version Check:**
  ```sh
  protoc --version
  # Should match: libprotoc 3.6.1
  ```

### Build Instructions

```sh
git clone https://github.com/yourusername/container_manager.git
cd container_manager/App
mkdir build && cd build
cmake ..
make
```

### Running the Server

```sh
./CM
```

The server will start and listen on the default port `5000` for HTTP, subscribe to the MQTT topic `container/execute` on port `1883`, listen on the POSIX message queue `/container_manager_queue`, and register a D-Bus object on the **session bus** as `org.container.manager` at `/org/container/manager`.

## API Usage

### Example JSON Request

```json
{
  "runtime": "docker",
  "operation": "create",
  "parameters": [
    {
      "container_name": "my_nginx",
      "cpus": "0.5",
      "memory": "128",
      "pids": "10",
      "restart_policy": "unless-stopped",
      "image_name": "nginx:latest"
    }
  ]
}
```

### HTTP Endpoint

- **POST** `/execute`
  Send a JSON request as shown above to perform container operations.

### MQTT Usage

- **Topic:** `container/execute`
- **Publish Example:**
  ```sh
  mosquitto_pub -h localhost -p 1883 -t container/execute -m '{ "runtime":"docker", "operation":"create", "parameters":[{"container_name":"my_nginx", "cpus":"0.5", "memory":"128", "pids":"10", "restart_policy":"unless-stopped", "image_name":"nginx:latest"}]}'
  ```

### POSIX Message Queue Usage

- **Queue Name:** `/container_manager_queue`
- **Send Example (Python):**
  ```python
  import posix_ipc
  mq = posix_ipc.MessageQueue('/container_manager_queue', posix_ipc.O_CREAT)
  mq.send('{"runtime": "podman", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "64", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
  ```

### D-Bus (Session Bus) Usage

- **Bus Name:** `org.container.manager`
- **Object Path:** `/org/container/manager`
- **Interface:** `org.container.manager`
- **Method:** `Execute` (accepts a JSON string as the first argument)

**Python Example:**

```python
import dbus
bus = dbus.SessionBus()
proxy = bus.get_object('org.container.manager', '/org/container/manager')
iface = dbus.Interface(proxy, dbus_interface='org.container.manager')
iface.Execute('{"runtime": "docker", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "128", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
```

**Note:**

- The D-Bus consumer uses the **session bus** for user applications.
- No root privileges or system-wide D-Bus policy is required.

## Code Structure

- **main.cpp:**
  Initializes logging, clears the database, and starts the HTTP server (in a thread), MQTT subscriber, Message Queue consumer, and D-Bus consumer (session bus).

- **api/inc/http_server.hpp:**
  HTTP server for processing incoming requests.
  _Pluggable protocol support: Add new handlers for MQ, D-Bus, MQTT, etc._

- **api/inc/mosquitto_mqtt_subscriber.hpp:**
  MQTT subscriber for processing incoming MQTT messages.

- **api/inc/posix_message_queue_consumer.hpp:**
  POSIX message queue consumer for processing incoming queue messages.

- **api/inc/dbus_consumer.hpp:**
  D-Bus consumer for processing incoming requests over the session bus.

- **core/inc/container_service.hpp:**
  Service layer for business logic and command dispatch.

- **database/inc/database_interface.hpp:**
  Abstract interface for database operations.
  _Pluggable backend: Implement this interface to use a different database._

- **database/inc/redis_database.hpp:**
  Redis-based implementation of the database interface.

- **executor/inc/json_request_executor.hpp:**
  Handles JSON requests.
  _Extensible: Add new executors for Protobuf or other formats._

- **runtime/inc/docker_commands.hpp, podman_commands.hpp:**
  Command classes for Docker and Podman operations.

- **utils/inc/threadpool.hpp:**
  Thread pool utility for concurrent request handling.

- **utils/inc/logging.hpp:**
  Logging macros and configuration.

## Python UI: Container Creator

A cross-platform Python GUI tool is provided for easily sending container management requests to the Container Manager backend.

### Features

- Select protocol: REST (HTTP), MQTT, POSIX Message Queue, or D-Bus (session bus)
- Fill in container parameters (runtime, operation, resources, image, etc.)
- Send JSON requests directly to the backend via REST, MQTT, Message Queue, or D-Bus

### Requirements

- Python 3.7+
- [requests](https://pypi.org/project/requests/)
- [paho-mqtt](https://pypi.org/project/paho-mqtt/)
- [posix_ipc](https://pypi.org/project/posix_ipc/)
- [dbus-python](https://pypi.org/project/dbus-python/)
- Tkinter (usually included with Python)

Install dependencies:

```sh
pip install requests paho-mqtt posix_ipc dbus-python
```

### Usage

```sh
cd ui
python container_creator_app.py
```

1. Select the protocol (REST, MQTT, MessageQueue, or DBus).
2. Fill in the container parameters.
3. Click **Send** to send the JSON to the backend.
   - For REST, the backend must be running and listening on the specified port.
   - For MQTT, the broker must be running and accessible.
   - For MessageQueue, the backend must be running and listening on the configured POSIX message queue.
   - For D-Bus, the backend must be running and registered on the session bus.

The UI will show a confirmation or error message after sending the request.

**This tool is ideal for testing, demos, and rapid prototyping with the Container Manager backend.**

## Documentation

- **Doxygen HTML Docs:**
  After building, open `docs/doxygen/html/index.html` in your browser.

- **Generating Docs:**
  ```sh
  doxygen Doxyfile
  ```

## Extending the Project

- **Add a new runtime:**
  Implement new command classes in `App/runtime/` and register them in `CommandFactory`.

- **Add a new operation:**
  Extend the command pattern and update the service layer.

- **Add a new protocol:**
  Implement a new API handler in `App/api/` (e.g., for Message Queue, D-Bus, gRPC, MQTT) and register it in the main application.

  - **gRPC support:** Planned. The CMake setup already supports `.proto` file compilation and linking.
    Once the gRPC server/client code is implemented, this section will be updated with usage and integration details.

- **Add a new data format:**
  Implement a new `RequestExecutor` (e.g., for Protobuf) in `App/executor/` and update the API handler to use it.

- **Change database backend:**
  Implement the `IDatabaseHandler` interface for your preferred database (SQL, NoSQL, etc.) and update the service to use it.

## Contributing

Contributions are welcome! Please open issues or pull requests for bug fixes, new features, or documentation improvements.

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgements

- [glog](https://github.com/google/glog)
- [cpp_redis](https://github.com/Cylix/cpp_redis)
- [nlohmann/json](https://github.com/nlohmann/json)
- [mosquitto](https://mosquitto.org/)
- [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp)
- [Doxygen](https://www.doxygen.nl/)
