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
end

DOCKER -- DB Access --> DBIF
PODMAN -- DB Access --> DBIF
DBIF -- Redis API --> REDIS
```

## Example Sequence Diagram

```mermaid
sequenceDiagram
    participant Client
    participant REST
    participant JSONExec
    participant Service
    participant DockerCmd
    participant Redis

    Client->>REST: POST /execute (JSON)
    REST->>JSONExec: Forward payload
    JSONExec->>Service: Validated request
    Service->>DockerCmd: Dispatch create command
    DockerCmd->>Redis: Store metadata
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
    Docker[(Docker Daemon)]
    Podman[(Podman Daemon)]
    MQTTB[(MQTT Broker)]
    Client1[REST Client]
    Client2[MQTT Client]
    Client3[DBus Client]
    Client4[POSIX MQ Client]

    Client1-->|HTTP|CM
    Client2-->|MQTT|MQTTB-->|MQTT|CM
    Client3-->|D-Bus|CM
    Client4-->|POSIX MQ|CM
    CM-->|Redis API|Redis
    CM-->|Docker API|Docker
    CM-->|Podman API|Podman
```
