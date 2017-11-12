#ifndef LR_GL_BONE_DATA_H
#define LR_GL_BONE_DATA_H

#include "../lrGLBuffer.h"

#include <lResourceManager/liResourceManager.h>

struct lrGLBoneData
{
	unsigned int BufferLength = 0;	/**< A buffer objectek hossza */
	
	lrGLBuffer BoneIdBuffer;	/**< A bone indexeket tároló buffer object */
	lrGLBuffer WeightBuffer;	/**< A súlyokat tároló buffer object */
	
	void UploadData(lrmSkeletalMesh &skeletal_mesh)
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
	
	lrGLBoneData()
	{}
	
	~lrGLBoneData()
	{
		//glDeleteBuffers(1,&BoneIdBuffer);
		//glDeleteBuffers(1,&WeightBuffer);
	}
};

#endif // LR_GL_BONE_DATA_H
