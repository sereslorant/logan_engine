
find_package(GLUT)

set(TEST_MD5_MESH_SRC TestMd5File.cpp)

add_executable(TestMd5File ${TEST_MD5_MESH_SRC})
target_link_libraries(TestMd5File Md5Loader ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})

set(TEST_MD5_ANIM_SRC TestMd5Anim.cpp)

add_executable(TestMd5Anim ${TEST_MD5_ANIM_SRC})
target_link_libraries(TestMd5Anim Md5Loader ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
