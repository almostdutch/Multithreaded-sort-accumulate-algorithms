# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin

# Include any dependencies generated for this target.
include CMakeFiles/accumulate_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/accumulate_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/accumulate_test.dir/flags.make

CMakeFiles/accumulate_test.dir/src/main.cpp.o: CMakeFiles/accumulate_test.dir/flags.make
CMakeFiles/accumulate_test.dir/src/main.cpp.o: ../src/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/accumulate_test.dir/src/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/accumulate_test.dir/src/main.cpp.o -c /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/src/main.cpp

CMakeFiles/accumulate_test.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/accumulate_test.dir/src/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/src/main.cpp > CMakeFiles/accumulate_test.dir/src/main.cpp.i

CMakeFiles/accumulate_test.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/accumulate_test.dir/src/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/src/main.cpp -o CMakeFiles/accumulate_test.dir/src/main.cpp.s

# Object files for target accumulate_test
accumulate_test_OBJECTS = \
"CMakeFiles/accumulate_test.dir/src/main.cpp.o"

# External object files for target accumulate_test
accumulate_test_EXTERNAL_OBJECTS =

accumulate_test: CMakeFiles/accumulate_test.dir/src/main.cpp.o
accumulate_test: CMakeFiles/accumulate_test.dir/build.make
accumulate_test: CMakeFiles/accumulate_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable accumulate_test"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/accumulate_test.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/accumulate_test.dir/build: accumulate_test

.PHONY : CMakeFiles/accumulate_test.dir/build

CMakeFiles/accumulate_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/accumulate_test.dir/cmake_clean.cmake
.PHONY : CMakeFiles/accumulate_test.dir/clean

CMakeFiles/accumulate_test.dir/depend:
	cd /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin /home/vitaliy/installed_programs/eclipse/parallel-2021-06/eclipse-workspace/test2/accumulate_alg/bin/CMakeFiles/accumulate_test.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/accumulate_test.dir/depend

