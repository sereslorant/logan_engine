
include_directories("./")

find_package(OpenGL)
include_directories(${OPENGL_INCLUDE_DIR})

add_executable(TestPhys main.cpp)
target_link_libraries(TestPhys ${SDL2_LIBRARIES} ${OPENGL_LIBRARIES})

add_subdirectory(Test_lPhys3)
