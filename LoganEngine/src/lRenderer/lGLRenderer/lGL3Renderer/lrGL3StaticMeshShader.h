#ifndef LR_GL3_STATIC_MESH_SHADER_H
#define LR_GL3_STATIC_MESH_SHADER_H

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "lrGL3ShaderLibrary.h"

#include "../liGLShaderInterfaces.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PRINT_GL_SHADER_COMPILE_STATUS
#include <iostream>
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class lrGL3Shader
{
protected:
	bool Initialized = false;
	//
	unsigned int AttachedShaderCount = 0;
	GLuint *AttachedShaders = nullptr;
	//
	GLuint ProgramId = 0;   /**< A shader program azonosítója */
	//
public:
	//
	void UseProgram()
	{
		glUseProgram(ProgramId);
	}
	//
	void DisableProgram()
	{
		glUseProgram(0);
	}
	//
	void Initialize(GLuint attached_shaders[],unsigned int attached_shader_count)
	{
		ProgramId = glCreateProgram();
		//
		AttachedShaderCount = attached_shader_count;
		AttachedShaders = new GLuint[AttachedShaderCount];
		//
		for(unsigned int i=0;i < AttachedShaderCount;i++)
		{
			AttachedShaders[i] = attached_shaders[i];
			glAttachShader(ProgramId,AttachedShaders[i]);
		}
		//
		glLinkProgram(ProgramId);
		//
		Initialized = true;
	}
	//
	lrGL3Shader()
	{
		//
	}
	//
	virtual ~lrGL3Shader()
	{
		if(Initialized)
		{
			for(unsigned int i=0;i < AttachedShaderCount;i++)
			{
				glDetachShader(ProgramId,AttachedShaders[i]);
			}
			//
			delete AttachedShaders;
			//
			glDeleteProgram(ProgramId);
		}
	}
	//
	/*
	 * End of class
	 */
};

class lrGL3StaticMeshShader : public lrGL3Shader, public liGLShader, public liGLPbMatShader, public liGLPointLightShader, public liGLStaticMeshShader
{
public:
	//
	virtual GLint GetLightPositionLocation() override
	{
		return glGetUniformLocation(ProgramId, "LightPosition");
	}
	//
	virtual GLint GetLightColorLocation() override
	{
		return glGetUniformLocation(ProgramId, "LightColor");
	}
	//
	virtual GLint GetLightIntensityLocation() override
	{
		return glGetUniformLocation(ProgramId, "LightIntensity");
	}
	//
	virtual GLint GetCameraPositionLocation() override
	{
		return glGetUniformLocation(ProgramId, "CameraPosition");
	}
	//
	virtual GLint GetProjectionMatrixLocation() override
	{
		return glGetUniformLocation(ProgramId, "ProjectionMatrix");
	}
	//
	virtual GLint GetViewMatrixLocation() override
	{
		return glGetUniformLocation(ProgramId, "ViewMatrix");
	}
	//
	virtual GLint GetModelMatrixLocation() override
	{
		return glGetUniformLocation(ProgramId, "ModelMatrix");
	}
	//
	virtual GLint GetNormalMatrixLocation() override
	{
		return glGetUniformLocation(ProgramId, "NormalMatrix");
	}
	//
	virtual GLint GetMvpMatrixLocation() override
	{
		return glGetUniformLocation(ProgramId, "MvpMatrix");
	}
	//
	virtual GLint GetAlbedoMapLocation() override
	{
		return glGetUniformLocation(ProgramId, "AlbedoMap");
	}
	//
	virtual GLint GetEnvironmentMapLocation() override
	{
		return glGetUniformLocation(ProgramId, "EnvironmentMap");
	}
	//
	virtual GLint GetMatAlbedoLocation() override
	{
		return glGetUniformLocation(ProgramId, "MatAlbedo");
	}
	//
	virtual GLint GetMaterial1Location() override
	{
		return glGetUniformLocation(ProgramId, "Material1");
	}
	//
	virtual GLint GetVertexLocation() override
	{
		return glGetAttribLocation(ProgramId, "Vertex");
	}
	//
	virtual GLint GetNormalLocation() override
	{
		return glGetAttribLocation(ProgramId, "Normal");
	}
	//
	virtual GLint GetTangentLocation() override
	{
		return glGetAttribLocation(ProgramId, "Tangent");
	}
	//
	virtual GLint GetBitangentLocation() override
	{
		return glGetAttribLocation(ProgramId, "Bitangent");
	}
	//
	virtual GLint GetTexCoordLocation() override
	{
		return glGetAttribLocation(ProgramId, "TexCoord");
	}
	//
	lrGL3StaticMeshShader()
	{
		//
	}
	//
	virtual ~lrGL3StaticMeshShader() override
	{
		//
	}
	//
	/*
	 * End of class
	 */
};

#endif // LR_GL3_STATIC_MESH_SHADER_H
