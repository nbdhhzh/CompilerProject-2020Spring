# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

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
CMAKE_SOURCE_DIR = /home/hzh/proj2_new

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hzh/proj2_new/build

# Utility rule file for clean_run.

# Include the progress variables for this target.
include project1/CMakeFiles/clean_run.dir/progress.make

project1/CMakeFiles/clean_run:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hzh/proj2_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "run generated clean in /home/hzh/proj2_new/project1"
	cd /home/hzh/proj2_new/project1 && /home/hzh/proj2_new/build/project1/cleanf

clean_run: project1/CMakeFiles/clean_run
clean_run: project1/CMakeFiles/clean_run.dir/build.make

.PHONY : clean_run

# Rule to build all files generated by this target.
project1/CMakeFiles/clean_run.dir/build: clean_run

.PHONY : project1/CMakeFiles/clean_run.dir/build

project1/CMakeFiles/clean_run.dir/clean:
	cd /home/hzh/proj2_new/build/project1 && $(CMAKE_COMMAND) -P CMakeFiles/clean_run.dir/cmake_clean.cmake
.PHONY : project1/CMakeFiles/clean_run.dir/clean

project1/CMakeFiles/clean_run.dir/depend:
	cd /home/hzh/proj2_new/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hzh/proj2_new /home/hzh/proj2_new/project1 /home/hzh/proj2_new/build /home/hzh/proj2_new/build/project1 /home/hzh/proj2_new/build/project1/CMakeFiles/clean_run.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : project1/CMakeFiles/clean_run.dir/depend

