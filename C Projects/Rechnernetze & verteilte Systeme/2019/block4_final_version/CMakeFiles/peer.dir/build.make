# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.7

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
CMAKE_SOURCE_DIR = "/mnt/c/Users/test/Desktop/C implementations/block4_final_version"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/mnt/c/Users/test/Desktop/C implementations/block4_final_version"

# Include any dependencies generated for this target.
include CMakeFiles/peer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/peer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/peer.dir/flags.make

CMakeFiles/peer.dir/peer.c.o: CMakeFiles/peer.dir/flags.make
CMakeFiles/peer.dir/peer.c.o: peer.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/peer.dir/peer.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/peer.dir/peer.c.o   -c "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/peer.c"

CMakeFiles/peer.dir/peer.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/peer.dir/peer.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/peer.c" > CMakeFiles/peer.dir/peer.c.i

CMakeFiles/peer.dir/peer.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/peer.dir/peer.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/peer.c" -o CMakeFiles/peer.dir/peer.c.s

CMakeFiles/peer.dir/peer.c.o.requires:

.PHONY : CMakeFiles/peer.dir/peer.c.o.requires

CMakeFiles/peer.dir/peer.c.o.provides: CMakeFiles/peer.dir/peer.c.o.requires
	$(MAKE) -f CMakeFiles/peer.dir/build.make CMakeFiles/peer.dir/peer.c.o.provides.build
.PHONY : CMakeFiles/peer.dir/peer.c.o.provides

CMakeFiles/peer.dir/peer.c.o.provides.build: CMakeFiles/peer.dir/peer.c.o


CMakeFiles/peer.dir/hashmap.c.o: CMakeFiles/peer.dir/flags.make
CMakeFiles/peer.dir/hashmap.c.o: hashmap.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Building C object CMakeFiles/peer.dir/hashmap.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/peer.dir/hashmap.c.o   -c "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/hashmap.c"

CMakeFiles/peer.dir/hashmap.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/peer.dir/hashmap.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/hashmap.c" > CMakeFiles/peer.dir/hashmap.c.i

CMakeFiles/peer.dir/hashmap.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/peer.dir/hashmap.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/hashmap.c" -o CMakeFiles/peer.dir/hashmap.c.s

CMakeFiles/peer.dir/hashmap.c.o.requires:

.PHONY : CMakeFiles/peer.dir/hashmap.c.o.requires

CMakeFiles/peer.dir/hashmap.c.o.provides: CMakeFiles/peer.dir/hashmap.c.o.requires
	$(MAKE) -f CMakeFiles/peer.dir/build.make CMakeFiles/peer.dir/hashmap.c.o.provides.build
.PHONY : CMakeFiles/peer.dir/hashmap.c.o.provides

CMakeFiles/peer.dir/hashmap.c.o.provides.build: CMakeFiles/peer.dir/hashmap.c.o


CMakeFiles/peer.dir/protokoll.c.o: CMakeFiles/peer.dir/flags.make
CMakeFiles/peer.dir/protokoll.c.o: protokoll.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/peer.dir/protokoll.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/peer.dir/protokoll.c.o   -c "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/protokoll.c"

CMakeFiles/peer.dir/protokoll.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/peer.dir/protokoll.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/protokoll.c" > CMakeFiles/peer.dir/protokoll.c.i

CMakeFiles/peer.dir/protokoll.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/peer.dir/protokoll.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/protokoll.c" -o CMakeFiles/peer.dir/protokoll.c.s

CMakeFiles/peer.dir/protokoll.c.o.requires:

.PHONY : CMakeFiles/peer.dir/protokoll.c.o.requires

CMakeFiles/peer.dir/protokoll.c.o.provides: CMakeFiles/peer.dir/protokoll.c.o.requires
	$(MAKE) -f CMakeFiles/peer.dir/build.make CMakeFiles/peer.dir/protokoll.c.o.provides.build
.PHONY : CMakeFiles/peer.dir/protokoll.c.o.provides

CMakeFiles/peer.dir/protokoll.c.o.provides.build: CMakeFiles/peer.dir/protokoll.c.o


CMakeFiles/peer.dir/connection.c.o: CMakeFiles/peer.dir/flags.make
CMakeFiles/peer.dir/connection.c.o: connection.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_4) "Building C object CMakeFiles/peer.dir/connection.c.o"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/peer.dir/connection.c.o   -c "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/connection.c"

CMakeFiles/peer.dir/connection.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/peer.dir/connection.c.i"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/connection.c" > CMakeFiles/peer.dir/connection.c.i

CMakeFiles/peer.dir/connection.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/peer.dir/connection.c.s"
	/usr/bin/cc  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/connection.c" -o CMakeFiles/peer.dir/connection.c.s

CMakeFiles/peer.dir/connection.c.o.requires:

.PHONY : CMakeFiles/peer.dir/connection.c.o.requires

CMakeFiles/peer.dir/connection.c.o.provides: CMakeFiles/peer.dir/connection.c.o.requires
	$(MAKE) -f CMakeFiles/peer.dir/build.make CMakeFiles/peer.dir/connection.c.o.provides.build
.PHONY : CMakeFiles/peer.dir/connection.c.o.provides

CMakeFiles/peer.dir/connection.c.o.provides.build: CMakeFiles/peer.dir/connection.c.o


# Object files for target peer
peer_OBJECTS = \
"CMakeFiles/peer.dir/peer.c.o" \
"CMakeFiles/peer.dir/hashmap.c.o" \
"CMakeFiles/peer.dir/protokoll.c.o" \
"CMakeFiles/peer.dir/connection.c.o"

# External object files for target peer
peer_EXTERNAL_OBJECTS =

peer: CMakeFiles/peer.dir/peer.c.o
peer: CMakeFiles/peer.dir/hashmap.c.o
peer: CMakeFiles/peer.dir/protokoll.c.o
peer: CMakeFiles/peer.dir/connection.c.o
peer: CMakeFiles/peer.dir/build.make
peer: CMakeFiles/peer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_5) "Linking C executable peer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/peer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/peer.dir/build: peer

.PHONY : CMakeFiles/peer.dir/build

CMakeFiles/peer.dir/requires: CMakeFiles/peer.dir/peer.c.o.requires
CMakeFiles/peer.dir/requires: CMakeFiles/peer.dir/hashmap.c.o.requires
CMakeFiles/peer.dir/requires: CMakeFiles/peer.dir/protokoll.c.o.requires
CMakeFiles/peer.dir/requires: CMakeFiles/peer.dir/connection.c.o.requires

.PHONY : CMakeFiles/peer.dir/requires

CMakeFiles/peer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/peer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/peer.dir/clean

CMakeFiles/peer.dir/depend:
	cd "/mnt/c/Users/test/Desktop/C implementations/block4_final_version" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/mnt/c/Users/test/Desktop/C implementations/block4_final_version" "/mnt/c/Users/test/Desktop/C implementations/block4_final_version" "/mnt/c/Users/test/Desktop/C implementations/block4_final_version" "/mnt/c/Users/test/Desktop/C implementations/block4_final_version" "/mnt/c/Users/test/Desktop/C implementations/block4_final_version/CMakeFiles/peer.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/peer.dir/depend

