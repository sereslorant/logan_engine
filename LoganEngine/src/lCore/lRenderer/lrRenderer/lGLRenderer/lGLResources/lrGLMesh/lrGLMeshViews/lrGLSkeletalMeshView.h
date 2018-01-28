#ifndef LR_GL_SKELETAL_MESH_VIEW_H
#define LR_GL_SKELETAL_MESH_VIEW_H

#include "../lrGLMeshResources/lrGLSkeletalMeshResource.h"

#include "lrGLSkeletalMaterialGroupView.h"

class lrGLSkeletalMeshView
{
private:
	lrGLSkeletalMeshResource *SkeletalMeshResource;
	
public:
	
	unsigned int NumMtlGroups();
	
	lrGLSkeletalMaterialGroupView GetMaterialGroup(unsigned int id);
	
	lrGLSkeletalMeshView(lrGLSkeletalMeshResource *skeletal_mesh_resource)
		:SkeletalMeshResource(skeletal_mesh_resource)
	{}
	
	~lrGLSkeletalMeshView()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_SKELETAL_MESH_VIEW_H
