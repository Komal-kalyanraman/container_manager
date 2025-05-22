# Container Manager Project: Naming and Commenting Standards

## 1. Naming Conventions

- **Classes/Structs:** PascalCase (e.g., `ContainerServiceHandler`, `ServerConfig`)
- **Methods/Functions:** PascalCase (e.g., `HandleRequest`, `InitLogging`)
- **Files:** lower_snake_case (e.g., `container_service.hpp`, `init_handler.cpp`)
- **Namespaces:** PascalCase (e.g., `CommandTemplate`)
- **Constants:** k-prefixed PascalCase for global constants (e.g., `kEmptyString`)
- **Macros:** ALL_CAPS_WITH_UNDERSCORES (e.g., `ENABLE_GLOG_LOGGING`)

### 1.1. Variable Naming by Scope

- **Public Member Variables:**

  - Use lower_snake_case (e.g., `max_connections`).
  - However, prefer making member variables private/protected and exposing access via getters/setters.

- **Private/Protected Member Variables:**

  - Use lower*snake_case with a trailing underscore (e.g., `connection_pool*`).

- **Local Variables and Function Parameters:**

  - Use lower_snake_case (e.g., `container_name`, `image_name`, `db`).

- **Global Constants:**

  - Use `k`-prefixed PascalCase (e.g., `kEmptyString`).

- **Macros:**
  - Use ALL_CAPS_WITH_UNDERSCORES (e.g., `ENABLE_GLOG_LOGGING`).

> **Note:** In this project, most member variables are private by default and follow the trailing underscore convention.

## 2. Commenting Standards

- **File Header:**  
  Every file starts with a Doxygen-style header:
  ```
  /**
   * @file filename.hpp
   * @brief Short description of the file.
   */
  ```
- **Class/Struct Comments:**  
  Doxygen-style, before the class/struct definition:
  ```
  /**
   * @class ClassName
   * @brief Short description.
   * @details (Optional) More details.
   */
  ```
- **Method/Function Comments:**  
  Doxygen-style, before the method/function:
  ```
  /**
   * @brief Short description.
   * @param param_name Description of parameter.
   * @return Description of return value.
   */
  ```
- **Member Variable Comments:**  
  Doxygen-style, after the member variable:
  ```
  int value_; ///< Description of the member variable.
  ```
- **Inline Comments:**  
  Used sparingly for complex logic or important notes.

## 3. General Guidelines

- All public interfaces and classes must have Doxygen-style comments.
- Use clear, descriptive names for all identifiers.
- Consistency is prioritized across the codebase.
- Avoid abbreviations unless they are well-known (e.g., `cfg` for config).
- Prefer explicitness over brevity for clarity.
