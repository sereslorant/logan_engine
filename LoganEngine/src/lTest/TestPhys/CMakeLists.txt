cmake_minimum_required(VERSION 2.6)
project(glx_tutorial)

set(CMAKE_CXX_STANDARD 11)

include_directories("./")

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(TestPhys main.cpp)
target_link_libraries(TestPhys X11 EGL ${OPENGL_gl_LIBRARY})

add_subdirectory(Test_lPhys3)
