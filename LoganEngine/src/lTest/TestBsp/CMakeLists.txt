
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(TestBsp BspMain.cpp Csirguru.cpp)
target_link_libraries(TestBsp Bsp ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
