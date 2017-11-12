
find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(TestMultiGpu_GLX TestMultiGpu_GLX.cpp)
target_link_libraries(TestMultiGpu_GLX X11 ${OPENGL_gl_LIBRARY})
