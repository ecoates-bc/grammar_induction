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
CMAKE_SOURCE_DIR = /home/edith/LING/448/code/rpni_cpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/edith/LING/448/code/rpni_cpp/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/rpni_cpp.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/rpni_cpp.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/rpni_cpp.dir/flags.make

CMakeFiles/rpni_cpp.dir/automaton.cpp.o: CMakeFiles/rpni_cpp.dir/flags.make
CMakeFiles/rpni_cpp.dir/automaton.cpp.o: ../automaton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/edith/LING/448/code/rpni_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/rpni_cpp.dir/automaton.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rpni_cpp.dir/automaton.cpp.o -c /home/edith/LING/448/code/rpni_cpp/automaton.cpp

CMakeFiles/rpni_cpp.dir/automaton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpni_cpp.dir/automaton.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/edith/LING/448/code/rpni_cpp/automaton.cpp > CMakeFiles/rpni_cpp.dir/automaton.cpp.i

CMakeFiles/rpni_cpp.dir/automaton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpni_cpp.dir/automaton.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/edith/LING/448/code/rpni_cpp/automaton.cpp -o CMakeFiles/rpni_cpp.dir/automaton.cpp.s

CMakeFiles/rpni_cpp.dir/main.cpp.o: CMakeFiles/rpni_cpp.dir/flags.make
CMakeFiles/rpni_cpp.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/edith/LING/448/code/rpni_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/rpni_cpp.dir/main.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rpni_cpp.dir/main.cpp.o -c /home/edith/LING/448/code/rpni_cpp/main.cpp

CMakeFiles/rpni_cpp.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpni_cpp.dir/main.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/edith/LING/448/code/rpni_cpp/main.cpp > CMakeFiles/rpni_cpp.dir/main.cpp.i

CMakeFiles/rpni_cpp.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpni_cpp.dir/main.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/edith/LING/448/code/rpni_cpp/main.cpp -o CMakeFiles/rpni_cpp.dir/main.cpp.s

CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o: CMakeFiles/rpni_cpp.dir/flags.make
CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o: ../rpni_eval.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/edith/LING/448/code/rpni_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o"
	/usr/bin/g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o -c /home/edith/LING/448/code/rpni_cpp/rpni_eval.cpp

CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.i"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/edith/LING/448/code/rpni_cpp/rpni_eval.cpp > CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.i

CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.s"
	/usr/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/edith/LING/448/code/rpni_cpp/rpni_eval.cpp -o CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.s

# Object files for target rpni_cpp
rpni_cpp_OBJECTS = \
"CMakeFiles/rpni_cpp.dir/automaton.cpp.o" \
"CMakeFiles/rpni_cpp.dir/main.cpp.o" \
"CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o"

# External object files for target rpni_cpp
rpni_cpp_EXTERNAL_OBJECTS =

rpni_cpp: CMakeFiles/rpni_cpp.dir/automaton.cpp.o
rpni_cpp: CMakeFiles/rpni_cpp.dir/main.cpp.o
rpni_cpp: CMakeFiles/rpni_cpp.dir/rpni_eval.cpp.o
rpni_cpp: CMakeFiles/rpni_cpp.dir/build.make
rpni_cpp: CMakeFiles/rpni_cpp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/edith/LING/448/code/rpni_cpp/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable rpni_cpp"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/rpni_cpp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/rpni_cpp.dir/build: rpni_cpp

.PHONY : CMakeFiles/rpni_cpp.dir/build

CMakeFiles/rpni_cpp.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/rpni_cpp.dir/cmake_clean.cmake
.PHONY : CMakeFiles/rpni_cpp.dir/clean

CMakeFiles/rpni_cpp.dir/depend:
	cd /home/edith/LING/448/code/rpni_cpp/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/edith/LING/448/code/rpni_cpp /home/edith/LING/448/code/rpni_cpp /home/edith/LING/448/code/rpni_cpp/cmake-build-debug /home/edith/LING/448/code/rpni_cpp/cmake-build-debug /home/edith/LING/448/code/rpni_cpp/cmake-build-debug/CMakeFiles/rpni_cpp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/rpni_cpp.dir/depend

