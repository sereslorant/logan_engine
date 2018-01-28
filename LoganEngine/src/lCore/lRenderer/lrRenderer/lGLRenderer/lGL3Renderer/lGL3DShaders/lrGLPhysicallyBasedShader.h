#ifndef LR_GL_PHYSICALLY_BASED_SHADER_H
#define LR_GL_PHYSICALLY_BASED_SHADER_H

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

class lrGLPhysicallyBasedShader : public liGLPbMatShader
{
protected:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetAlbedoMapLocation() override
	{
		return Shader.GetUniformLocation("AlbedoMap");
	}
	
	virtual GLint GetEnvironmentMapLocation() override
	{
		return Shader.GetUniformLocation("EnvironmentMap");
	}
	
	virtual GLint GetMatAlbedoLocation() override
	{
		return Shader.GetUniformLocation("MatAlbedo");
	}
	
	virtual GLint GetMaterial1Location() override
	{
		return Shader.GetUniformLocation("Material1");
	}
	
	lrGLPhysicallyBasedShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGLPhysicallyBasedShader() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_PHYSICALLY_BASED_SHADER_H
