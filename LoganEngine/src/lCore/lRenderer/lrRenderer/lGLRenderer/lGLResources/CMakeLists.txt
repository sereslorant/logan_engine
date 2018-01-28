
set(LGL_MESH_DATA_SRC	lrGLMesh/lrGLMeshData/lrGLVertexData.cpp
						lrGLMesh/lrGLMeshData/lrGLPrimitiveData.cpp
						lrGLMesh/lrGLMeshData/lrGLBoneData.cpp
						)

set(LGL_MESH_RESOURCE_SRC	lrGLMesh/lrGLMeshResources/lrGLStaticMeshResource.cpp
							lrGLMesh/lrGLMeshResources/lrGLSkeletalMeshResource.cpp
							)

set(LGL_MESH_VIEW_SRC	lrGLMesh/lrGLMeshViews/lrGLStaticMaterialGroupView.cpp
						lrGLMesh/lrGLMeshViews/lrGLStaticMeshView.cpp
						lrGLMesh/lrGLMeshViews/lrGLSkeletalMaterialGroupView.cpp
						lrGLMesh/lrGLMeshViews/lrGLSkeletalMeshView.cpp
						)

set(LGL_TEXTURE_VIEW_SRC	lrGLTexture/lrGLTextureViews/lrGLTexture2DView.cpp
							lrGLTexture/lrGLTextureViews/lrGLTextureCubemapView.cpp
							)

add_library(lGLResourceLoader lrGLResourceLoader.cpp ${LGL_MESH_DATA_SRC} ${LGL_MESH_RESOURCE_SRC} ${LGL_MESH_VIEW_SRC} ${LGL_TEXTURE_VIEW_SRC})
