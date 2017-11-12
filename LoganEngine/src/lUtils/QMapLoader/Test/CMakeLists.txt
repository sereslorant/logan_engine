
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})
include_directories(${GLUT_INCLUDE_DIRS})

add_executable(TestCreateMesh TestCreateMesh.cpp TestWingedEdgeUtils.cpp)
target_link_libraries(TestCreateMesh Q3BrushMesh ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(TestWingedEdge TestWingedEdge.cpp TestWingedEdgeUtils.cpp)
target_link_libraries(TestWingedEdge Q3BrushMesh ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

add_executable(TestMapLoader TestMapLoader.cpp)
