#ifndef LR_GL_SHADER_H
#define LR_GL_SHADER_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

class lrGLShader
{
protected:
	bool Initialized = false;
	
	GLuint ProgramId = 0;   /**< A shader program azonosítója */
	
public:
	
	GLint GetAttribLocation(const char *attrib_name)
	{
		return glGetAttribLocation(ProgramId,attrib_name);
	}
	
	GLint GetUniformLocation(const char *attrib_name)
	{
		return glGetUniformLocation(ProgramId,attrib_name);
	}
	
	void UseProgram()
	{
		glUseProgram(ProgramId);
	}
	
	void DisableProgram()
	{
		glUseProgram(0);
	}
	
	void Initialize(GLuint attached_shaders[],unsigned int attached_shader_count);
	
	lrGLShader()
	{}
	
	~lrGLShader();
	/*
	 * End of class
	 */
};

#endif // LR_GL_SHADER_H
