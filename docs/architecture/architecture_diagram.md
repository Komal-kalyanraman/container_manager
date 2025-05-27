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

    %% API to Executor
    REST -- JSON --> JSON
    REST -- Proto --> PROTO
    MQTT -- JSON --> JSON
    MQTT -- Proto --> PROTO
    MQ -- JSON --> JSON
    MQ -- Proto --> PROTO
    DBUS -- JSON --> JSON
    DBUS -- Proto --> PROTO

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

subgraph ExecutorLayer
JSON[JSON Request Executor]
PROTO[Protobuf Request Executor]
end

REST -- JSON --> JSON
REST -- Proto --> PROTO
MQTT -- JSON --> JSON
MQTT -- Proto --> PROTO
MQ -- JSON --> JSON
MQ -- Proto --> PROTO
DBUS -- JSON --> JSON
DBUS -- Proto --> PROTO
GRPC -- Proto --> PROTO
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
    participant JSONExec
    participant Service
    participant DockerCmd
    participant DBIF
    participant Redis
    participant Embedded

    Client->>REST: POST /execute (JSON)
    REST->>JSONExec: Forward payload
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
    CM[Container Manager Service]
    Redis[(Redis DB)]
    Embedded[(Embedded DB)]
    Docker[(Docker Daemon)]
    Podman[(Podman Daemon)]
    MQTTB[(MQTT Broker)]
    Client1[REST Client]
    Client2[MQTT Client]
    Client3[DBus Client]
    Client4[POSIX MQ Client]
    DockerAPI[Docker API]
    DockerCLI[Docker CLI]
    PodmanAPI[Podman API]
    PodmanCLI[Podman CLI]

    Client1-->|HTTP|CM
    Client2-->|MQTT|MQTTB-->|MQTT|CM
    Client3-->|D-Bus|CM
    Client4-->|POSIX MQ|CM

    CM-->|Redis API|Redis
    CM-->|Embedded API|Embedded

    %% Docker connections
    CM-->|Docker API|DockerAPI
    CM-->|Docker CLI|DockerCLI
    DockerAPI-->|Unix Socket/API|Docker
    DockerCLI-->|CLI|Docker

    %% Podman connections
    CM-->|Podman API|PodmanAPI
    CM-->|Podman CLI|PodmanCLI
    PodmanAPI-->|Unix Socket/API|Podman
    PodmanCLI-->|CLI|Podman
```
