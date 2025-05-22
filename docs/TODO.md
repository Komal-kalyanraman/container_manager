## **App Refactoring & Improvement To-Do List**

### **1. Dependency Injection**

- Remove all singleton usage for `RedisDatabaseHandler`.
- Pass `IDatabaseHandler&` (or smart pointer) to `ContainerServiceHandler` and other consumers.
- Ensure all service/database usages are via injected instances, not static/global access.

### **2. Error Handling & Logging**

- Standardize error handling and logging across all commands and service layers.
- Consider a common error/status enum or structure for responses.

### **3. Unit Testing**

- Write or update unit tests for `ContainerServiceHandler` and `CommandFactory`.
- Use mock implementations of `IDatabaseHandler` for testing.

## **Optional/Nice-to-Have**

- Consider using smart pointers (`std::unique_ptr`/`std::shared_ptr`) for dependency management if lifetime/ownership is complex.
- Explore using a dependency injection framework if the project grows larger.
