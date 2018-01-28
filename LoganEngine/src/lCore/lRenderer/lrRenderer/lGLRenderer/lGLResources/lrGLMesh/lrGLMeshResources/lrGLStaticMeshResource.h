#ifndef LR_GL_STATIC_MESH_RESOURCE_H
#define LR_GL_STATIC_MESH_RESOURCE_H

#include "../lrGLMeshData/lrGLVertexData.h"
#include "../lrGLMeshData/lrGLPrimitiveData.h"

#include <vector>

struct lrGLStaticMeshResource
{
	lrGLVertexData VertexData;
	std::vector<lrGLPrimitiveData> MaterialGroups;
	
	void Initialize(lrmStaticMesh &static_mesh);
	
	lrGLStaticMeshResource()
	{}
	
	~lrGLStaticMeshResource()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_STATIC_MESH_RESOURCE_H
