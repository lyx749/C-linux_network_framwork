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
CMAKE_SOURCE_DIR = /home/lyx/httpServer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lyx/httpServer/build

# Include any dependencies generated for this target.
include src/logs/CMakeFiles/log.dir/depend.make

# Include the progress variables for this target.
include src/logs/CMakeFiles/log.dir/progress.make

# Include the compile flags for this target's objects.
include src/logs/CMakeFiles/log.dir/flags.make

src/logs/CMakeFiles/log.dir/http_log.cxx.o: src/logs/CMakeFiles/log.dir/flags.make
src/logs/CMakeFiles/log.dir/http_log.cxx.o: ../src/logs/http_log.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/logs/CMakeFiles/log.dir/http_log.cxx.o"
	cd /home/lyx/httpServer/build/src/logs && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/log.dir/http_log.cxx.o -c /home/lyx/httpServer/src/logs/http_log.cxx

src/logs/CMakeFiles/log.dir/http_log.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/log.dir/http_log.cxx.i"
	cd /home/lyx/httpServer/build/src/logs && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/http_log.cxx > CMakeFiles/log.dir/http_log.cxx.i

src/logs/CMakeFiles/log.dir/http_log.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/log.dir/http_log.cxx.s"
	cd /home/lyx/httpServer/build/src/logs && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/http_log.cxx -o CMakeFiles/log.dir/http_log.cxx.s

# Object files for target log
log_OBJECTS = \
"CMakeFiles/log.dir/http_log.cxx.o"

# External object files for target log
log_EXTERNAL_OBJECTS =

../_staticLib/liblog.a: src/logs/CMakeFiles/log.dir/http_log.cxx.o
../_staticLib/liblog.a: src/logs/CMakeFiles/log.dir/build.make
../_staticLib/liblog.a: src/logs/CMakeFiles/log.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../_staticLib/liblog.a"
	cd /home/lyx/httpServer/build/src/logs && $(CMAKE_COMMAND) -P CMakeFiles/log.dir/cmake_clean_target.cmake
	cd /home/lyx/httpServer/build/src/logs && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/log.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/logs/CMakeFiles/log.dir/build: ../_staticLib/liblog.a

.PHONY : src/logs/CMakeFiles/log.dir/build

src/logs/CMakeFiles/log.dir/clean:
	cd /home/lyx/httpServer/build/src/logs && $(CMAKE_COMMAND) -P CMakeFiles/log.dir/cmake_clean.cmake
.PHONY : src/logs/CMakeFiles/log.dir/clean

src/logs/CMakeFiles/log.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer/src/logs /home/lyx/httpServer/build /home/lyx/httpServer/build/src/logs /home/lyx/httpServer/build/src/logs/CMakeFiles/log.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/logs/CMakeFiles/log.dir/depend

