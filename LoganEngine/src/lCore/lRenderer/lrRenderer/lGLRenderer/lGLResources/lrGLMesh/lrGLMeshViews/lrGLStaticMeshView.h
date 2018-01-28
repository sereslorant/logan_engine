#ifndef LR_GL_STATIC_MESH_VIEW_H
#define LR_GL_STATIC_MESH_VIEW_H

#include "../lrGLMeshResources/lrGLStaticMeshResource.h"

#include "lrGLStaticMaterialGroupView.h"

class lrGLStaticMeshView
{
private:
	lrGLStaticMeshResource *StaticMeshResource;
	
public:
	
	unsigned int NumMtlGroups();
	
	lrGLStaticMaterialGroupView GetMaterialGroup(unsigned int id);
	
	lrGLStaticMeshView(lrGLStaticMeshResource *static_mesh_resource)
		:StaticMeshResource(static_mesh_resource)
	{}
	
	~lrGLStaticMeshView()
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_STATIC_MESH_VIEW_H
