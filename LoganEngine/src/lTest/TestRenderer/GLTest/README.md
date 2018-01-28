# GLTest
This folder contains test cases for the OpenGL Renderer.

# Test cases

## Test_GL3Renderer

This is the integration test for the GL renderer.
It checks whether the Renderer offers the functionality the engine needs in order to function properly.

## Test_GL3StaticMesh

This test checks whether the Static Mesh data structure loads properly.
We can observe the normal vectors and texture coordinates.

## Test_GL3SkeletalMesh

This test checks whether the Skeletal Mesh data structure loads properly.
We can observe the normal vectors and texture coordinates.
It also plays an animation so we can check whether the weights are loaded properly.

## Test_GL3TransformArray

This test checks whether the TransformArray functions properly.
The test case renders multiple static and skeletal meshes with the same mesh but different transform.

## Test_GL3SceneCache

This test checks whether the SceneCache data structure works properly
The test case renders multiple skeletal and static meshes with different textures and materials.
