/*
@licstart  The following is the entire license notice for the
JavaScript code in this file.

Copyright (C) 1997-2019 by Dimitri van Heesch

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as published by
the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

@licend  The above is the entire license notice
for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "Container Manager", "index.html", [
    [ "Use Cases for this Template", "index.html#autotoc_md1", null ],
    [ "Features", "index.html#autotoc_md2", null ],
    [ "Architecture Documentation", "index.html#autotoc_md3", null ],
    [ "Security Architecture", "index.html#autotoc_md4", null ],
    [ "Database Backend Selection", "index.html#autotoc_md5", [
      [ "How to Select the Database Backend", "index.html#autotoc_md6", null ],
      [ "Example: Enabling Redis Database", "index.html#autotoc_md7", null ]
    ] ],
    [ "Architecture Overview", "index.html#autotoc_md8", null ],
    [ "Modularity & Build System", "index.html#autotoc_md9", [
      [ "Protocols and Data Formats", "index.html#autotoc_md10", null ],
      [ "Supported Runtimes", "index.html#autotoc_md11", null ],
      [ "CMake Feature Flags", "index.html#autotoc_md12", null ]
    ] ],
    [ "Build Instructions", "index.html#autotoc_md13", [
      [ "1. Clone and Prepare", "index.html#autotoc_md14", null ],
      [ "2. Install Only What You Need", "index.html#autotoc_md15", [
        [ "If you want only JSON over D-Bus (embedded database):", "index.html#autotoc_md16", null ],
        [ "If you want only Protobuf over Message Queue (embedded database):", "index.html#autotoc_md17", null ],
        [ "If you want REST and MQTT with JSON only (embedded database):", "index.html#autotoc_md18", null ],
        [ "If you want all protocols with JSON format (embedded database):", "index.html#autotoc_md19", null ],
        [ "If you want Redis as the database backend:", "index.html#autotoc_md20", null ]
      ] ],
      [ "3. Summary Table", "index.html#autotoc_md21", null ],
      [ "4. General Build Steps", "index.html#autotoc_md22", null ],
      [ "5. Examples", "index.html#autotoc_md23", null ]
    ] ],
    [ "Running the Server", "index.html#autotoc_md24", null ],
    [ "API Usage", "index.html#autotoc_md25", [
      [ "Example JSON Request", "index.html#autotoc_md26", null ],
      [ "Example Protobuf Request", "index.html#autotoc_md27", null ],
      [ "HTTP Endpoint", "index.html#autotoc_md28", null ],
      [ "MQTT Usage", "index.html#autotoc_md29", null ],
      [ "POSIX Message Queue Usage", "index.html#autotoc_md30", null ],
      [ "D-Bus (Session Bus) Usage", "index.html#autotoc_md31", null ]
    ] ],
    [ "Code Structure", "index.html#autotoc_md32", null ],
    [ "Python UI: Container Creator", "index.html#autotoc_md33", [
      [ "Features", "index.html#autotoc_md34", null ],
      [ "Requirements", "index.html#autotoc_md35", null ],
      [ "Usage", "index.html#autotoc_md36", null ]
    ] ],
    [ "Documentation", "index.html#autotoc_md37", null ],
    [ "Extending the Project", "index.html#autotoc_md38", null ],
    [ "Contributing", "index.html#autotoc_md39", null ],
    [ "License", "index.html#autotoc_md40", null ],
    [ "Acknowledgements", "index.html#autotoc_md41", null ],
    [ "Container Manager Architecture", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html", [
      [ "Overview", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md43", [
        [ "System Architecture", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md44", null ]
      ] ],
      [ "Folder Structure", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md45", null ],
      [ "Component Descriptions", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md46", [
        [ "1. API Layer (Protocol Handlers)", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md47", null ],
        [ "2. Executor Layer (Request Executors)", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md48", null ],
        [ "3. Core Layer (Service & Command Pattern)", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md49", null ],
        [ "4. Runtime Layer", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md50", null ],
        [ "5. Database Layer", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md51", null ],
        [ "6. Utilities", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md52", null ]
      ] ],
      [ "Security & Encryption", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md53", null ],
      [ "Data Flow", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md54", null ],
      [ "Extensibility", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md55", null ],
      [ "Production-Readiness", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md56", null ],
      [ "Example Sequence Diagram", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md57", null ],
      [ "Deployment Diagram", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md58", null ],
      [ "Recommendations for Contributors", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md59", null ],
      [ "References", "md__home_komal_workspace_personal_container_manager_docs_architecture_architecture.html#autotoc_md60", null ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Variables", "namespacemembers_vars.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", "functions_dup" ],
        [ "Functions", "functions_func.html", "functions_func" ],
        [ "Variables", "functions_vars.html", null ],
        [ "Related Functions", "functions_rela.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ],
      [ "File Members", "globals.html", [
        [ "All", "globals.html", null ],
        [ "Functions", "globals_func.html", null ],
        [ "Variables", "globals_vars.html", null ],
        [ "Typedefs", "globals_type.html", null ],
        [ "Enumerations", "globals_enum.html", null ],
        [ "Macros", "globals_defs.html", null ]
      ] ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"CMakeCXXCompilerId_8cpp.html",
"classcontainermanager_1_1ContainerParameters.html#a8314afa07bd3eb2c5bfa980dbea06fbf",
"functions_i.html"
];

var SYNCONMSG = 'click to disable panel synchronisation';
var SYNCOFFMSG = 'click to enable panel synchronisation';