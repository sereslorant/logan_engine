
#include "lrGLSkeletalMeshResource.h"

void lrGLSkeletalMeshResource::Initialize(lrmSkeletalMesh &skeletal_mesh)
{
	VertexData.UploadData(skeletal_mesh.BindPoseMesh);
	BoneData.UploadData(skeletal_mesh);
	
	MaterialGroups.resize(skeletal_mesh.BindPoseMesh.GetNumMtlGroups());
	for(unsigned int i=0;i < MaterialGroups.size();i++)
	{
		MaterialGroups[i].UploadData(skeletal_mesh.BindPoseMesh.GetMaterialGroup(i));
	}
}
