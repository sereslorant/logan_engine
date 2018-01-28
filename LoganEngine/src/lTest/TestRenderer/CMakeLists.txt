
set(L_TEST_RENDERER_SRC_DIR "./")
set(L_TEST_GL_SRC_DIR "${L_TEST_RENDERER_SRC_DIR}/GLTest")

set(L_TEST_GL3_STATIC_MESH_SRC "${L_TEST_GL_SRC_DIR}/Test_GL3StaticMesh.cpp")

set(L_TEST_GL3_SKELETAL_MESH_SRC "${L_TEST_GL_SRC_DIR}/Test_GL3SkeletalMesh.cpp")

set(L_TEST_GL3_TRANSFORM_ARRAY_SRC "${L_TEST_GL_SRC_DIR}/Test_GL3TransformArray.cpp")

set(L_TEST_GL3_CACHED_LAYER_SRC "${L_TEST_GL_SRC_DIR}/Test_GL3SceneCache.cpp")

set(L_TEST_GL3_RENDERER_SRC "${L_TEST_RENDERER_SRC_DIR}/RendererTest.cpp" "${L_TEST_GL_SRC_DIR}/Test_GL3Renderer.cpp")


add_executable(Test_GL3StaticMesh ${L_TEST_GL3_STATIC_MESH_SRC})
target_link_libraries(Test_GL3StaticMesh ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)

add_executable(Test_GL3SkeletalMesh ${L_TEST_GL3_SKELETAL_MESH_SRC})
target_link_libraries(Test_GL3SkeletalMesh ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)

add_executable(Test_GL3TransformArray ${L_TEST_GL3_TRANSFORM_ARRAY_SRC})
target_link_libraries(Test_GL3TransformArray ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)

add_executable(Test_GL3SceneCache ${L_TEST_GL3_CACHED_LAYER_SRC})
target_link_libraries(Test_GL3SceneCache ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/TestMeshes/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(Test_GL3Renderer ${L_TEST_GL3_RENDERER_SRC})
target_link_libraries(Test_GL3Renderer l3DScene ${SDL2_LIBRARIES} lGL3Renderer Md5Loader)
