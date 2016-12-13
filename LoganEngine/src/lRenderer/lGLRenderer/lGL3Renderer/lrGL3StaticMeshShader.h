#ifndef LR_GL3_STATIC_MESH_SHADER_H
#define LR_GL3_STATIC_MESH_SHADER_H

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

#include "../liGLShaderInterfaces.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#define PRINT_GL_SHADER_COMPILE_STATUS
#include <iostream>
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class lrGL3StaticMeshShader : public liGLShader, public liGLStaticMeshShader
{
private:
	GLuint VsId;    /**< A vertex shader azonosítója */
	GLuint FsId;    /**< A fragment shader azonosítója */
	//
	GLuint ProgramId;   /**< A shader program azonosítója */
	//
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
	virtual GLint GetMaterial0Location() override
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
	static GLuint LoadShader(const char *src,GLenum shader_type)
	{
		GLuint Id;
		Id = glCreateShader(shader_type);
		//
		glShaderSource(Id,1,&src,NULL);
		glCompileShader(Id);
		//
		#ifdef PRINT_GL_SHADER_COMPILE_STATUS
			char Error[1024];

			glGetShaderInfoLog(Id,1024,NULL,Error);
			std::cout << "Shader compile status:\n" << Error << std::endl;
		#endif
		//
		return Id;
	}
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
	lrGL3StaticMeshShader(const char *vertex_shader,const char *fragment_shader)
	{
		VsId = LoadShader(vertex_shader,GL_VERTEX_SHADER);
		FsId = LoadShader(fragment_shader,GL_FRAGMENT_SHADER);
		//
		ProgramId = glCreateProgram();
		//
		glAttachShader(ProgramId,VsId);
		glAttachShader(ProgramId,FsId);
		//
		glLinkProgram(ProgramId);
	}
	/*
	 * End of class
	 */
};

extern const char *VertexShaderSource;
extern const char *FragmentShaderSource;

#endif // LR_GL3_STATIC_MESH_SHADER_H
