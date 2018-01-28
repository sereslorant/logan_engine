#ifndef LR_GL_BONE_DATA_H
#define LR_GL_BONE_DATA_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLUtils/lrGLBuffer.h>

#include <lResourceManager/liResourceManager.h>

struct lrGLBoneData
{
	unsigned int BufferLength = 0;	/**< A buffer objectek hossza */
	
	lrGLBuffer BoneIdBuffer;	/**< A bone indexeket tároló buffer object */
	lrGLBuffer WeightBuffer;	/**< A súlyokat tároló buffer object */
	
	void UploadData(lrmSkeletalMesh &skeletal_mesh);
	
	lrGLBoneData()
	{}
	
	~lrGLBoneData()
	{}
};

#endif // LR_GL_BONE_DATA_H
