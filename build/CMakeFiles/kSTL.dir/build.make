# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.21

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
CMAKE_COMMAND = /home/david/.cmake/bin/cmake

# The command to remove a file.
RM = /home/david/.cmake/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/david/Programmieren/C++/QT/kSTL

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/david/Programmieren/C++/QT/kSTL/build

# Include any dependencies generated for this target.
include CMakeFiles/kSTL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/kSTL.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/kSTL.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/kSTL.dir/flags.make

CMakeFiles/kSTL.dir/src/main.c.o: CMakeFiles/kSTL.dir/flags.make
CMakeFiles/kSTL.dir/src/main.c.o: ../src/main.c
CMakeFiles/kSTL.dir/src/main.c.o: CMakeFiles/kSTL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/Programmieren/C++/QT/kSTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/kSTL.dir/src/main.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/kSTL.dir/src/main.c.o -MF CMakeFiles/kSTL.dir/src/main.c.o.d -o CMakeFiles/kSTL.dir/src/main.c.o -c /home/david/Programmieren/C++/QT/kSTL/src/main.c

CMakeFiles/kSTL.dir/src/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kSTL.dir/src/main.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/david/Programmieren/C++/QT/kSTL/src/main.c > CMakeFiles/kSTL.dir/src/main.c.i

CMakeFiles/kSTL.dir/src/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kSTL.dir/src/main.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/david/Programmieren/C++/QT/kSTL/src/main.c -o CMakeFiles/kSTL.dir/src/main.c.s

CMakeFiles/kSTL.dir/src/vhelper.c.o: CMakeFiles/kSTL.dir/flags.make
CMakeFiles/kSTL.dir/src/vhelper.c.o: ../src/vhelper.c
CMakeFiles/kSTL.dir/src/vhelper.c.o: CMakeFiles/kSTL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/david/Programmieren/C++/QT/kSTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/kSTL.dir/src/vhelper.c.o"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT CMakeFiles/kSTL.dir/src/vhelper.c.o -MF CMakeFiles/kSTL.dir/src/vhelper.c.o.d -o CMakeFiles/kSTL.dir/src/vhelper.c.o -c /home/david/Programmieren/C++/QT/kSTL/src/vhelper.c

CMakeFiles/kSTL.dir/src/vhelper.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/kSTL.dir/src/vhelper.c.i"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/david/Programmieren/C++/QT/kSTL/src/vhelper.c > CMakeFiles/kSTL.dir/src/vhelper.c.i

CMakeFiles/kSTL.dir/src/vhelper.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/kSTL.dir/src/vhelper.c.s"
	gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/david/Programmieren/C++/QT/kSTL/src/vhelper.c -o CMakeFiles/kSTL.dir/src/vhelper.c.s

# Object files for target kSTL
kSTL_OBJECTS = \
"CMakeFiles/kSTL.dir/src/main.c.o" \
"CMakeFiles/kSTL.dir/src/vhelper.c.o"

# External object files for target kSTL
kSTL_EXTERNAL_OBJECTS =

kSTL: CMakeFiles/kSTL.dir/src/main.c.o
kSTL: CMakeFiles/kSTL.dir/src/vhelper.c.o
kSTL: CMakeFiles/kSTL.dir/build.make
kSTL: /usr/lib/x86_64-linux-gnu/libglfw.so.3
kSTL: /usr/lib/x86_64-linux-gnu/libGL.so
kSTL: /usr/lib/x86_64-linux-gnu/libGLEW.so
kSTL: /usr/lib/x86_64-linux-gnu/libvulkan.so
kSTL: CMakeFiles/kSTL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/david/Programmieren/C++/QT/kSTL/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking C executable kSTL"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kSTL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/kSTL.dir/build: kSTL
.PHONY : CMakeFiles/kSTL.dir/build

CMakeFiles/kSTL.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/kSTL.dir/cmake_clean.cmake
.PHONY : CMakeFiles/kSTL.dir/clean

CMakeFiles/kSTL.dir/depend:
	cd /home/david/Programmieren/C++/QT/kSTL/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/david/Programmieren/C++/QT/kSTL /home/david/Programmieren/C++/QT/kSTL /home/david/Programmieren/C++/QT/kSTL/build /home/david/Programmieren/C++/QT/kSTL/build /home/david/Programmieren/C++/QT/kSTL/build/CMakeFiles/kSTL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/kSTL.dir/depend

