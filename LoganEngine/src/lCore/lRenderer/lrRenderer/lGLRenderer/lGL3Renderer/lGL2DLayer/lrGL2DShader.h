#ifndef LR_GL_2D_SHADER_H
#define LR_GL_2D_SHADER_H

#include "../../liGLShaderInterfaces.h"

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

class lrGL2DShader : public liGL2DShader, public liGL2DSolidColorShader
{
private:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetCameraLocation() override
	{
		return Shader.GetUniformLocation("CameraMatrix");
	}
	
	virtual GLint GetColorLocation() override
	{
		return Shader.GetUniformLocation("Color");
	}
	
	virtual GLint GetTransformLocation() override
	{
		return Shader.GetUniformLocation("TransformMatrix");
	}
	
	virtual GLint GetVertexLocation() override
	{
		return Shader.GetAttribLocation("Vertex");
	}
	
	lrGL2DShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGL2DShader() override
	{}
	/*
	 * End of class
	 */
};

#endif /* LR_GL_2D_SHADER_H */
