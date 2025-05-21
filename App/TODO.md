## **App Refactoring & Improvement To-Do List**

### **1. Dependency Injection**

- Remove all singleton usage for `RedisDatabaseHandler`.
- Pass `IDatabaseHandler&` (or smart pointer) to `ContainerServiceHandler` and other consumers.
- Ensure all service/database usages are via injected instances, not static/global access.

### **2. Remove Code Duplication**

- Identify and refactor duplicated logic in Docker/Podman command classes (start, stop, remove, restart, etc.).
- Consider a base class or utility function for common command execution logic.
- Parameterize runtime where possible to reduce class count.

### **3. Use Enums for Operation/Runtime**

- Replace string-based `operation` and `runtime` in `ContainerRequest` with enums.
- Update all code to use enums for type safety and clarity.

### **4. Error Handling & Logging**

- Standardize error handling and logging across all commands and service layers.
- Consider a common error/status enum or structure for responses.

### **5. Consistent Naming & Documentation**

- Ensure all classes, methods, and files have clear, consistent naming.
- Add or update Doxygen-style comments for all public interfaces and classes.

### **6. Unit Testing**

- Write or update unit tests for `ContainerServiceHandler` and `CommandFactory`.
- Use mock implementations of `IDatabaseHandler` for testing.

### **7. Configuration Management**

- If many runtime/config parameters are used, consider a configuration struct or object.
- Centralize configuration loading and access.

### **8. Protocol Executors**

- Ensure all executors (JSON, Protobuf, etc.) use the new DI-based service handler.
- Remove any static or singleton service/database usage in executors.

### **9. Main Application Wiring**

- In `main.cpp`, create and inject dependencies (DB handler, service handler) as needed.
- Avoid global or static state.

### **10. Thread Management in main.cpp**

- Refactor the dummy thread that keeps `main` alive.
- Implement proper lifecycle management for protocol threads and graceful shutdown.
- Remove or replace the infinite sleep thread with a robust solution.

## **Optional/Nice-to-Have**

- Consider using smart pointers (`std::unique_ptr`/`std::shared_ptr`) for dependency management if lifetime/ownership is complex.
- Explore using a dependency injection framework if the project grows larger.
