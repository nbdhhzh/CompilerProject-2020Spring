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

# Utility rule file for jsoncpp_readerwriter_tests.

# Include the progress variables for this target.
include jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/progress.make

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests: bin/jsontestrunner_exe
jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests: bin/jsoncpp_test
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && /usr/bin/python3 -B /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner/../../test/runjsontests.py /home/hzh/proj2_new/build/bin/jsontestrunner_exe /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner/../../test/data

jsoncpp_readerwriter_tests: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests
jsoncpp_readerwriter_tests: jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/build.make

.PHONY : jsoncpp_readerwriter_tests

# Rule to build all files generated by this target.
jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/build: jsoncpp_readerwriter_tests

.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/build

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/clean:
	cd /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner && $(CMAKE_COMMAND) -P CMakeFiles/jsoncpp_readerwriter_tests.dir/cmake_clean.cmake
.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/clean

jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/depend:
	cd /home/hzh/proj2_new/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hzh/proj2_new /home/hzh/proj2_new/3rdparty/jsoncpp/src/jsontestrunner /home/hzh/proj2_new/build /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner /home/hzh/proj2_new/build/jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : jsoncpp-build/src/jsontestrunner/CMakeFiles/jsoncpp_readerwriter_tests.dir/depend

