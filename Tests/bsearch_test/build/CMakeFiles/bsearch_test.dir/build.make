# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.15

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\CMake\bin\cmake.exe

# The command to remove a file.
RM = C:\CMake\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\ELIO\source\DB\Tests\bsearch_test

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\ELIO\source\DB\Tests\bsearch_test\build

# Include any dependencies generated for this target.
include CMakeFiles/bsearch_test.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bsearch_test.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bsearch_test.dir/flags.make

CMakeFiles/bsearch_test.dir/bsearch_test.c.obj: CMakeFiles/bsearch_test.dir/flags.make
CMakeFiles/bsearch_test.dir/bsearch_test.c.obj: ../bsearch_test.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\ELIO\source\DB\Tests\bsearch_test\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bsearch_test.dir/bsearch_test.c.obj"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\bsearch_test.dir\bsearch_test.c.obj   -c C:\Users\ELIO\source\DB\Tests\bsearch_test\bsearch_test.c

CMakeFiles/bsearch_test.dir/bsearch_test.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bsearch_test.dir/bsearch_test.c.i"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E C:\Users\ELIO\source\DB\Tests\bsearch_test\bsearch_test.c > CMakeFiles\bsearch_test.dir\bsearch_test.c.i

CMakeFiles/bsearch_test.dir/bsearch_test.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bsearch_test.dir/bsearch_test.c.s"
	C:\MinGW\bin\gcc.exe $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S C:\Users\ELIO\source\DB\Tests\bsearch_test\bsearch_test.c -o CMakeFiles\bsearch_test.dir\bsearch_test.c.s

# Object files for target bsearch_test
bsearch_test_OBJECTS = \
"CMakeFiles/bsearch_test.dir/bsearch_test.c.obj"

# External object files for target bsearch_test
bsearch_test_EXTERNAL_OBJECTS =

bsearch_test.exe: CMakeFiles/bsearch_test.dir/bsearch_test.c.obj
bsearch_test.exe: CMakeFiles/bsearch_test.dir/build.make
bsearch_test.exe: CMakeFiles/bsearch_test.dir/linklibs.rsp
bsearch_test.exe: CMakeFiles/bsearch_test.dir/objects1.rsp
bsearch_test.exe: CMakeFiles/bsearch_test.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\ELIO\source\DB\Tests\bsearch_test\build\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bsearch_test.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\bsearch_test.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bsearch_test.dir/build: bsearch_test.exe

.PHONY : CMakeFiles/bsearch_test.dir/build

CMakeFiles/bsearch_test.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\bsearch_test.dir\cmake_clean.cmake
.PHONY : CMakeFiles/bsearch_test.dir/clean

CMakeFiles/bsearch_test.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\ELIO\source\DB\Tests\bsearch_test C:\Users\ELIO\source\DB\Tests\bsearch_test C:\Users\ELIO\source\DB\Tests\bsearch_test\build C:\Users\ELIO\source\DB\Tests\bsearch_test\build C:\Users\ELIO\source\DB\Tests\bsearch_test\build\CMakeFiles\bsearch_test.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bsearch_test.dir/depend

