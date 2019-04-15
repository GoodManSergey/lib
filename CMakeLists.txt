cmake_minimum_required(VERSION 3.5)
include(FindPkgConfig)
project(library)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_search_module(JSONCPP REQUIRED jsoncpp)
pkg_search_module(PUGIXML REQUIRED pugixml)

set(SOURCE_FILES Library.cpp)
add_executable(library ${SOURCE_FILES})
target_include_directories(library PRIVATE ${JSONCPP_INCLUDE_DIRS})
target_include_directories(library PRIVATE ${PUGIXML_INCLUDE_DIRS})
target_link_libraries(library ${JSONCPP_LDFLAGS})
