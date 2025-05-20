# Container Manager

**Container Manager** is an open-source C++ library and service for managing containers across multiple runtimes such as Docker and Podman. It provides a unified API and extensible architecture for creating, starting, stopping, restarting, and removing containers, with support for CLI command, HTTP-based control, and multiple IPC protocols. The project is modular, production-ready, and designed for extensibility.

## Features

- **Unified Container Management:**  
  Manage Docker and Podman containers through a single interface.

- **Extensible Command Pattern:**  
  Easily add support for new runtimes or operations using the command pattern.

- **Multi-Protocol Support:**  
  Supports REST (HTTP/JSON or Protobuf), MQTT, POSIX Message Queue, and D-Bus (session bus) for remote management.  
  **Pluggable protocol architecture:** Easily extend to support gRPC (planned), or other IPC/RPC mechanisms.

- **Flexible Data Formats:**  
  Incoming data can be JSON **or Protocol Buffers (protobuf)** for high-performance or strongly-typed APIs.  
  **gRPC/Protobuf support is planned for the gRPC protocol.**

- **HTTP API Server:**  
  Built-in HTTP server for remote container management via JSON or Protobuf requests.

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

See the [Architecture Documentation](docs/architecture.md) for detailed diagrams and explanations.

```
App/
├── api/        # HTTP server, MQTT subscriber, Message Queue and D-Bus consumer (and future protocol handlers)
├── core/       # Business logic (service layer)
├── database/   # Database interface and Redis implementation (pluggable)
├── executor/   # Request executors (JSON, Protobuf, etc.)
├── runtime/    # Command pattern implementations for Docker, Podman, etc.
├── utils/      # Common utilities (thread pool, logging, etc.)
├── main.cpp    # Application entry point
└── third_party/# External dependencies (excluded from docs/build)
```

- **Command Pattern:**  
  All container operations are encapsulated as command objects, making it easy to extend and maintain.

- **Service Layer:**  
  The `ContainerServiceHandler` coordinates requests, runtime checks, and command execution.

- **API Layer:**  
  The `HttpServer` exposes a RESTful API for remote management (now supports both JSON and Protobuf).  
  The `MosquittoMqttSubscriber` subscribes to MQTT topics for remote management.  
  The `MessageQueueConsumer` listens for requests on a POSIX message queue.  
  The `DbusConsumer` listens for requests on the D-Bus **session bus** for user-level IPC.  
  The architecture is designed to support additional protocols (gRPC, etc.) by adding new API handlers.

- **Executor Layer:**  
  The `RequestExecutor` abstraction allows for different data formats (JSON, Protobuf, etc.).

- **Database Layer:**  
  The `IDatabaseHandler` interface allows you to swap Redis for any other backend (SQL, NoSQL, in-memory, etc.) with minimal code changes.

## Modularity & Build System

**Container Manager is fully modular.**  
You only need to install the dependencies for the protocols and data formats you want to use.  
**You do NOT need to install all protocol and data format libraries unless you want a full-featured build.**

### Protocols and Data Formats

- **Protocols:**
  - REST (HTTP/JSON or Protobuf)
  - MQTT
  - POSIX Message Queue
  - D-Bus (session bus)
  - gRPC (planned)
- **Data Formats:**
  - JSON (default)
  - Protobuf (optional, for binary serialization)

### CMake Feature Flags

You can enable or disable each protocol and data format using CMake flags:

- `ENABLE_REST` (REST/HTTP server)
- `ENABLE_MQTT` (MQTT subscriber)
- `ENABLE_MSGQUEUE` (POSIX Message Queue)
- `ENABLE_DBUS` (D-Bus consumer)
- `ENABLE_GRPC` (gRPC, planned)
- `ENABLE_PROTOBUF` (Protobuf support; if OFF, only JSON is used)

**All options are ON by default.**  
To build only what you need, set the flags before running CMake.

## Build Instructions

### 1. Clone and Prepare

```sh
git clone https://github.com/yourusername/container_manager.git
cd container_manager/App
mkdir build && cd build
```

### 2. Install Only What You Need

#### If you want only JSON over D-Bus:

- Install only:
  - [sdbus-c++](https://github.com/Kistler-Group/sdbus-cpp)
  - [nlohmann/json](https://github.com/nlohmann/json)
  - [glog](https://github.com/google/glog)
  - [cpp_redis](https://github.com/Cylix/cpp_redis)
- CMake:
  ```sh
  cmake .. -DENABLE_REST=OFF -DENABLE_MQTT=OFF -DENABLE_MSGQUEUE=OFF -DENABLE_DBUS=ON -DENABLE_GRPC=OFF -DENABLE_PROTOBUF=OFF
  make
  ```

#### If you want only Protobuf over Message Queue:

- Install only:
  - [nlohmann/json](https://github.com/nlohmann/json) (for config)
  - [glog](https://github.com/google/glog)
  - [cpp_redis](https://github.com/Cylix/cpp_redis)
  - Protobuf (`libprotobuf-dev`, `protobuf-compiler`)
- CMake:
  ```sh
  cmake .. -DENABLE_REST=OFF -DENABLE_MQTT=OFF -DENABLE_MSGQUEUE=ON -DENABLE_DBUS=OFF -DENABLE_GRPC=OFF -DENABLE_PROTOBUF=ON
  make
  ```

#### If you want REST and MQTT with JSON only:

- Install:
  - [nlohmann/json](https://github.com/nlohmann/json)
  - [glog](https://github.com/google/glog)
  - [cpp_redis](https://github.com/Cylix/cpp_redis)
  - [mosquitto](https://github.com/eclipse-mosquitto/mosquitto)
  - [httplib (included as third_party)](https://github.com/yhirose/cpp-httplib)
- CMake:
  ```sh
  cmake .. -DENABLE_REST=ON -DENABLE_MQTT=ON -DENABLE_MSGQUEUE=OFF -DENABLE_DBUS=OFF -DENABLE_GRPC=OFF -DENABLE_PROTOBUF=OFF
  make
  ```

#### If you want all protocols with JSON format (full build):

- Install all dependencies listed in the prerequisites.
- CMake (default):
  ```sh
  cmake ..
  make
  ```

#### If you want all protocols with Protobuf only (no JSON):

- Install all protocol dependencies plus Protobuf.
- CMake:
  ```sh
  cmake .. -DENABLE_REST=ON -DENABLE_MQTT=ON -DENABLE_MSGQUEUE=ON -DENABLE_DBUS=ON -DENABLE_GRPC=ON -DENABLE_PROTOBUF=ON
  make
  ```

### 3. Summary Table

| Protocol/Data Format | CMake Flag         | Required Packages                                |
| -------------------- | ------------------ | ------------------------------------------------ |
| REST (HTTP/JSON)     | ENABLE_REST=ON     | nlohmann_json, glog, cpp_redis, httplib          |
| MQTT                 | ENABLE_MQTT=ON     | mosquitto, nlohmann_json, glog, cpp_redis        |
| Message Queue        | ENABLE_MSGQUEUE=ON | nlohmann_json, glog, cpp_redis                   |
| D-Bus                | ENABLE_DBUS=ON     | sdbus-c++, nlohmann_json, glog, cpp_redis        |
| gRPC (planned)       | ENABLE_GRPC=ON     | grpc++, protobuf, nlohmann_json, glog, cpp_redis |
| Protobuf             | ENABLE_PROTOBUF=ON | protobuf                                         |

- **JSON** is always available (unless you only enable Protobuf).
- **Protobuf** is optional and only needed if you want binary serialization.

### 4. General Build Steps

```sh
cmake .. <your-flags>
make -j$(nproc)
```

### 5. Examples

- **Enable only D-Bus with JSON:**
  ```sh
  cmake .. -DENABLE_REST=OFF -DENABLE_MQTT=OFF -DENABLE_MSGQUEUE=OFF -DENABLE_DBUS=ON -DENABLE_GRPC=OFF -DENABLE_PROTOBUF=OFF
  make
  ```
- **Enable REST, MQTT, and Protobuf:**
  ```sh
  cmake .. -DENABLE_REST=ON -DENABLE_MQTT=ON -DENABLE_MSGQUEUE=OFF -DENABLE_DBUS=OFF -DENABLE_GRPC=OFF -DENABLE_PROTOBUF=ON
  make
  ```
- **Enable all protocols with Protobuf only:**
  ```sh
  cmake .. -DENABLE_REST=ON -DENABLE_MQTT=ON -DENABLE_MSGQUEUE=ON -DENABLE_DBUS=ON -DENABLE_GRPC=ON -DENABLE_PROTOBUF=ON
  make
  ```
- **Full build (all protocols with JSON format):**
  ```sh
  cmake ..
  make
  ```

**This modular build system ensures you only install and build what you need for your use case.**  
**Check the CMake output for any missing dependencies based on your selected flags.**

## Running the Server

```sh
./CM
```

The server will start and listen on the enabled protocols and data formats as per your build configuration.

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

### Example Protobuf Request

- See `executor/proto/container_manager.proto` for the schema.
- Use the generated Python or C++ classes to serialize/deserialize requests.
- Example Python code to send a Protobuf request via Message Queue:

```python
import posix_ipc
import container_manager_pb2  # Generated by protoc

req = container_manager_pb2.ContainerRequest()
req.runtime = "docker"
req.operation = "create"
param = req.parameters.add()
param.container_name = "my_nginx"
param.cpus = "0.5"
param.memory = "128"
param.pids = "10"
param.restart_policy = "unless-stopped"
param.image_name = "nginx:latest"

payload = req.SerializeToString()
mq = posix_ipc.MessageQueue('/container_manager_queue', posix_ipc.O_CREAT)
mq.send(payload)
```

### HTTP Endpoint

- **POST** `/execute`
  - Send a JSON or Protobuf request as shown above to perform container operations.
  - For Protobuf, set the header: `Content-Type: application/octet-stream`

### MQTT Usage

- **Topic:** `container/execute`
- **Publish Example (JSON):**
  ```sh
  mosquitto_pub -h localhost -p 1883 -t container/execute -m '{ "runtime":"docker", "operation":"create", "parameters":[{"container_name":"my_nginx", "cpus":"0.5", "memory":"128", "pids":"10", "restart_policy":"unless-stopped", "image_name":"nginx:latest"}]}'
  ```
- **Publish Example (Protobuf):**
  Use the generated Protobuf class to serialize and send the message as binary.

### POSIX Message Queue Usage

- **Queue Name:** `/container_manager_queue`
- **Send Example (Python, JSON):**
  ```python
  import posix_ipc
  mq = posix_ipc.MessageQueue('/container_manager_queue', posix_ipc.O_CREAT)
  mq.send('{"runtime": "podman", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "64", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
  ```
- **Send Example (Python, Protobuf):**
  See the Protobuf example above.

### D-Bus (Session Bus) Usage

- **Bus Name:** `org.container.manager`
- **Object Path:** `/org/container/manager`
- **Interface:** `org.container.manager`
- **Method:** `Execute` (accepts a JSON string or Protobuf bytes as the first argument)

**Python Example (JSON):**

```python
import dbus
bus = dbus.SessionBus()
proxy = bus.get_object('org.container.manager', '/org/container/manager')
iface = dbus.Interface(proxy, dbus_interface='org.container.manager')
iface.Execute('{"runtime": "docker", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "128", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
```

**Python Example (Protobuf):**

```python
iface.Execute(req.SerializeToString())
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

- **executor/inc/proto_request_executor.hpp:**  
  Handles Protobuf requests.

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
- Select data format: JSON or Protobuf
- Fill in container parameters (runtime, operation, resources, image, etc.)
- Send requests directly to the backend via REST, MQTT, Message Queue, or D-Bus

### Requirements

- Python 3.7+
- [requests](https://pypi.org/project/requests/)
- [paho-mqtt](https://pypi.org/project/paho-mqtt/)
- [posix_ipc](https://pypi.org/project/posix_ipc/)
- [dbus-python](https://pypi.org/project/dbus-python/)
- [protobuf](https://pypi.org/project/protobuf/)
- Tkinter (usually included with Python)

Install dependencies:

```sh
pip install requests paho-mqtt posix_ipc dbus-python protobuf
```

### Usage

```sh
cd ui
python container_creator_app.py
```

1. Select the protocol (REST, MQTT, MessageQueue, or DBus).
2. Select the data format (JSON or Proto).
3. Fill in the container parameters.
4. Click **Send** to send the request to the backend.
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
- [protobuf](https://developers.google.com/protocol-buffers)
