# Container Manager Architecture

## Overview

**Container Manager** is a modular, extensible, and production-ready C++ service designed to provide unified container management across multiple runtimes, including Docker and Podman, with support for additional engines planned. The system is architected for flexibility, security, and ease of integration in automotive environment deployments.

Container Manager abstracts the complexity of managing containers by exposing a unified API over multiple communication protocols—REST, MQTT, POSIX Message Queue, D-Bus, and (planned) gRPC. It supports both JSON and Protocol Buffers (Protobuf) as data formats, allowing seamless integration with a wide range of clients and systems.

**Key architectural highlights:**

- **Protocol-Agnostic Core:**  
  The business logic and command execution are completely decoupled from protocol handling, enabling easy addition of new communication methods without impacting core functionality.
- **Extensible Command Pattern:**  
  All container operations (create, start, stop, etc.) are encapsulated as command objects, making it straightforward to add new operations or support new container runtimes.
- **Pluggable Data Formats:**  
  Both JSON and Protobuf are supported for all protocols, with the ability to add more serialization formats as needed.
- **Pluggable Database Backend:**  
  The system supports both an embedded in-memory database (default) and Redis for scalable, production deployments. The database interface is abstracted, allowing further extension.
- **Enterprise-Grade Security:**  
  All incoming protocol payloads can be encrypted using AES-256-GCM or ChaCha20-Poly1305. The security layer is protocol-agnostic and auto-detects encrypted payloads, ensuring confidentiality and integrity for over-the-air (OTA) requests. Key management is handled via secure, hex-encoded files.
- **Thread Pool and Robust Logging:**  
  All protocol handlers leverage a shared thread pool for efficient, concurrent request processing. Logging is handled via Google glog for production-grade diagnostics.
- **Production-Ready and Configurable:**  
  All features (protocols, data formats, security, database backend) are enabled or disabled at build time via CMake flags, making the system highly configurable for different deployment scenarios.

**Intended Audience:**  
This architecture is aimed at developers, system integrators, and architects who require a secure, extensible, and maintainable foundation for building unified container orchestration solutions—especially in environments where protocol diversity, security, and modularity are critical.

**Design Philosophy:**

- **Separation of Concerns:** Each layer (API, executor, core, runtime, database, security) is isolated and replaceable.
- **Extensibility:** New protocols, runtimes, data formats, and security algorithms can be added with minimal changes to existing code.
- **Security by Design:** Encryption is integrated at the protocol boundary, with a focus on OTA request confidentiality and integrity.
- **Production Focus:** Features like graceful shutdown, centralized configuration, and robust logging are built-in.

This document provides a deep dive into the system architecture, component responsibilities, data flow, extensibility points, and the design patterns that make Container Manager a robust foundation for modern container management needs.

## System Architecture

<img src="system_architecture.png" alt="System Architecture" width="1000"/>

## Folder Structure

```
App/
├── api/          # Protocol handlers (REST, MQTT, MQ, D-Bus and future protocols)
├── core/         # Business logic (service layer)
├── database/     # Database interface, embedded and Redis implementations (pluggable)
├── executor/     # Request executors (JSON, Protobuf, handle decryption)
├── runtime/      # Command pattern implementations for Docker CLI, Podman CLI, Docker API, Podman API, etc.
├── utils/        # Common utilities (thread pool, logging, etc.)
├── security/     # Security providers (AES-GCM, ChaCha20, Null)
├── main.cpp      # Application entry point
└── third_party/  # External dependencies (nlohmann_json, httplib)
```

## Design Patterns Used

This project leverages several classic software design patterns to ensure modularity, extensibility, and maintainability:

- **Command Pattern:**  
  Encapsulates each container operation (create, start, stop, etc.) as a command object.  
  _Location:_ `runtime/`, `core/`

- **Factory Pattern:**  
  Used to instantiate the correct command or handler based on runtime/operation.  
  _Location:_ `runtime/` (CommandFactory), `api/`
  Allows pluggable encryption algorithms (AES-GCM, ChaCha20, etc.) via interchangeable security providers during build compilation.
  _Location:_ `security/`

- **Service Pattern:**  
  Encapsulates business logic and coordinates operations between components.  
  _Location:_ `core/` (e.g., `ContainerServiceHandler`)

- **Abstract Factory / Interface Pattern:**  
  Provides contracts for interchangeable components such as database backends and security providers.  
  _Location:_ `database/` (`IDatabaseHandler`), `security/` (`ISecurityProvider`)

- **Observer/Subscriber Pattern:**  
  Protocol handlers (MQTT, Message Queue, D-Bus) listen for and react to incoming messages/events.  
  _Location:_ `api/`

These patterns make the codebase easy to extend (e.g., adding new runtimes, protocols, or security algorithms) and maintain over time.

## Component Descriptions

The following components are organized to match the folder structure of the project:

### 1. API Layer ( api/ )

- **REST API Server:**  
  Handles HTTP requests, parses JSON/Protobuf payloads (optionally encrypted), and forwards them to the executor layer.

- **MQTT Subscriber:**  
  Subscribes to a configured MQTT topic, receives messages (optionally encrypted), and forwards them to the executor.

- **POSIX Message Queue Consumer:**  
  Listens on a POSIX message queue for incoming requests (optionally encrypted).

- **D-Bus Consumer:**  
  Listens for method calls on the D-Bus session bus, enabling desktop and user-level IPC. Supports encrypted and unencrypted payloads, with automatic Base64 decoding for binary data.

- **gRPC Server (planned):**  
  Will provide a strongly-typed, high-performance RPC interface using Protobuf.

> All protocol handlers are modular and can be enabled/disabled at build time via CMake flags.  
> All protocols support secure payload delivery using AES-256-GCM or ChaCha20-Poly1305 encryption for incoming data when enabled.

### 2. Core Layer ( core/ )

- **Container Service Handler:**  
  Central business logic. Receives validated requests from executors, performs runtime checks, and dispatches commands.

- **Service Pattern:**  
  Encapsulates business logic and coordinates operations between components. Each container operation (create, start, stop, etc.) is encapsulated as a command object. Supports multiple runtimes (Docker CLI, Podman CLI, Docker API, Podman API, etc.) and is easily extensible.

### 3. Database Layer ( database/ )

- **IDatabaseHandler:**  
  Abstract interface for database operations (CRUD, state, metadata).

- **EmbeddedDatabaseHandler:**  
  **Default:** Lightweight, fixed-size, in-memory key-value store for embedded and resource-constrained systems.  
  No external dependencies required.

- **RedisDatabaseHandler:**  
  Optional production-ready Redis implementation of the database interface.  
  Requires [cpp_redis](https://github.com/Cylix/cpp_redis) and a running Redis server.

> The database layer is pluggable—swap between embedded and Redis (or any other backend) by implementing the interface.  
> Database selection is controlled via the `ENABLE_REDIS` CMake flag. If not set, the embedded database is used by default.

### 4. Executor Layer ( executor/ )

- **JSON Request Executor:**  
  Validates and parses incoming JSON requests (optionally decrypting them), converting them to internal command objects.

- **Protobuf Request Executor:**  
  Validates and parses incoming Protobuf requests (optionally decrypting them), converting them to internal command objects.

> The executor layer abstracts data format and encryption handling, allowing protocol handlers to remain agnostic of the underlying serialization and security.  
> The executor auto-detects if the payload is encrypted:
>
> - For JSON: If parsing as JSON fails, it attempts decryption and parses again.
> - For Protobuf: If parsing as Protobuf fails, it attempts decryption and parses again.
> - This ensures that all protocols (REST, MQTT, MQ, D-Bus) can send either encrypted or unencrypted payloads without any protocol-specific logic.

### 5. Runtime Layer ( runtime/ )

- **Docker Commands:**  
  Implements container operations for Docker using the Docker CLI.

- **Podman Commands:**  
  Implements container operations for Podman using the Podman CLI.

- **Docker API Commands:**  
  Implements container operations using the Docker REST API over a Unix socket.

- **Podman API Commands:**  
  Implements container operations using the Podman REST API over a Unix socket.

> **Disclaimer:**  
> The `podman-api` runtime is not extensively tested. Use with caution and report any issues you encounter.

> The runtime layer can be extended to support additional container engines and APIs.

### 6. Utilities ( utils/ )

- **Thread Pool:**  
  Efficiently handles concurrent requests.

- **Logging:**  
  Uses Google glog for robust, production-grade logging.

- **Config:**  
  Centralized configuration structs for all protocols and system settings.

- **curl:**  
  Utility for HTTP(S) and Unix socket requests, used for interacting with Docker/Podman APIs and other endpoints.

- **init:**  
  Initialization helpers for setting up application state, configuration, and environment before starting protocol handlers.

### 7. Security & Encryption ( security/ )

- **Security Providers:**  
  Pluggable encryption/decryption providers implementing AES-256-GCM and ChaCha20-Poly1305 algorithms.

- **Key Management:**  
  Keys are stored as hex-encoded files in `storage/security/` (`aes_key.txt`, `chacha20_key.txt`) and loaded at runtime.

- **Decryption Workflow:**  
  When `ENABLE_ENCRYPTION` is ON, all incoming protocol payloads can be decrypted using the selected algorithm.  
  The executor layer auto-detects and decrypts encrypted payloads before parsing, regardless of protocol or data format.

- **Protocol Agnostic:**  
  All protocol handlers (REST, MQTT, MQ, D-Bus) support secure payload delivery and decryption.

- **Configuration:**  
  Encryption can be enabled/disabled at build time (`ENABLE_ENCRYPTION` CMake flag).  
  The algorithm is selected via `SECURITY_ALGORITHM`.

- **Limitation:**  
  Only incoming data is decrypted; outgoing responses are not encrypted.

## Data Flow

1. **Request Reception:**  
   A request arrives via one of the enabled protocols (REST, MQTT, MQ, D-Bus, gRPC (planned)).  
   If encryption is enabled, the payload may be encrypted.

2. **Deserialization & Decryption:**  
   The protocol handler forwards the raw payload to the appropriate executor (JSON or Protobuf).  
   **The executor detects if the payload is encrypted and, if so, decrypts it using the configured algorithm before parsing.**

   - For JSON: If parsing fails, decryption is attempted.
   - For Protobuf: If parsing fails, decryption is attempted.

3. **Validation & Parsing:**  
   The executor validates the request and converts it to an internal command object.

4. **Business Logic:**  
   The service handler checks runtime constraints, permissions, and dispatches the command.

5. **Command Execution:**  
   The command object performs the requested operation (e.g., create/start/stop container) using the appropriate runtime (Docker CLI, Podman CLI, Docker API, Podman API).

6. **Database Interaction:**  
   State and metadata are persisted/retrieved via the database interface.  
   The actual backend (embedded or Redis) is selected at build time.

7. **Response:**  
   The result is sent back via the originating protocol (if applicable).  
   **Note:** Outgoing responses are not encrypted by this application.

## Extensibility

- **Add a Protocol:**  
  Implement a new handler in `api/`, register it in `main.cpp`, and add a CMake flag.

- **Add a Data Format:**  
  Implement a new executor in `executor/`, update protocol handlers to support it.

- **Add a Runtime:**  
  Implement new command classes in `runtime/`, register them in the command factory.

- **Change Database:**  
  Implement `IDatabaseHandler` for your preferred backend.  
  Select the backend at build time using the appropriate CMake flag.

- **Change Security:**  
  Update the encryption key, IV/nonce, or algorithm as needed.  
  Toggle decryption support at build time with `ENABLE_ENCRYPTION`.  
  Add new security algorithms by implementing a new provider in `security/` and updating the executor logic.

## Production-Readiness

- **Graceful Shutdown:**  
  All protocol consumers support `Stop()` and `Join()` for clean shutdown.

- **Signal Handling:**  
  (Recommended) Add signal handlers for SIGINT/SIGTERM to trigger shutdown.

- **Configurable:**  
  All protocol and system settings are centralized and can be externalized.

- **Logging:**  
  All errors and key events are logged with context.

- **Testing:**  
  Unit and integration tests are present; expand coverage as needed.

- **Security:**  
  AES-256-GCM and ChaCha20-Poly1305 decryption is available for all protocols and data formats, providing strong security guarantees for production deployments.  
  **Encryption of outgoing responses is not implemented.**

## Example Sequence Diagram

<img src="architecture_example_sequence_diagram.png" alt="Example Sequence Diagram" width="1000"/>

## Deployment Diagram

<img src="architecture_deployment_diagram.png" alt="Deployment Diagram" width="800"/>

## Recommendations for Contributors

- **Follow the modular architecture:**  
  Add new features as isolated modules with clear interfaces.
- **Document all public APIs and classes.**
- **Write tests for new features and bug fixes.**
- **Update diagrams and documentation as the architecture evolves.**
- **If you add or change encryption/decryption, update this document and the security section.**

## References

- [Doxygen Documentation](../doxygen/html/index.html)
- [README.md](../../README.md)

---

_For questions, suggestions, or contributions, please open an issue or pull request on GitHub._
