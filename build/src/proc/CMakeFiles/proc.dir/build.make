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
include src/proc/CMakeFiles/proc.dir/depend.make

# Include the progress variables for this target.
include src/proc/CMakeFiles/proc.dir/progress.make

# Include the compile flags for this target's objects.
include src/proc/CMakeFiles/proc.dir/flags.make

src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.o: src/proc/CMakeFiles/proc.dir/flags.make
src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.o: ../src/proc/http_c_daemon.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.o"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proc.dir/http_c_daemon.cxx.o -c /home/lyx/httpServer/src/proc/http_c_daemon.cxx

src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proc.dir/http_c_daemon.cxx.i"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/proc/http_c_daemon.cxx > CMakeFiles/proc.dir/http_c_daemon.cxx.i

src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proc.dir/http_c_daemon.cxx.s"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/proc/http_c_daemon.cxx -o CMakeFiles/proc.dir/http_c_daemon.cxx.s

src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.o: src/proc/CMakeFiles/proc.dir/flags.make
src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.o: ../src/proc/http_c_process_cycle.cxx
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.o"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/proc.dir/http_c_process_cycle.cxx.o -c /home/lyx/httpServer/src/proc/http_c_process_cycle.cxx

src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/proc.dir/http_c_process_cycle.cxx.i"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/proc/http_c_process_cycle.cxx > CMakeFiles/proc.dir/http_c_process_cycle.cxx.i

src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/proc.dir/http_c_process_cycle.cxx.s"
	cd /home/lyx/httpServer/build/src/proc && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/proc/http_c_process_cycle.cxx -o CMakeFiles/proc.dir/http_c_process_cycle.cxx.s

# Object files for target proc
proc_OBJECTS = \
"CMakeFiles/proc.dir/http_c_daemon.cxx.o" \
"CMakeFiles/proc.dir/http_c_process_cycle.cxx.o"

# External object files for target proc
proc_EXTERNAL_OBJECTS =

../_staticLib/libproc.a: src/proc/CMakeFiles/proc.dir/http_c_daemon.cxx.o
../_staticLib/libproc.a: src/proc/CMakeFiles/proc.dir/http_c_process_cycle.cxx.o
../_staticLib/libproc.a: src/proc/CMakeFiles/proc.dir/build.make
../_staticLib/libproc.a: src/proc/CMakeFiles/proc.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library ../../../_staticLib/libproc.a"
	cd /home/lyx/httpServer/build/src/proc && $(CMAKE_COMMAND) -P CMakeFiles/proc.dir/cmake_clean_target.cmake
	cd /home/lyx/httpServer/build/src/proc && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/proc.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
src/proc/CMakeFiles/proc.dir/build: ../_staticLib/libproc.a

.PHONY : src/proc/CMakeFiles/proc.dir/build

src/proc/CMakeFiles/proc.dir/clean:
	cd /home/lyx/httpServer/build/src/proc && $(CMAKE_COMMAND) -P CMakeFiles/proc.dir/cmake_clean.cmake
.PHONY : src/proc/CMakeFiles/proc.dir/clean

src/proc/CMakeFiles/proc.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer/src/proc /home/lyx/httpServer/build /home/lyx/httpServer/build/src/proc /home/lyx/httpServer/build/src/proc/CMakeFiles/proc.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : src/proc/CMakeFiles/proc.dir/depend

