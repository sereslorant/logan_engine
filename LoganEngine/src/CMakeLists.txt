cmake_minimum_required(VERSION 3.1)
project(LoganEngine)

set(CMAKE_CXX_STANDARD 14)

#include(FindPkgConfig)
#pkg_search_module(SDL2 REQUIRED sdl2)
#pkg_search_module(OPENGL REQUIRED gl)


find_package(SDL2 REQUIRED)
find_package(OpenGL REQUIRED)

string(STRIP ${SDL2_INCLUDE_DIRS} SDL2_INCLUDE_DIRS)
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)


include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIRS})

set(L_SRC_DIR "./")
include_directories(${L_SRC_DIR})

set(L_CORE_DIR "${L_SRC_DIR}/lCore")
include_directories(${L_CORE_DIR})

add_subdirectory(lCore)
add_subdirectory(lUtils)

add_subdirectory(lExample)

add_subdirectory(lTest)

