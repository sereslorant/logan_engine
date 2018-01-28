#ifndef LR_GL_STATIC_MESH_SHADER_H
#define LR_GL_STATIC_MESH_SHADER_H

#include <lRenderer/lrRenderer/lGLRenderer/liGLShaderInterfaces.h>

#include <lRenderer/lrRenderer/lGLRenderer/lGLShaders/lrGLShader.h>

class lrGLStaticMeshShader : public liGLStaticMeshShader
{
protected:
	lrGLShader &Shader;
	
public:
	
	virtual GLint GetVertexLocation() override
	{
		return Shader.GetAttribLocation("Vertex");
	}
	
	virtual GLint GetNormalLocation() override
	{
		return Shader.GetAttribLocation("Normal");
	}
	
	virtual GLint GetTangentLocation() override
	{
		return Shader.GetAttribLocation("Tangent");
	}
	
	virtual GLint GetBitangentLocation() override
	{
		return Shader.GetAttribLocation("Bitangent");
	}
	
	virtual GLint GetTexCoordLocation() override
	{
		return Shader.GetAttribLocation("TexCoord");
	}
	
	lrGLStaticMeshShader(lrGLShader &shader)
		:Shader(shader)
	{}
	
	virtual ~lrGLStaticMeshShader() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_STATIC_MESH_SHADER_H
