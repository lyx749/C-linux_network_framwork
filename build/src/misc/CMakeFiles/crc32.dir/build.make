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
include src/misc/CMakeFiles/crc32.dir/depend.make

# Include the progress variables for this target.
include src/misc/CMakeFiles/crc32.dir/progress.make

# Include the compile flags for this target's objects.
include src/misc/CMakeFiles/crc32.dir/flags.make

src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.o: src/misc/CMakeFiles/crc32.dir/flags.make
src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.o: ../src/misc/http_c_crc32.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.o"
	cd /home/lyx/httpServer/build/src/misc && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/crc32.dir/http_c_crc32.cxx.o -c /home/lyx/httpServer/src/misc/http_c_crc32.cxx

src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/crc32.dir/http_c_crc32.cxx.i"
	cd /home/lyx/httpServer/build/src/misc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/misc/http_c_crc32.cxx > CMakeFiles/crc32.dir/http_c_crc32.cxx.i

src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/crc32.dir/http_c_crc32.cxx.s"
	cd /home/lyx/httpServer/build/src/misc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/misc/http_c_crc32.cxx -o CMakeFiles/crc32.dir/http_c_crc32.cxx.s

# Object files for target crc32
crc32_OBJECTS = \
"CMakeFiles/crc32.dir/http_c_crc32.cxx.o"

# External object files for target crc32
crc32_EXTERNAL_OBJECTS =

../_staticLib/libcrc32.a: src/misc/CMakeFiles/crc32.dir/http_c_crc32.cxx.o
../_staticLib/libcrc32.a: src/misc/CMakeFiles/crc32.dir/build.make
../_staticLib/libcrc32.a: src/misc/CMakeFiles/crc32.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library ../../../_staticLib/libcrc32.a"
	cd /home/lyx/httpServer/build/src/misc && $(CMAKE_COMMAND) -P CMakeFiles/crc32.dir/cmake_clean_target.cmake
	cd /home/lyx/httpServer/build/src/misc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/crc32.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/misc/CMakeFiles/crc32.dir/build: ../_staticLib/libcrc32.a

.PHONY : src/misc/CMakeFiles/crc32.dir/build

src/misc/CMakeFiles/crc32.dir/clean:
	cd /home/lyx/httpServer/build/src/misc && $(CMAKE_COMMAND) -P CMakeFiles/crc32.dir/cmake_clean.cmake
.PHONY : src/misc/CMakeFiles/crc32.dir/clean

src/misc/CMakeFiles/crc32.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer/src/misc /home/lyx/httpServer/build /home/lyx/httpServer/build/src/misc /home/lyx/httpServer/build/src/misc/CMakeFiles/crc32.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/misc/CMakeFiles/crc32.dir/depend

