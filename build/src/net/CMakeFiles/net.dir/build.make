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
include src/net/CMakeFiles/net.dir/depend.make

# Include the progress variables for this target.
include src/net/CMakeFiles/net.dir/progress.make

# Include the compile flags for this target's objects.
include src/net/CMakeFiles/net.dir/flags.make

src/net/CMakeFiles/net.dir/http_c_socket.cxx.o: src/net/CMakeFiles/net.dir/flags.make
src/net/CMakeFiles/net.dir/http_c_socket.cxx.o: ../src/net/http_c_socket.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/net/CMakeFiles/net.dir/http_c_socket.cxx.o"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/http_c_socket.cxx.o -c /home/lyx/httpServer/src/net/http_c_socket.cxx

src/net/CMakeFiles/net.dir/http_c_socket.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/http_c_socket.cxx.i"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/net/http_c_socket.cxx > CMakeFiles/net.dir/http_c_socket.cxx.i

src/net/CMakeFiles/net.dir/http_c_socket.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/http_c_socket.cxx.s"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/net/http_c_socket.cxx -o CMakeFiles/net.dir/http_c_socket.cxx.s

src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.o: src/net/CMakeFiles/net.dir/flags.make
src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.o: ../src/net/http_c_socket_connect.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.o"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/http_c_socket_connect.cxx.o -c /home/lyx/httpServer/src/net/http_c_socket_connect.cxx

src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/http_c_socket_connect.cxx.i"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/net/http_c_socket_connect.cxx > CMakeFiles/net.dir/http_c_socket_connect.cxx.i

src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/http_c_socket_connect.cxx.s"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/net/http_c_socket_connect.cxx -o CMakeFiles/net.dir/http_c_socket_connect.cxx.s

src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.o: src/net/CMakeFiles/net.dir/flags.make
src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.o: ../src/net/http_c_socket_accept.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.o"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/http_c_socket_accept.cxx.o -c /home/lyx/httpServer/src/net/http_c_socket_accept.cxx

src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/http_c_socket_accept.cxx.i"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/net/http_c_socket_accept.cxx > CMakeFiles/net.dir/http_c_socket_accept.cxx.i

src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/http_c_socket_accept.cxx.s"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/net/http_c_socket_accept.cxx -o CMakeFiles/net.dir/http_c_socket_accept.cxx.s

src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.o: src/net/CMakeFiles/net.dir/flags.make
src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.o: ../src/net/http_c_socket_request.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.o"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/http_c_socket_request.cxx.o -c /home/lyx/httpServer/src/net/http_c_socket_request.cxx

src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/http_c_socket_request.cxx.i"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/net/http_c_socket_request.cxx > CMakeFiles/net.dir/http_c_socket_request.cxx.i

src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/http_c_socket_request.cxx.s"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/net/http_c_socket_request.cxx -o CMakeFiles/net.dir/http_c_socket_request.cxx.s

src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.o: src/net/CMakeFiles/net.dir/flags.make
src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.o: ../src/net/http_c_socket_time.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.o"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/net.dir/http_c_socket_time.cxx.o -c /home/lyx/httpServer/src/net/http_c_socket_time.cxx

src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/net.dir/http_c_socket_time.cxx.i"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/net/http_c_socket_time.cxx > CMakeFiles/net.dir/http_c_socket_time.cxx.i

src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/net.dir/http_c_socket_time.cxx.s"
	cd /home/lyx/httpServer/build/src/net && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/net/http_c_socket_time.cxx -o CMakeFiles/net.dir/http_c_socket_time.cxx.s

# Object files for target net
net_OBJECTS = \
"CMakeFiles/net.dir/http_c_socket.cxx.o" \
"CMakeFiles/net.dir/http_c_socket_connect.cxx.o" \
"CMakeFiles/net.dir/http_c_socket_accept.cxx.o" \
"CMakeFiles/net.dir/http_c_socket_request.cxx.o" \
"CMakeFiles/net.dir/http_c_socket_time.cxx.o"

# External object files for target net
net_EXTERNAL_OBJECTS =

../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/http_c_socket.cxx.o
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/http_c_socket_connect.cxx.o
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/http_c_socket_accept.cxx.o
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/http_c_socket_request.cxx.o
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/http_c_socket_time.cxx.o
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/build.make
../_staticLib/libnet.a: src/net/CMakeFiles/net.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX static library ../../../_staticLib/libnet.a"
	cd /home/lyx/httpServer/build/src/net && $(CMAKE_COMMAND) -P CMakeFiles/net.dir/cmake_clean_target.cmake
	cd /home/lyx/httpServer/build/src/net && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/net.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/net/CMakeFiles/net.dir/build: ../_staticLib/libnet.a

.PHONY : src/net/CMakeFiles/net.dir/build

src/net/CMakeFiles/net.dir/clean:
	cd /home/lyx/httpServer/build/src/net && $(CMAKE_COMMAND) -P CMakeFiles/net.dir/cmake_clean.cmake
.PHONY : src/net/CMakeFiles/net.dir/clean

src/net/CMakeFiles/net.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer/src/net /home/lyx/httpServer/build /home/lyx/httpServer/build/src/net /home/lyx/httpServer/build/src/net/CMakeFiles/net.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/net/CMakeFiles/net.dir/depend

