cmake_minimum_required(VERSION 3.8)
include(FindPkgConfig)
project(library)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE SOURCE_FILES src/lib/*.cpp)

file(GLOB_RECURSE SERVER_FILES src/server/*.cpp)

file(GLOB_RECURSE CLIENT_FILES src/client/*.cpp)

find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
#pkg_search_module(PUGIXML REQUIRED pugixml)

#set(SOURCE_FILES Library.cpp)
add_executable(library ${SOURCE_FILES} ${SERVER_FILES})
target_compile_options(library PUBLIC -std=c++17)
#target_compile_options(library PRIVATE -Wall -Werror -pedantic)
target_include_directories(library PRIVATE ${JSONCPP_INCLUDE_DIRS})
#target_include_directories(library PRIVATE ${PUGIXML_INCLUDE_DIRS})
target_link_libraries(library ${JSONCPP_LDFLAGS})
target_link_libraries(library pugixml)

find_package (Threads)
add_executable(Client ${SOURCE_FILES} ${CLIENT_FILES})
target_compile_options(Client PUBLIC -std=c++17)
target_include_directories(Client PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_link_libraries(Client ${JSONCPP_LDFLAGS})
target_link_libraries (Client ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Client pugixml)