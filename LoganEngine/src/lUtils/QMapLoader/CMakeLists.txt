cmake_minimum_required(VERSION 2.6)
project(qmaploader)

set(CMAKE_CXX_STANDARD 14)

add_library(Q3BrushMesh Q3Geometry.cpp Q3BrushMesh.cpp WeMesh.cpp)

add_subdirectory(Test)
