# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3

# Include any dependencies generated for this target.
include test/CMakeFiles/test-conversions.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include test/CMakeFiles/test-conversions.dir/compiler_depend.make

# Include the progress variables for this target.
include test/CMakeFiles/test-conversions.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/test-conversions.dir/flags.make

test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o: test/CMakeFiles/test-conversions.dir/flags.make
test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o: test/src/unit-conversions.cpp
test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o: test/CMakeFiles/test-conversions.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o"
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o -MF CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o.d -o CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o -c /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test/src/unit-conversions.cpp

test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.i"
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test/src/unit-conversions.cpp > CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.i

test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.s"
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test/src/unit-conversions.cpp -o CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.s

# Object files for target test-conversions
test__conversions_OBJECTS = \
"CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o"

# External object files for target test-conversions
test__conversions_EXTERNAL_OBJECTS = \
"/home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test/CMakeFiles/doctest_main.dir/src/unit.cpp.o"

test/test-conversions: test/CMakeFiles/test-conversions.dir/src/unit-conversions.cpp.o
test/test-conversions: test/CMakeFiles/doctest_main.dir/src/unit.cpp.o
test/test-conversions: test/CMakeFiles/test-conversions.dir/build.make
test/test-conversions: test/CMakeFiles/test-conversions.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable test-conversions"
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/test-conversions.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/test-conversions.dir/build: test/test-conversions
.PHONY : test/CMakeFiles/test-conversions.dir/build

test/CMakeFiles/test-conversions.dir/clean:
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test && $(CMAKE_COMMAND) -P CMakeFiles/test-conversions.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/test-conversions.dir/clean

test/CMakeFiles/test-conversions.dir/depend:
	cd /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3 /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3 /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test /home/ec2-user/container-manager/mc_orchestrator/json-3.7.3/test/CMakeFiles/test-conversions.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/test-conversions.dir/depend

