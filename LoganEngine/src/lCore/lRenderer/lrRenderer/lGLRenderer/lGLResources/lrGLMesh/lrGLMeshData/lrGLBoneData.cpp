
#include "lrGLBoneData.h"

void lrGLBoneData::UploadData(lrmSkeletalMesh &skeletal_mesh)
{
	BoneIdBuffer.UploadData(
						GL_ARRAY_BUFFER,
						skeletal_mesh.BoneIds.size()*sizeof(lrmSkeletalMesh::lrmBoneId),
						&skeletal_mesh.BoneIds[0]
					);
	
	WeightBuffer.UploadData(
						GL_ARRAY_BUFFER,
						skeletal_mesh.Weights.size()*sizeof(lrmSkeletalMesh::lrmWeight),
						&skeletal_mesh.Weights[0]
					);
	
	glBindBuffer(GL_ARRAY_BUFFER,0);
}
