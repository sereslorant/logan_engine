#ifndef LR_GL_SKELETAL_MESH_SHADER_H
#define LR_GL_SKELETAL_MESH_SHADER_H

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

class lrGLSkeletalMeshShader : public liGLSkeletalMeshShader
{
protected:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetBoneTransformLocation() override
	{
		return Shader.GetUniformLocation("BoneTransform[0]");
	}
	
	virtual GLint GetBoneIdLocation() override
	{
		return Shader.GetAttribLocation("BoneId");
	}
	
	virtual GLint GetWeightLocation() override
	{
		return Shader.GetAttribLocation("Weight");
	}
	
	lrGLSkeletalMeshShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGLSkeletalMeshShader() override
	{}
};

#endif // LR_GL_SKELETAL_MESH_SHADER_H
