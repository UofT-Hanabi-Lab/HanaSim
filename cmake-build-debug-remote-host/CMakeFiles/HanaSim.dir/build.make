# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /tmp/tmp.zB8BHBXaKg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host

# Include any dependencies generated for this target.
include CMakeFiles/HanaSim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HanaSim.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HanaSim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HanaSim.dir/flags.make

CMakeFiles/HanaSim.dir/src/card.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/card.cpp.o: ../src/card.cpp
CMakeFiles/HanaSim.dir/src/card.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HanaSim.dir/src/card.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/card.cpp.o -MF CMakeFiles/HanaSim.dir/src/card.cpp.o.d -o CMakeFiles/HanaSim.dir/src/card.cpp.o -c /tmp/tmp.zB8BHBXaKg/src/card.cpp

CMakeFiles/HanaSim.dir/src/card.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/card.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/src/card.cpp > CMakeFiles/HanaSim.dir/src/card.cpp.i

CMakeFiles/HanaSim.dir/src/card.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/card.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/src/card.cpp -o CMakeFiles/HanaSim.dir/src/card.cpp.s

CMakeFiles/HanaSim.dir/src/game.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/game.cpp.o: ../src/game.cpp
CMakeFiles/HanaSim.dir/src/game.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HanaSim.dir/src/game.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/game.cpp.o -MF CMakeFiles/HanaSim.dir/src/game.cpp.o.d -o CMakeFiles/HanaSim.dir/src/game.cpp.o -c /tmp/tmp.zB8BHBXaKg/src/game.cpp

CMakeFiles/HanaSim.dir/src/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/game.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/src/game.cpp > CMakeFiles/HanaSim.dir/src/game.cpp.i

CMakeFiles/HanaSim.dir/src/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/game.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/src/game.cpp -o CMakeFiles/HanaSim.dir/src/game.cpp.s

CMakeFiles/HanaSim.dir/src/main.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/main.cpp.o: ../src/main.cpp
CMakeFiles/HanaSim.dir/src/main.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HanaSim.dir/src/main.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/main.cpp.o -MF CMakeFiles/HanaSim.dir/src/main.cpp.o.d -o CMakeFiles/HanaSim.dir/src/main.cpp.o -c /tmp/tmp.zB8BHBXaKg/src/main.cpp

CMakeFiles/HanaSim.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/main.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/src/main.cpp > CMakeFiles/HanaSim.dir/src/main.cpp.i

CMakeFiles/HanaSim.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/main.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/src/main.cpp -o CMakeFiles/HanaSim.dir/src/main.cpp.s

CMakeFiles/HanaSim.dir/src/move.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/move.cpp.o: ../src/move.cpp
CMakeFiles/HanaSim.dir/src/move.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/HanaSim.dir/src/move.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/move.cpp.o -MF CMakeFiles/HanaSim.dir/src/move.cpp.o.d -o CMakeFiles/HanaSim.dir/src/move.cpp.o -c /tmp/tmp.zB8BHBXaKg/src/move.cpp

CMakeFiles/HanaSim.dir/src/move.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/move.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/src/move.cpp > CMakeFiles/HanaSim.dir/src/move.cpp.i

CMakeFiles/HanaSim.dir/src/move.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/move.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/src/move.cpp -o CMakeFiles/HanaSim.dir/src/move.cpp.s

CMakeFiles/HanaSim.dir/src/state.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/state.cpp.o: ../src/state.cpp
CMakeFiles/HanaSim.dir/src/state.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/HanaSim.dir/src/state.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/state.cpp.o -MF CMakeFiles/HanaSim.dir/src/state.cpp.o.d -o CMakeFiles/HanaSim.dir/src/state.cpp.o -c /tmp/tmp.zB8BHBXaKg/src/state.cpp

CMakeFiles/HanaSim.dir/src/state.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/state.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/src/state.cpp > CMakeFiles/HanaSim.dir/src/state.cpp.i

CMakeFiles/HanaSim.dir/src/state.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/state.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/src/state.cpp -o CMakeFiles/HanaSim.dir/src/state.cpp.s

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: ../bots/consolebot.cpp
CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/consolebot.cpp

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/consolebot.cpp > CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/consolebot.cpp -o CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: ../bots/holmesbot.cpp
CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/holmesbot.cpp

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/holmesbot.cpp > CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/holmesbot.cpp -o CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: ../bots/humanplayer.cpp
CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o -MF CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/humanplayer.cpp

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/humanplayer.cpp > CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/humanplayer.cpp -o CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s

CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: ../bots/randombot.cpp
CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/HanaSim.dir/bots/randombot.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/randombot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/randombot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/randombot.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/randombot.cpp

CMakeFiles/HanaSim.dir/bots/randombot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/randombot.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/randombot.cpp > CMakeFiles/HanaSim.dir/bots/randombot.cpp.i

CMakeFiles/HanaSim.dir/bots/randombot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/randombot.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/randombot.cpp -o CMakeFiles/HanaSim.dir/bots/randombot.cpp.s

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: ../bots/simplebot.cpp
CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/simplebot.cpp

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/simplebot.cpp > CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/simplebot.cpp -o CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s

CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o: ../bots/smartbot.cpp
CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o -c /tmp/tmp.zB8BHBXaKg/bots/smartbot.cpp

CMakeFiles/HanaSim.dir/bots/smartbot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/smartbot.cpp.i"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /tmp/tmp.zB8BHBXaKg/bots/smartbot.cpp > CMakeFiles/HanaSim.dir/bots/smartbot.cpp.i

CMakeFiles/HanaSim.dir/bots/smartbot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/smartbot.cpp.s"
	/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /tmp/tmp.zB8BHBXaKg/bots/smartbot.cpp -o CMakeFiles/HanaSim.dir/bots/smartbot.cpp.s

# Object files for target HanaSim
HanaSim_OBJECTS = \
"CMakeFiles/HanaSim.dir/src/card.cpp.o" \
"CMakeFiles/HanaSim.dir/src/game.cpp.o" \
"CMakeFiles/HanaSim.dir/src/main.cpp.o" \
"CMakeFiles/HanaSim.dir/src/move.cpp.o" \
"CMakeFiles/HanaSim.dir/src/state.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/randombot.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o" \
"CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o"

# External object files for target HanaSim
HanaSim_EXTERNAL_OBJECTS =

HanaSim: CMakeFiles/HanaSim.dir/src/card.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/src/game.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/src/main.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/src/move.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/src/state.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/randombot.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/bots/smartbot.cpp.o
HanaSim: CMakeFiles/HanaSim.dir/build.make
HanaSim: ../include/libtorch/lib/libc10.so
HanaSim: ../include/libtorch/lib/libkineto.a
HanaSim: ../include/libtorch/lib/libtorch.dylib
HanaSim: ../include/libtorch/lib/libtorch_cpu.dylib
HanaSim: ../include/libtorch/lib/libc10.dylib
HanaSim: /usr/lib/gcc/x86_64-linux-gnu/11/libgomp.so
HanaSim: /usr/lib/x86_64-linux-gnu/libpthread.a
HanaSim: CMakeFiles/HanaSim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Linking CXX executable HanaSim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HanaSim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HanaSim.dir/build: HanaSim
.PHONY : CMakeFiles/HanaSim.dir/build

CMakeFiles/HanaSim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HanaSim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HanaSim.dir/clean

CMakeFiles/HanaSim.dir/depend:
	cd /tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /tmp/tmp.zB8BHBXaKg /tmp/tmp.zB8BHBXaKg /tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host /tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host /tmp/tmp.zB8BHBXaKg/cmake-build-debug-remote-host/CMakeFiles/HanaSim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HanaSim.dir/depend

