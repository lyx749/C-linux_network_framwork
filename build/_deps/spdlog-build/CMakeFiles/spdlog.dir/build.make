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
include _deps/spdlog-build/CMakeFiles/spdlog.dir/depend.make

# Include the progress variables for this target.
include _deps/spdlog-build/CMakeFiles/spdlog.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.o: ../src/logs/spdlog/src/spdlog.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/spdlog.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/spdlog.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/spdlog.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/spdlog.cpp > CMakeFiles/spdlog.dir/src/spdlog.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/spdlog.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/spdlog.cpp -o CMakeFiles/spdlog.dir/src/spdlog.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o: ../src/logs/spdlog/src/stdout_sinks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/stdout_sinks.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/stdout_sinks.cpp > CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/stdout_sinks.cpp -o CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.o: ../src/logs/spdlog/src/color_sinks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/color_sinks.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/color_sinks.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/color_sinks.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/color_sinks.cpp > CMakeFiles/spdlog.dir/src/color_sinks.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/color_sinks.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/color_sinks.cpp -o CMakeFiles/spdlog.dir/src/color_sinks.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.o: ../src/logs/spdlog/src/file_sinks.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/file_sinks.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/file_sinks.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/file_sinks.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/file_sinks.cpp > CMakeFiles/spdlog.dir/src/file_sinks.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/file_sinks.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/file_sinks.cpp -o CMakeFiles/spdlog.dir/src/file_sinks.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.o: ../src/logs/spdlog/src/async.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/async.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/async.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/async.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/async.cpp > CMakeFiles/spdlog.dir/src/async.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/async.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/async.cpp -o CMakeFiles/spdlog.dir/src/async.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.o: ../src/logs/spdlog/src/cfg.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/cfg.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/cfg.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/cfg.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/cfg.cpp > CMakeFiles/spdlog.dir/src/cfg.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/cfg.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/cfg.cpp -o CMakeFiles/spdlog.dir/src/cfg.cpp.s

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.o: _deps/spdlog-build/CMakeFiles/spdlog.dir/flags.make
_deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.o: ../src/logs/spdlog/src/fmt.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object _deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.o"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/spdlog.dir/src/fmt.cpp.o -c /home/lyx/httpServer/src/logs/spdlog/src/fmt.cpp

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/spdlog.dir/src/fmt.cpp.i"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lyx/httpServer/src/logs/spdlog/src/fmt.cpp > CMakeFiles/spdlog.dir/src/fmt.cpp.i

_deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/spdlog.dir/src/fmt.cpp.s"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && /usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lyx/httpServer/src/logs/spdlog/src/fmt.cpp -o CMakeFiles/spdlog.dir/src/fmt.cpp.s

# Object files for target spdlog
spdlog_OBJECTS = \
"CMakeFiles/spdlog.dir/src/spdlog.cpp.o" \
"CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o" \
"CMakeFiles/spdlog.dir/src/color_sinks.cpp.o" \
"CMakeFiles/spdlog.dir/src/file_sinks.cpp.o" \
"CMakeFiles/spdlog.dir/src/async.cpp.o" \
"CMakeFiles/spdlog.dir/src/cfg.cpp.o" \
"CMakeFiles/spdlog.dir/src/fmt.cpp.o"

# External object files for target spdlog
spdlog_EXTERNAL_OBJECTS =

../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/spdlog.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/stdout_sinks.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/color_sinks.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/file_sinks.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/async.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/cfg.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/src/fmt.cpp.o
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/build.make
../_staticLib/libspdlogd.a: _deps/spdlog-build/CMakeFiles/spdlog.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lyx/httpServer/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Linking CXX static library ../../../_staticLib/libspdlogd.a"
	cd /home/lyx/httpServer/build/_deps/spdlog-build && $(CMAKE_COMMAND) -P CMakeFiles/spdlog.dir/cmake_clean_target.cmake
	cd /home/lyx/httpServer/build/_deps/spdlog-build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/spdlog.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/spdlog-build/CMakeFiles/spdlog.dir/build: ../_staticLib/libspdlogd.a

.PHONY : _deps/spdlog-build/CMakeFiles/spdlog.dir/build

_deps/spdlog-build/CMakeFiles/spdlog.dir/clean:
	cd /home/lyx/httpServer/build/_deps/spdlog-build && $(CMAKE_COMMAND) -P CMakeFiles/spdlog.dir/cmake_clean.cmake
.PHONY : _deps/spdlog-build/CMakeFiles/spdlog.dir/clean

_deps/spdlog-build/CMakeFiles/spdlog.dir/depend:
	cd /home/lyx/httpServer/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lyx/httpServer /home/lyx/httpServer/src/logs/spdlog /home/lyx/httpServer/build /home/lyx/httpServer/build/_deps/spdlog-build /home/lyx/httpServer/build/_deps/spdlog-build/CMakeFiles/spdlog.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/spdlog-build/CMakeFiles/spdlog.dir/depend

