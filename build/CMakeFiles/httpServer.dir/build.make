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
include CMakeFiles/httpServer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/httpServer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/httpServer.dir/flags.make

CMakeFiles/httpServer.dir/main.cpp.o: CMakeFiles/httpServer.dir/flags.make
CMakeFiles/httpServer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/httpServer.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/httpServer.dir/main.cpp.o -c /home/lyx/httpServer/main.cpp

CMakeFiles/httpServer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/httpServer.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/main.cpp > CMakeFiles/httpServer.dir/main.cpp.i

CMakeFiles/httpServer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/httpServer.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/main.cpp -o CMakeFiles/httpServer.dir/main.cpp.s

# Object files for target httpServer
httpServer_OBJECTS = \
"CMakeFiles/httpServer.dir/main.cpp.o"

# External object files for target httpServer
httpServer_EXTERNAL_OBJECTS =

../_bin/httpServer: CMakeFiles/httpServer.dir/main.cpp.o
../_bin/httpServer: CMakeFiles/httpServer.dir/build.make
../_bin/httpServer: ../_staticLib/libnet.a
../_bin/httpServer: ../_staticLib/libthreadPool.a
../_bin/httpServer: ../_staticLib/liblogic.a
../_bin/httpServer: ../_staticLib/liblog.a
../_bin/httpServer: ../_staticLib/libcrc32.a
../_bin/httpServer: ../_staticLib/libnet.a
../_bin/httpServer: ../_staticLib/libmemory.a
../_bin/httpServer: ../_staticLib/liblog.a
../_bin/httpServer: ../_staticLib/libspdlogd.a
../_bin/httpServer: ../_staticLib/libhttpParser.a
../_bin/httpServer: CMakeFiles/httpServer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../_bin/httpServer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/httpServer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/httpServer.dir/build: ../_bin/httpServer

.PHONY : CMakeFiles/httpServer.dir/build

CMakeFiles/httpServer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/httpServer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/httpServer.dir/clean

CMakeFiles/httpServer.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer /home/lyx/httpServer/build /home/lyx/httpServer/build /home/lyx/httpServer/build/CMakeFiles/httpServer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/httpServer.dir/depend

