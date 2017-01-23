#ifndef LR_GL3_STATIC_MESH_SHADER_H
#define LR_GL3_STATIC_MESH_SHADER_H

#include "lrGL3ShaderLibrary.h"

#include "lrGL3Shader.h"

#include "../liGLShaderInterfaces.h"

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
