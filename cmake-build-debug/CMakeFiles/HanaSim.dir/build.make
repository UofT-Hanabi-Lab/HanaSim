# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/aarch64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/danielxu/Desktop/CSC494/HanaSim

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/HanaSim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HanaSim.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HanaSim.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HanaSim.dir/flags.make

CMakeFiles/HanaSim.dir/src/card.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/card.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/src/card.cpp
CMakeFiles/HanaSim.dir/src/card.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HanaSim.dir/src/card.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/card.cpp.o -MF CMakeFiles/HanaSim.dir/src/card.cpp.o.d -o CMakeFiles/HanaSim.dir/src/card.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/src/card.cpp

CMakeFiles/HanaSim.dir/src/card.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/card.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/src/card.cpp > CMakeFiles/HanaSim.dir/src/card.cpp.i

CMakeFiles/HanaSim.dir/src/card.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/card.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/src/card.cpp -o CMakeFiles/HanaSim.dir/src/card.cpp.s

CMakeFiles/HanaSim.dir/src/game.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/game.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/src/game.cpp
CMakeFiles/HanaSim.dir/src/game.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/HanaSim.dir/src/game.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/game.cpp.o -MF CMakeFiles/HanaSim.dir/src/game.cpp.o.d -o CMakeFiles/HanaSim.dir/src/game.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/src/game.cpp

CMakeFiles/HanaSim.dir/src/game.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/game.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/src/game.cpp > CMakeFiles/HanaSim.dir/src/game.cpp.i

CMakeFiles/HanaSim.dir/src/game.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/game.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/src/game.cpp -o CMakeFiles/HanaSim.dir/src/game.cpp.s

CMakeFiles/HanaSim.dir/src/main.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/main.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/src/main.cpp
CMakeFiles/HanaSim.dir/src/main.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/HanaSim.dir/src/main.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/main.cpp.o -MF CMakeFiles/HanaSim.dir/src/main.cpp.o.d -o CMakeFiles/HanaSim.dir/src/main.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/src/main.cpp

CMakeFiles/HanaSim.dir/src/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/main.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/src/main.cpp > CMakeFiles/HanaSim.dir/src/main.cpp.i

CMakeFiles/HanaSim.dir/src/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/main.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/src/main.cpp -o CMakeFiles/HanaSim.dir/src/main.cpp.s

CMakeFiles/HanaSim.dir/src/move.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/move.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/src/move.cpp
CMakeFiles/HanaSim.dir/src/move.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/HanaSim.dir/src/move.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/move.cpp.o -MF CMakeFiles/HanaSim.dir/src/move.cpp.o.d -o CMakeFiles/HanaSim.dir/src/move.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/src/move.cpp

CMakeFiles/HanaSim.dir/src/move.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/move.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/src/move.cpp > CMakeFiles/HanaSim.dir/src/move.cpp.i

CMakeFiles/HanaSim.dir/src/move.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/move.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/src/move.cpp -o CMakeFiles/HanaSim.dir/src/move.cpp.s

CMakeFiles/HanaSim.dir/src/state.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/src/state.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/src/state.cpp
CMakeFiles/HanaSim.dir/src/state.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/HanaSim.dir/src/state.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/src/state.cpp.o -MF CMakeFiles/HanaSim.dir/src/state.cpp.o.d -o CMakeFiles/HanaSim.dir/src/state.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/src/state.cpp

CMakeFiles/HanaSim.dir/src/state.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/src/state.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/src/state.cpp > CMakeFiles/HanaSim.dir/src/state.cpp.i

CMakeFiles/HanaSim.dir/src/state.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/src/state.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/src/state.cpp -o CMakeFiles/HanaSim.dir/src/state.cpp.s

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/bots/consolebot.cpp
CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/consolebot.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/bots/consolebot.cpp

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/bots/consolebot.cpp > CMakeFiles/HanaSim.dir/bots/consolebot.cpp.i

CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/bots/consolebot.cpp -o CMakeFiles/HanaSim.dir/bots/consolebot.cpp.s

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/bots/holmesbot.cpp
CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/bots/holmesbot.cpp

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/bots/holmesbot.cpp > CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.i

CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/bots/holmesbot.cpp -o CMakeFiles/HanaSim.dir/bots/holmesbot.cpp.s

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/bots/humanplayer.cpp
CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o -MF CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/bots/humanplayer.cpp

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/bots/humanplayer.cpp > CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.i

CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/bots/humanplayer.cpp -o CMakeFiles/HanaSim.dir/bots/humanplayer.cpp.s

CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/bots/randombot.cpp
CMakeFiles/HanaSim.dir/bots/randombot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/HanaSim.dir/bots/randombot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/randombot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/randombot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/randombot.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/bots/randombot.cpp

CMakeFiles/HanaSim.dir/bots/randombot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/randombot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/bots/randombot.cpp > CMakeFiles/HanaSim.dir/bots/randombot.cpp.i

CMakeFiles/HanaSim.dir/bots/randombot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/randombot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/bots/randombot.cpp -o CMakeFiles/HanaSim.dir/bots/randombot.cpp.s

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: CMakeFiles/HanaSim.dir/flags.make
CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: /Users/danielxu/Desktop/CSC494/HanaSim/bots/simplebot.cpp
CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o: CMakeFiles/HanaSim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o -MF CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o.d -o CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o -c /Users/danielxu/Desktop/CSC494/HanaSim/bots/simplebot.cpp

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/danielxu/Desktop/CSC494/HanaSim/bots/simplebot.cpp > CMakeFiles/HanaSim.dir/bots/simplebot.cpp.i

CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/danielxu/Desktop/CSC494/HanaSim/bots/simplebot.cpp -o CMakeFiles/HanaSim.dir/bots/simplebot.cpp.s

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
"CMakeFiles/HanaSim.dir/bots/simplebot.cpp.o"

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
HanaSim: CMakeFiles/HanaSim.dir/build.make
HanaSim: /Library/Frameworks/Python.framework/Versions/3.10/lib/libpython3.10.dylib
HanaSim: CMakeFiles/HanaSim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Linking CXX executable HanaSim"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HanaSim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HanaSim.dir/build: HanaSim
.PHONY : CMakeFiles/HanaSim.dir/build

CMakeFiles/HanaSim.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HanaSim.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HanaSim.dir/clean

CMakeFiles/HanaSim.dir/depend:
	cd /Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/danielxu/Desktop/CSC494/HanaSim /Users/danielxu/Desktop/CSC494/HanaSim /Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug /Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug /Users/danielxu/Desktop/CSC494/HanaSim/cmake-build-debug/CMakeFiles/HanaSim.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/HanaSim.dir/depend

