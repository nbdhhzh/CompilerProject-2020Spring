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

# Include any dependencies generated for this target.
include jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/depend.make

# Include the progress variables for this target.
include jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/progress.make

# Include the compile flags for this target's objects.
include jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/flags.make

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/flags.make
jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o: ../3rdparty/jsoncpp/src/jsontestrunner/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hzh/proj2_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o"
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/jsontestrunner_exe.dir/main.cpp.o -c /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner/main.cpp

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/jsontestrunner_exe.dir/main.cpp.i"
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner/main.cpp > CMakeFiles/jsontestrunner_exe.dir/main.cpp.i

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/jsontestrunner_exe.dir/main.cpp.s"
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner/main.cpp -o CMakeFiles/jsontestrunner_exe.dir/main.cpp.s

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.requires:

.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.requires

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.provides: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.requires
	$(MAKE) -f jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/build.make jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.provides.build
.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.provides

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.provides.build: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o


# Object files for target jsontestrunner_exe
jsontestrunner_exe_OBJECTS = \
"CMakeFiles/jsontestrunner_exe.dir/main.cpp.o"

# External object files for target jsontestrunner_exe
jsontestrunner_exe_EXTERNAL_OBJECTS =

bin/jsontestrunner_exe: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o
bin/jsontestrunner_exe: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/build.make
bin/jsontestrunner_exe: lib/libjsoncpp.a
bin/jsontestrunner_exe: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hzh/proj2_new/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../../../bin/jsontestrunner_exe"
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/jsontestrunner_exe.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/build: bin/jsontestrunner_exe

.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/build

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/requires: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/main.cpp.o.requires

.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/requires

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/clean:
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && $(CMAKE_COMMAND) -P CMakeFiles/jsontestrunner_exe.dir/cmake_clean.cmake
.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/clean

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/depend:
	cd /home/hzh/proj2_new/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hzh/proj2_new /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner /home/hzh/proj2_new/build /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsontestrunner_exe.dir/depend

