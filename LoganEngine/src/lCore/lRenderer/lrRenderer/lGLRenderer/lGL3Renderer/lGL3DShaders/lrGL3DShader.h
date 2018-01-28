#ifndef LR_GL3D_SHADER_H
#define LR_GL3D_SHADER_H

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

class lrGL3DShader : public liGL3DShader
{
protected:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetCameraPositionLocation() override
	{
		return Shader.GetUniformLocation("CameraPosition");
	}
	
	virtual GLint GetProjectionMatrixLocation() override
	{
		return Shader.GetUniformLocation("ProjectionMatrix");
	}
	
	virtual GLint GetViewMatrixLocation() override
	{
		return Shader.GetUniformLocation("ViewMatrix");
	}
	
	virtual GLint GetModelMatrixLocation() override
	{
		return Shader.GetUniformLocation("ModelMatrix");
	}
	
	virtual GLint GetNormalMatrixLocation() override
	{
		return Shader.GetUniformLocation("NormalMatrix");
	}
	
	virtual GLint GetMvpMatrixLocation() override
	{
		return Shader.GetUniformLocation("MvpMatrix");
	}
	
	lrGL3DShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGL3DShader() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL3D_SHADER_H
