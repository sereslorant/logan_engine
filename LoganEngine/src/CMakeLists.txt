cmake_minimum_required(VERSION 3.1)
project(LoganEngine)

set(CMAKE_CXX_STANDARD 11)

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

add_subdirectory(lExample)



#Compiling test environments

set(L_TEST_RESOURCE_MANAGER_MAIN_SRC_DIR "${L_SRC_DIR}/lTest/TestResourceManager")
set(L_TEST_RESOURCE_MANAGER_MAIN_SRC "${L_TEST_RESOURCE_MANAGER_MAIN_SRC_DIR}/TestResourceManager.cpp")

add_executable(Test_ResourceManager ${L_TEST_RESOURCE_MANAGER_MAIN_SRC})

set(L_TEST_GL3_MAIN_SRC_DIR "${L_SRC_DIR}/lTest/TestRenderer")
set(L_TEST_GL3_MAIN_SRC "${L_TEST_GL3_MAIN_SRC_DIR}/RendererTest.cpp" "${L_TEST_GL3_MAIN_SRC_DIR}/Test_lGL3Renderer.cpp")

add_executable(Test_GL3Renderer ${L_TEST_GL3_MAIN_SRC})
target_link_libraries(Test_GL3Renderer l3DScene ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)
