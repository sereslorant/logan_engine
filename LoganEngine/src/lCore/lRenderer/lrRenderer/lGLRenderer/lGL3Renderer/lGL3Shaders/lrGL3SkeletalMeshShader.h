#ifndef LR_GL3_SKELETAL_MESH_SHADER_H
#define LR_GL3_SKELETAL_MESH_SHADER_H

#include "lrGL3StaticMeshShader.h"

class lGL3SkeletalMeshShader : public lrGL3StaticMeshShader, public liGLSkeletalMeshShader
{
public:
	
	GLint GetBoneTransformLocation()
	{
		return glGetUniformLocation(ProgramId,"BoneTransform[0]");
	}
	
	virtual GLint GetBoneIdLocation() override
	{
		return glGetAttribLocation(ProgramId, "BoneId");
	}
	
	virtual GLint GetWeightLocation() override
	{
		return glGetAttribLocation(ProgramId, "Weight");
	}
	
	lGL3SkeletalMeshShader()
	{}
	
	virtual ~lGL3SkeletalMeshShader() override
	{}
};

#endif // LR_GL3_SKELETAL_MESH_SHADER_H
