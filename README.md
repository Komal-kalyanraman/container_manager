# Container Manager

**Container Manager** is a modular, production-ready C++ service for unified container management across Docker, Podman, and more (planned). It supports REST, MQTT, MQ, D-Bus, gRPC (planned), Docker/Podman CLI, HTTP-API with incoming JSON & Protobuf data. Features: extensible architecture, pluggable database backend (embedded by default, Redis optional), robust logging, thread pool for all protocols, and enterprise-grade security with AES-256-GCM or ChaCha20-Poly1305 encryption.

> **This is a reference architecture and template project** designed to demonstrate production-grade C++ patterns for container management systems. Clone, modify, and extend as needed for your specific requirements.

## Use Cases

- **Need D-Bus + Podman REST API + Protobuf + AES encryption?** Clone and modify.
- **Want MQTT + Docker CLI + JSON + embedded database?** Clone and configure.
- **Building a container orchestrator with custom runtime?** Extend the runtime layer.

## Features

- **Unified Container Management:**  
  Manage Docker and Podman containers through a single interface.

- **Extensible Command Pattern:**  
  Easily add support for new runtimes or operations using the command pattern.

- **Docker API & Podman API Support:**  
  Manage containers via CLI or REST APIs over Unix sockets for rootless/user-level/remote management.

- **Multi-Protocol Support:**  
  REST (HTTP/JSON or Protobuf), MQTT, POSIX Message Queue, and D-Bus (session bus).  
  **Pluggable protocol architecture:** Easily extend to support gRPC (planned), or other IPC/RPC mechanisms.

- **Flexible Data Formats:**  
  JSON **or Protocol Buffers (protobuf)** for high-performance or strongly-typed APIs.

- **Thread Pool:**  
  Efficient request handling using a configurable thread pool.

- **Database Integration:**  
  **Embedded database backend by default**; **Redis optional** via CMake flag.  
  **Pluggable backend:** Implement the `IDatabaseHandler` interface for custom databases.

- **Logging:**  
  Integrated with Google glog for robust logging.

- **Security:**  
  **AES-256-GCM and ChaCha20-Poly1305 encryption** for all protocols (REST, MQTT, D-Bus, MQ).  
  See [Security Architecture](#security-architecture).

- **Production-Grade Documentation:**  
  Doxygen-based documentation with diagrams and source browsing.

- **Open Source:**  
  Licensed under the MIT License.

## Security Architecture

**Container Manager v0.7.0** introduces a robust security layer:

- **AES-256-GCM and ChaCha20-Poly1305 encryption** are supported for all protocols.
  - Payloads are encrypted on the client side and decrypted transparently by the backend.
  - The backend auto-detects and decrypts encrypted payloads, regardless of protocol or data format (JSON or Protobuf).
  - **Detection and decryption are handled in the executor layer**, so all protocol handlers remain generic and agnostic to encryption.
- **Key management:**
  - The AES and ChaCha20 keys are stored as hex-encoded files in `storage/security/`:
    - `storage/security/aes_key.txt` (64 hex chars, 32 bytes)
    - `storage/security/chacha20_key.txt` (64 hex chars, 32 bytes)
  - The backend loads the key at runtime from these files.
  - For production, use environment variables or a secure key vault for even better security.
- **Encryption detection:**
  - The executor layer auto-detects whether the incoming payload is encrypted:
    - For JSON: If the payload is not valid JSON, it attempts decryption.
    - For Protobuf: If the payload cannot be parsed as a valid Protobuf message, it attempts decryption and then parses again.
  - This logic is protocol-agnostic and works for REST, MQTT, MQ, and D-Bus.

### Sending Encrypted Payloads

- **REST:** Send encrypted bytes directly as the request body with `Content-Type: application/octet-stream`.
- **MQTT and D-Bus:**  
  Encrypted payloads must be **Base64-encoded** before sending.  
  The backend will automatically decode and decrypt.

#### Example: Encrypted JSON or Protobuf Request (Python)

```python
from Crypto.Cipher import AES
from Crypto.Random import get_random_bytes
import requests
import json

def load_aes_key(path="../key_storage/aes_key.txt"):
    with open(path, "r") as f:
        key_hex = f.read().strip()
    return bytes.fromhex(key_hex)

AES_KEY = load_aes_key()

def encrypt_payload(data_bytes):
    iv = get_random_bytes(12)
    cipher = AES.new(AES_KEY, AES.MODE_GCM, nonce=iv)
    ciphertext, tag = cipher.encrypt_and_digest(data_bytes)
    return iv + tag + ciphertext

payload = {"runtime": "docker", "operation": "available", "parameters": []}
payload_bytes = json.dumps(payload).encode('utf-8')
encrypted_payload = encrypt_payload(payload_bytes)

response = requests.post(
    'http://localhost:5000/execute',
    data=encrypted_payload,
    headers={'Content-Type': 'application/octet-stream'}
)
```

#### Example: D-Bus and MQTT Clients (Python, Encrypted Payload)

```python
import base64
encrypted_payload = ... # bytes from AES-GCM encryption
# For D-Bus or MQTT, send as Base64-encoded string:
b64_payload = base64.b64encode(encrypted_payload).decode()
# Send b64_payload via D-Bus or MQTT
```

> **Note:**  
> The backend will automatically decode Base64 and decrypt for D-Bus and MQTT protocols.

## Database Backend Selection

Container Manager supports two database backends:

- **Embedded Database (default):**  
  Lightweight, fixed-size, in-memory key-value store.  
  No external dependencies required.

- **Redis Database (optional):**  
  Use Redis for scalable, persistent, and distributed deployments.  
  Requires [cpp_redis](https://github.com/Cylix/cpp_redis) and a running Redis server.
  Database backend selection is controlled via the `ENABLE_REDIS` CMake flag.

  > **Note:** When `ENABLE_REDIS=ON`, you must have `cpp_redis` and a Redis server available.

## Architecture Overview

For a comprehensive overview of the system design, security, and extensibility, please see the [Architecture Documentation](docs/architecture/architecture.md).

```
App/
├── api/        # HTTP server, MQTT subscriber, Message Queue and D-Bus consumer (and future protocol handlers)
├── core/       # Business logic (service layer)
├── database/   # Database interface, embedded and Redis implementations (pluggable)
├── executor/   # Request executors (JSON, Protobuf, handle decryption)
├── runtime/    # Command pattern implementations for Docker CLI, Podman CLI, Docker API, Podman API, etc.
├── utils/      # Common utilities (thread pool, logging, etc.)
├── security/   # Security providers (AES-GCM, ChaCha20, Null)
├── main.cpp    # Application entry point
└── third_party/# External dependencies (excluded from docs/build)
```

## Modularity & Build System

**Container Manager is fully modular.**  
You only need to install the dependencies for the protocols and data formats you want to use.

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

### Supported Runtimes

- **docker:** Manage containers using the Docker CLI.
- **podman:** Manage containers using the Podman CLI.
- **docker-api:** Manage containers using the Docker REST API over Unix socket.
- **podman-api:** Manage containers using the Podman REST API over Unix socket.

> **Disclaimer:**  
> The `podman-api` runtime is not extensively tested. Use with caution and report any issues you encounter.

### CMake Feature Flags

You can enable or disable each protocol, data format, and security feature using CMake flags:

- `ENABLE_REST` (REST/HTTP server)
- `ENABLE_MQTT` (MQTT subscriber)
- `ENABLE_MSGQUEUE` (POSIX Message Queue)
- `ENABLE_DBUS` (D-Bus consumer)
- `ENABLE_GRPC` (gRPC, planned)
- `ENABLE_PROTOBUF` (Protobuf support; if OFF, only JSON is used)
- `ENABLE_REDIS` (Redis database backend; if OFF, embedded database is used)
- `ENABLE_ENCRYPTION` (Enable encryption for all protocols; ON by default)
- `SECURITY_ALGORITHM` (Select encryption algorithm: `AES_GCM` or `CHACHA20`; default is `AES_GCM`)

## Build Examples

**Examples:**

- **Enable Protobuf, MQTT, Message Queue, REST, Redis, and ChaCha20 encryption:**

  ```sh
  cmake .. \
    -DENABLE_PROTOBUF=ON \
    -DENABLE_MQTT=ON \
    -DENABLE_MSGQUEUE=ON \
    -DENABLE_REST=ON \
    -DENABLE_REDIS=ON \
    -DENABLE_ENCRYPTION=ON \
    -DSECURITY_ALGORITHM=CHACHA20
  make
  ```

- **Disable Protobuf, Redis, encryption but Enable MQTT, Message Queue, REST:**

  ```sh
  cmake .. \
    -DENABLE_MQTT=ON \
    -DENABLE_MSGQUEUE=ON \
    -DENABLE_REST=ON \
    -DENABLE_ENCRYPTION=OFF
  make
  ```

> **Note:**  
> The encryption key files must be present in `storage/security/` as described above.  
> The selected algorithm must match between the backend and any client sending encrypted payloads.

## Running the Server

```sh
./CM
```

The server will start and listen on the enabled protocols and data formats as per your build configuration.

## API Usage

### Example JSON Request

```json
{
  "runtime": "docker-api",
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

- Use `"runtime": "docker"` or `"podman"` for CLI-based management.
- Use `"runtime": "docker-api"` or `"podman-api"` for REST API-based management.

### Example Protobuf Request

- See `executor/proto/container_manager.proto` for the schema.
- Use the generated Python or C++ classes to serialize/deserialize requests.
- Example Python code to send a Protobuf request via Message Queue:

```python
import posix_ipc
import container_manager_pb2  # Generated by protoc

req = container_manager_pb2.ContainerRequest()
req.runtime = "docker-api"
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
  - For encrypted payloads, set the header: `Content-Type: application/octet-stream` and send the encrypted bytes.

### MQTT Usage

- **Topic:** `container/execute`
- **Publish Example (JSON):**
  ```sh
  mosquitto_pub -h localhost -p 1883 -t container/execute -m '{ "runtime":"docker-api", "operation":"create", "parameters":[{"container_name":"my_nginx", "cpus":"0.5", "memory":"128", "pids":"10", "restart_policy":"unless-stopped", "image_name":"nginx:latest"}]}'
  ```
- **Publish Example (Protobuf or Encrypted):**
  Use the generated Protobuf class to serialize and send the message as binary.  
  If encryption is enabled, Base64-encode the encrypted payload before publishing.

### POSIX Message Queue Usage

- **Queue Name:** `/container_manager_queue`
- **Send Example (Python, JSON):**
  ```python
  import posix_ipc
  mq = posix_ipc.MessageQueue('/container_manager_queue', posix_ipc.O_CREAT)
  mq.send('{"runtime": "podman", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "64", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
  ```
- **Send Example (Python, Protobuf or Encrypted):**
  See the Protobuf and encryption examples above.

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
iface.Execute('{"runtime": "docker-api", "operation": "create", "parameters": [{"container_name": "my_nginx", "cpus": "0.5", "memory": "128", "pids": "10", "restart_policy": "unless-stopped", "image_name": "nginx:latest"}]}')
```

**Python Example (Protobuf or Encrypted):**

```python
import base64
encrypted_payload = ... # bytes from AES-GCM encryption
iface.Execute(base64.b64encode(encrypted_payload).decode())
```

**Note:**

- The D-Bus consumer uses the **session bus** for user applications.
- No root privileges or system-wide D-Bus policy is required.
- Encrypted payloads must be Base64-encoded before sending over D-Bus.

## Python UI: Container Creator

A cross-platform Python GUI tool is provided for easily sending container management requests to the Container Manager backend.

### Features

- Select protocol: REST (HTTP), MQTT, POSIX Message Queue, or D-Bus (session bus)
- Select data format: JSON or Protobuf
- Select runtime: Docker, Podman, Docker API, or Podman API
- Fill in container parameters (runtime, operation, resources, image, etc.)
- Enable/disable AES-GCM or ChaCha20 encryption
- Send requests directly to the backend via REST, MQTT, Message Queue, or D-Bus

### Requirements

- Python 3.7+
- [requests](https://pypi.org/project/requests/)
- [paho-mqtt](https://pypi.org/project/paho-mqtt/)
- [posix_ipc](https://pypi.org/project/posix_ipc/)
- [dbus-python](https://pypi.org/project/dbus-python/)
- [protobuf](https://pypi.org/project/protobuf/)
- [pycryptodome](https://pypi.org/project/pycryptodome/)
- Tkinter (usually included with Python)

Install dependencies:

```sh
pip install requests paho-mqtt posix_ipc dbus-python protobuf pycryptodome
```

### Usage

```sh
cd ui
python container_creator_app.py
```

1. Select the protocol (REST, MQTT, MessageQueue, or DBus).
2. Select the data format (JSON or Proto).
3. Select the runtime (docker, podman, docker-api, podman-api).
4. Fill in the container parameters.
5. Enable encryption if desired.
6. Click **Send** to send the request to the backend.

The UI will show a confirmation or error message after sending the request.

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
