### Complete Architecture diagram

```mermaid
flowchart TD
    %% Clients
    ClientREST[REST Client]
    ClientMQTT[MQTT Client]
    ClientDBus[D-Bus Client]
    ClientMQ[POSIX MQ Client]

    %% API Layer
    REST[REST API Server]
    MQTT[MQTT Subscriber]
    MQ[POSIX Message Queue Consumer]
    DBUS[D-Bus Consumer]

    %% Security Layer
    ENCRYPT[🔒 AES-256-GCM<br/>Encryption/Decryption]
    B64[📝 Base64 Encoding/Decoding]

    %% Executor Layer
    JSON[JSON Request Executor]
    PROTO[Protobuf Request Executor]

    %% Core Layer
    SERVICE[Container Service Handler]
    COMMANDS[Command Pattern]

    %% Runtime Layer
    DOCKER[Docker Commands]
    PODMAN[Podman Commands]
    DOCKERAPI[Docker API Commands]
    PODMANAPI[Podman API Commands]

    %% Database Layer
    DBIF[IDatabaseHandler]
    REDIS[RedisDatabaseHandler]
    EMBEDDED[EmbeddedDatabaseHandler]

    %% Infrastructure
    MQTTB[MQTT Broker]
    DockerD[(Docker Daemon)]
    PodmanD[(Podman Daemon)]

    %% Client to API
    ClientREST -- HTTP --> REST
    ClientMQTT -- MQTT --> MQTTB
    MQTTB -- MQTT --> MQTT
    ClientDBus -- D-Bus --> DBUS
    ClientMQ -- POSIX MQ --> MQ

    %% API to Security
    REST -- "Encrypted/Binary Payload" --> ENCRYPT
    MQTT -- "Encrypted/Binary Payload" --> ENCRYPT
    MQ -- "Encrypted/Binary Payload" --> ENCRYPT
    DBUS -- "Base64/Encrypted Payload" --> B64
    B64 -- "Decoded Payload" --> ENCRYPT

    %% Security to Executor
    ENCRYPT -- "Decrypted Data" --> JSON
    ENCRYPT -- "Decrypted Data" --> PROTO
    REST -- "Plain JSON" --> JSON
    REST -- "Plain Proto" --> PROTO
    MQTT -- "Plain JSON" --> JSON
    MQTT -- "Plain Proto" --> PROTO
    MQ -- "Plain JSON" --> JSON
    MQ -- "Plain Proto" --> PROTO
    DBUS -- "Plain JSON" --> JSON
    DBUS -- "Plain Proto" --> PROTO

    %% Executor to Core
    JSON -- Validated Request --> SERVICE
    PROTO -- Validated Request --> SERVICE

    %% Core to Command
    SERVICE -- Command Dispatch --> COMMANDS

    %% Command to Runtime
    COMMANDS -- Docker Ops --> DOCKER
    COMMANDS -- Podman Ops --> PODMAN
    COMMANDS -- Docker API Ops --> DOCKERAPI
    COMMANDS -- Podman API Ops --> PODMANAPI

    %% Runtime to Database
    DOCKER -- DB Access --> DBIF
    PODMAN -- DB Access --> DBIF
    DOCKERAPI -- DB Access --> DBIF
    PODMANAPI -- DB Access --> DBIF

    %% Database Implementations
    DBIF -- Redis API --> REDIS
    DBIF -- Embedded API --> EMBEDDED

    %% Runtime to Daemon
    DOCKER -- Docker CLI --> DockerD
    PODMAN -- Podman CLI --> PodmanD
    DOCKERAPI -- Docker REST API --> DockerD
    PODMANAPI -- Podman REST API --> PodmanD
```

### API & Executor Layers

```mermaid
flowchart TD
subgraph APILayer
REST[REST API Server]
MQTT[MQTT Subscriber]
MQ[POSIX Message Queue Consumer]
DBUS[D-Bus Consumer]
GRPC[GRPC Server planned]
end

subgraph SecurityLayer
ENCRYPT[🔒 AES-256-GCM Encryption/Decryption]
B64[📝 Base64 Encoding/Decoding]
end

subgraph ExecutorLayer
JSON[JSON Request Executor]
PROTO[Protobuf Request Executor]
end

REST -- "Encrypted/Binary" --> ENCRYPT
MQTT -- "Encrypted/Binary" --> ENCRYPT
MQ -- "Encrypted/Binary" --> ENCRYPT
DBUS -- "Base64/Encrypted" --> B64
B64 -- "Decoded" --> ENCRYPT

ENCRYPT -- "Decrypted JSON" --> JSON
ENCRYPT -- "Decrypted Proto" --> PROTO

REST -- "Plain JSON" --> JSON
REST -- "Plain Proto" --> PROTO
MQTT -- "Plain JSON" --> JSON
MQTT -- "Plain Proto" --> PROTO
MQ -- "Plain JSON" --> JSON
MQ -- "Plain Proto" --> PROTO
DBUS -- "Plain JSON" --> JSON
DBUS -- "Plain Proto" --> PROTO
GRPC -- "Proto" --> PROTO
```

### Executor, Core, and Runtime Layers

```mermaid
flowchart TD
subgraph ExecutorLayer
JSON[JSON Request Executor]
PROTO[Protobuf Request Executor]
end

subgraph CoreLayer
SERVICE[Container Service Handler]
COMMANDS[Command Pattern]
end

subgraph RuntimeLayer
DOCKER[Docker Commands]
PODMAN[Podman Commands]
end

JSON -- Validated Request --> SERVICE
PROTO -- Validated Request --> SERVICE
SERVICE -- Command Dispatch --> COMMANDS
COMMANDS -- Docker Ops --> DOCKER
COMMANDS -- Podman Ops --> PODMAN
```

### Runtime & Database Layers

```mermaid
flowchart TD
subgraph RuntimeLayer
DOCKER[Docker Commands]
PODMAN[Podman Commands]
end

subgraph DatabaseLayer
DBIF[IDatabaseHandler]
REDIS[RedisDatabaseHandler]
EMBEDDED[EmbeddedDatabaseHandler]
end

DOCKER -- DB Access --> DBIF
PODMAN -- DB Access --> DBIF
DBIF -- Redis API --> REDIS
DBIF -- Embedded API --> EMBEDDED
```

## Example Sequence Diagram

```mermaid
sequenceDiagram
    participant Client
    participant REST
    participant ENCRYPT as 🔒 AES-GCM
    participant JSONExec
    participant Service
    participant DockerCmd
    participant DBIF
    participant Redis
    participant Embedded

    Client->>REST: POST /execute (Encrypted JSON)
    REST->>ENCRYPT: Decrypt payload (AES-256-GCM)
    ENCRYPT->>JSONExec: Decrypted JSON
    JSONExec->>Service: Validated request
    Service->>DockerCmd: Dispatch create command
    DockerCmd->>DBIF: Store metadata
    alt Redis backend enabled
        DBIF->>Redis: Store data
        Redis-->>DBIF: Ack
    else Embedded backend (default)
        DBIF->>Embedded: Store data
        Embedded-->>DBIF: Ack
    end
    DockerCmd-->>Service: Result
    Service-->>JSONExec: Result
    JSONExec-->>REST: Response
    REST-->>Client: HTTP 200 OK
```

## Deployment Diagram

```mermaid
graph TD
    %% Clients and Brokers
    Client1[REST Client]
    Client2[MQTT Client]
    Client3[DBus Client]
    Client4[POSIX MQ Client]
    MQTTB[(MQTT Broker)]

    %% Security/Decoding Modules
    B64[📝 Base64 Decoding]
    ENCRYPT[🔒 AES-256-GCM Decryption]

    %% Main Service
    CM[Container Manager Service]

    %% Databases
    Redis[(Redis DB)]
    Embedded[(Embedded DB)]

    %% Docker/Podman Daemons and APIs
    Docker[(Docker Daemon)]
    Podman[(Podman Daemon)]
    DockerAPI[Docker API]
    DockerCLI[Docker CLI]
    PodmanAPI[Podman API]
    PodmanCLI[Podman CLI]

    %% Client to Service (with security modules)
    Client1 -->|HTTP| ENCRYPT
    Client2 -->|MQTT| MQTTB
    MQTTB -->|MQTT| ENCRYPT
    Client3 -->|D-Bus| B64
    B64 -->|Decoded| ENCRYPT
    Client4 -->|POSIX MQ| ENCRYPT

    %% Decryption to Main Service
    ENCRYPT -->|Decrypted Payload| CM

    %% Database connections
    CM -->|Redis API| Redis
    CM -->|Embedded API| Embedded

    %% Docker connections
    CM -->|Docker API| DockerAPI
    CM -->|Docker CLI| DockerCLI
    DockerAPI -->|Unix Socket/API| Docker
    DockerCLI -->|CLI| Docker

    %% Podman connections
    CM -->|Podman API| PodmanAPI
    CM -->|Podman CLI| PodmanCLI
    PodmanAPI -->|Unix Socket/API| Podman
    PodmanCLI -->|CLI| Podman
```
