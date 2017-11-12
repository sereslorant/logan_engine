#ifndef GL_SHADER_H
#define GL_SHADER_H

#include <GL/gl.h>
#include <lCore/lMath/lMath.h>

extern const char *TestVertexShader;
extern const char *FragmentShaderSource;

extern PFNGLCREATESHADERPROC glCreateShader;
extern PFNGLSHADERSOURCEPROC glShaderSource;
extern PFNGLCOMPILESHADERPROC glCompileShader;
extern PFNGLDELETESHADERPROC glDeleteShader;

extern PFNGLGETOBJECTPARAMETERIVARBPROC glGetObjectParameterivARB;
extern PFNGLGETINFOLOGARBPROC glGetInfoLogARB;

extern PFNGLCREATEPROGRAMPROC glCreateProgram;
extern PFNGLATTACHSHADERPROC glAttachShader;
extern PFNGLDETACHSHADERPROC glDetachShader;
extern PFNGLLINKPROGRAMPROC glLinkProgram;
extern PFNGLUSEPROGRAMPROC glUseProgram;
extern PFNGLDELETEPROGRAMPROC glDeleteProgram;

extern PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
extern PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;

extern PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix4fv;

void PrintShaderCompileError(GLuint shader);

class GL_Shader
{
private:
	GLuint Program;
	//
	GLuint VertexShader;
	GLuint FragmentShader;
	//
public:
	//
	GLint GetPositionLocation() const
	{
		return glGetAttribLocation(Program,"Position");
	}
	//
	GLint GetTexCoordLocation() const
	{
		return glGetAttribLocation(Program,"TexCoord");
	}
	//
	void UploadMvpMatrix(const lmMatrix4x4 &mvp_matrix)
	{
		glUniformMatrix4fv(glGetUniformLocation(Program,"MvpMatrix"),1,GL_FALSE,&mvp_matrix[0][0]);
	}
	//
	void UseProgram()
	{
		glUseProgram(Program);
	}
	//
	void DisableProgram()
	{
		glUseProgram(0);
	}
	//
	GL_Shader()
	{
		VertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexShader,1,&TestVertexShader,nullptr);
		glCompileShader(VertexShader);
		//
		PrintShaderCompileError(VertexShader);
		//
		FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentShader,1,&FragmentShaderSource,nullptr);
		glCompileShader(FragmentShader);
		//
		PrintShaderCompileError(FragmentShader);
		//
		Program = glCreateProgram();
		//
		glAttachShader(Program,VertexShader);
		glAttachShader(Program,FragmentShader);
		//
		glLinkProgram(Program);
		//
		glDetachShader(Program,VertexShader);
		glDetachShader(Program,FragmentShader);
	}
	//
	~GL_Shader()
	{
		glUseProgram(0);
		//
		glDeleteProgram(Program);
		//
		glDeleteShader(VertexShader);
		glDeleteShader(FragmentShader);
	}
};

#endif // GL_SHADER_H
