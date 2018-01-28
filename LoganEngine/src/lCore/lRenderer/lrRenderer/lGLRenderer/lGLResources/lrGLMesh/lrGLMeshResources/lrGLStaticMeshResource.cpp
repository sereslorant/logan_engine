
#include "lrGLStaticMeshResource.h"

void lrGLStaticMeshResource::Initialize(lrmStaticMesh &static_mesh)
{
	VertexData.UploadData(static_mesh);
	
	MaterialGroups.resize(static_mesh.GetNumMtlGroups());
	for(unsigned int i=0;i < MaterialGroups.size();i++)
	{
		//MaterialGroups[i].Initialize(static_mesh.GetMaterialGroup(i).IndexBuffer.size());
		MaterialGroups[i].UploadData(static_mesh.GetMaterialGroup(i));
	}
}
