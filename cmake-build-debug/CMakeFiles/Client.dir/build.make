# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

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
CMAKE_COMMAND = /home/kovalev_s/.local/lib/python2.7/site-packages/cmake/data/bin/cmake

# The command to remove a file.
RM = /home/kovalev_s/.local/lib/python2.7/site-packages/cmake/data/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/kovalev_s/code/learn

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/kovalev_s/code/learn/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Client.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Client.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Client.dir/flags.make

CMakeFiles/Client.dir/src/lib/author.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/author.cpp.o: ../src/lib/author.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Client.dir/src/lib/author.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/author.cpp.o -c /home/kovalev_s/code/learn/src/lib/author.cpp

CMakeFiles/Client.dir/src/lib/author.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/author.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/author.cpp > CMakeFiles/Client.dir/src/lib/author.cpp.i

CMakeFiles/Client.dir/src/lib/author.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/author.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/author.cpp -o CMakeFiles/Client.dir/src/lib/author.cpp.s

CMakeFiles/Client.dir/src/lib/book.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/book.cpp.o: ../src/lib/book.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Client.dir/src/lib/book.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/book.cpp.o -c /home/kovalev_s/code/learn/src/lib/book.cpp

CMakeFiles/Client.dir/src/lib/book.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/book.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/book.cpp > CMakeFiles/Client.dir/src/lib/book.cpp.i

CMakeFiles/Client.dir/src/lib/book.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/book.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/book.cpp -o CMakeFiles/Client.dir/src/lib/book.cpp.s

CMakeFiles/Client.dir/src/lib/file_storage.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/file_storage.cpp.o: ../src/lib/file_storage.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/Client.dir/src/lib/file_storage.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/file_storage.cpp.o -c /home/kovalev_s/code/learn/src/lib/file_storage.cpp

CMakeFiles/Client.dir/src/lib/file_storage.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/file_storage.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/file_storage.cpp > CMakeFiles/Client.dir/src/lib/file_storage.cpp.i

CMakeFiles/Client.dir/src/lib/file_storage.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/file_storage.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/file_storage.cpp -o CMakeFiles/Client.dir/src/lib/file_storage.cpp.s

CMakeFiles/Client.dir/src/lib/json_parser.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/json_parser.cpp.o: ../src/lib/json_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/Client.dir/src/lib/json_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/json_parser.cpp.o -c /home/kovalev_s/code/learn/src/lib/json_parser.cpp

CMakeFiles/Client.dir/src/lib/json_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/json_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/json_parser.cpp > CMakeFiles/Client.dir/src/lib/json_parser.cpp.i

CMakeFiles/Client.dir/src/lib/json_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/json_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/json_parser.cpp -o CMakeFiles/Client.dir/src/lib/json_parser.cpp.s

CMakeFiles/Client.dir/src/lib/library.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/library.cpp.o: ../src/lib/library.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/Client.dir/src/lib/library.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/library.cpp.o -c /home/kovalev_s/code/learn/src/lib/library.cpp

CMakeFiles/Client.dir/src/lib/library.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/library.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/library.cpp > CMakeFiles/Client.dir/src/lib/library.cpp.i

CMakeFiles/Client.dir/src/lib/library.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/library.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/library.cpp -o CMakeFiles/Client.dir/src/lib/library.cpp.s

CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o: ../src/lib/socket_tcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Building CXX object CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o -c /home/kovalev_s/code/learn/src/lib/socket_tcp.cpp

CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/socket_tcp.cpp > CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.i

CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/socket_tcp.cpp -o CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.s

CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o: ../src/lib/socket_udp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Building CXX object CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o -c /home/kovalev_s/code/learn/src/lib/socket_udp.cpp

CMakeFiles/Client.dir/src/lib/socket_udp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/socket_udp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/socket_udp.cpp > CMakeFiles/Client.dir/src/lib/socket_udp.cpp.i

CMakeFiles/Client.dir/src/lib/socket_udp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/socket_udp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/socket_udp.cpp -o CMakeFiles/Client.dir/src/lib/socket_udp.cpp.s

CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o: ../src/lib/xml_parser.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Building CXX object CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o -c /home/kovalev_s/code/learn/src/lib/xml_parser.cpp

CMakeFiles/Client.dir/src/lib/xml_parser.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/lib/xml_parser.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/lib/xml_parser.cpp > CMakeFiles/Client.dir/src/lib/xml_parser.cpp.i

CMakeFiles/Client.dir/src/lib/xml_parser.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/lib/xml_parser.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/lib/xml_parser.cpp -o CMakeFiles/Client.dir/src/lib/xml_parser.cpp.s

CMakeFiles/Client.dir/src/client/client.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/client/client.cpp.o: ../src/client/client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Building CXX object CMakeFiles/Client.dir/src/client/client.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/client/client.cpp.o -c /home/kovalev_s/code/learn/src/client/client.cpp

CMakeFiles/Client.dir/src/client/client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/client/client.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/client/client.cpp > CMakeFiles/Client.dir/src/client/client.cpp.i

CMakeFiles/Client.dir/src/client/client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/client/client.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/client/client.cpp -o CMakeFiles/Client.dir/src/client/client.cpp.s

CMakeFiles/Client.dir/src/client/client_tcp.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/client/client_tcp.cpp.o: ../src/client/client_tcp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_10) "Building CXX object CMakeFiles/Client.dir/src/client/client_tcp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/client/client_tcp.cpp.o -c /home/kovalev_s/code/learn/src/client/client_tcp.cpp

CMakeFiles/Client.dir/src/client/client_tcp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/client/client_tcp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/client/client_tcp.cpp > CMakeFiles/Client.dir/src/client/client_tcp.cpp.i

CMakeFiles/Client.dir/src/client/client_tcp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/client/client_tcp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/client/client_tcp.cpp -o CMakeFiles/Client.dir/src/client/client_tcp.cpp.s

CMakeFiles/Client.dir/src/client/client_udp.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/client/client_udp.cpp.o: ../src/client/client_udp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_11) "Building CXX object CMakeFiles/Client.dir/src/client/client_udp.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/client/client_udp.cpp.o -c /home/kovalev_s/code/learn/src/client/client_udp.cpp

CMakeFiles/Client.dir/src/client/client_udp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/client/client_udp.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/client/client_udp.cpp > CMakeFiles/Client.dir/src/client/client_udp.cpp.i

CMakeFiles/Client.dir/src/client/client_udp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/client/client_udp.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/client/client_udp.cpp -o CMakeFiles/Client.dir/src/client/client_udp.cpp.s

CMakeFiles/Client.dir/src/client/main.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/client/main.cpp.o: ../src/client/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_12) "Building CXX object CMakeFiles/Client.dir/src/client/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/client/main.cpp.o -c /home/kovalev_s/code/learn/src/client/main.cpp

CMakeFiles/Client.dir/src/client/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/client/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/client/main.cpp > CMakeFiles/Client.dir/src/client/main.cpp.i

CMakeFiles/Client.dir/src/client/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/client/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/client/main.cpp -o CMakeFiles/Client.dir/src/client/main.cpp.s

CMakeFiles/Client.dir/src/client/string_queue.cpp.o: CMakeFiles/Client.dir/flags.make
CMakeFiles/Client.dir/src/client/string_queue.cpp.o: ../src/client/string_queue.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_13) "Building CXX object CMakeFiles/Client.dir/src/client/string_queue.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/Client.dir/src/client/string_queue.cpp.o -c /home/kovalev_s/code/learn/src/client/string_queue.cpp

CMakeFiles/Client.dir/src/client/string_queue.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Client.dir/src/client/string_queue.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/kovalev_s/code/learn/src/client/string_queue.cpp > CMakeFiles/Client.dir/src/client/string_queue.cpp.i

CMakeFiles/Client.dir/src/client/string_queue.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Client.dir/src/client/string_queue.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/kovalev_s/code/learn/src/client/string_queue.cpp -o CMakeFiles/Client.dir/src/client/string_queue.cpp.s

# Object files for target Client
Client_OBJECTS = \
"CMakeFiles/Client.dir/src/lib/author.cpp.o" \
"CMakeFiles/Client.dir/src/lib/book.cpp.o" \
"CMakeFiles/Client.dir/src/lib/file_storage.cpp.o" \
"CMakeFiles/Client.dir/src/lib/json_parser.cpp.o" \
"CMakeFiles/Client.dir/src/lib/library.cpp.o" \
"CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o" \
"CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o" \
"CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o" \
"CMakeFiles/Client.dir/src/client/client.cpp.o" \
"CMakeFiles/Client.dir/src/client/client_tcp.cpp.o" \
"CMakeFiles/Client.dir/src/client/client_udp.cpp.o" \
"CMakeFiles/Client.dir/src/client/main.cpp.o" \
"CMakeFiles/Client.dir/src/client/string_queue.cpp.o"

# External object files for target Client
Client_EXTERNAL_OBJECTS =

Client: CMakeFiles/Client.dir/src/lib/author.cpp.o
Client: CMakeFiles/Client.dir/src/lib/book.cpp.o
Client: CMakeFiles/Client.dir/src/lib/file_storage.cpp.o
Client: CMakeFiles/Client.dir/src/lib/json_parser.cpp.o
Client: CMakeFiles/Client.dir/src/lib/library.cpp.o
Client: CMakeFiles/Client.dir/src/lib/socket_tcp.cpp.o
Client: CMakeFiles/Client.dir/src/lib/socket_udp.cpp.o
Client: CMakeFiles/Client.dir/src/lib/xml_parser.cpp.o
Client: CMakeFiles/Client.dir/src/client/client.cpp.o
Client: CMakeFiles/Client.dir/src/client/client_tcp.cpp.o
Client: CMakeFiles/Client.dir/src/client/client_udp.cpp.o
Client: CMakeFiles/Client.dir/src/client/main.cpp.o
Client: CMakeFiles/Client.dir/src/client/string_queue.cpp.o
Client: CMakeFiles/Client.dir/build.make
Client: CMakeFiles/Client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_14) "Linking CXX executable Client"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Client.dir/build: Client

.PHONY : CMakeFiles/Client.dir/build

CMakeFiles/Client.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Client.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Client.dir/clean

CMakeFiles/Client.dir/depend:
	cd /home/kovalev_s/code/learn/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/kovalev_s/code/learn /home/kovalev_s/code/learn /home/kovalev_s/code/learn/cmake-build-debug /home/kovalev_s/code/learn/cmake-build-debug /home/kovalev_s/code/learn/cmake-build-debug/CMakeFiles/Client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Client.dir/depend

