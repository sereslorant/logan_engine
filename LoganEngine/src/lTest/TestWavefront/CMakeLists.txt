
find_package(GLUT)

set(TEST_WAVEFRONT_SRC TestWavefront.cpp)

add_executable(TestWavefront ${TEST_WAVEFRONT_SRC})
target_link_libraries(TestWavefront ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
