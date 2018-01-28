#ifndef LR_GL_SKELETAL_MESH_RESOURCE_H
#define LR_GL_SKELETAL_MESH_RESOURCE_H

#include "../lrGLMeshData/lrGLVertexData.h"
#include "../lrGLMeshData/lrGLBoneData.h"
#include "../lrGLMeshData/lrGLPrimitiveData.h"

#include <vector>

struct lrGLSkeletalMeshResource
{
	lrGLVertexData	VertexData;
	lrGLBoneData	BoneData;
	std::vector<lrGLPrimitiveData> MaterialGroups;
	
	void Initialize(lrmSkeletalMesh &skeletal_mesh);
	
	lrGLSkeletalMeshResource()
	{}
	
	~lrGLSkeletalMeshResource()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_SKELETAL_MESH_RESOURCE_H
