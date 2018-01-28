
#include "lrGLShader.h"

void lrGLShader::Initialize(GLuint attached_shaders[],unsigned int attached_shader_count)
{
	ProgramId = glCreateProgram();
	
	for(unsigned int i=0;i < attached_shader_count;i++)
	{
		glAttachShader(ProgramId,attached_shaders[i]);
	}
	
	glLinkProgram(ProgramId);
	
	for(unsigned int i=0;i < attached_shader_count;i++)
	{
		glDetachShader(ProgramId,attached_shaders[i]);
	}
	
	Initialized = true;
}

lrGLShader::~lrGLShader()
{
	if(Initialized)
	{
		glDeleteProgram(ProgramId);
	}
}
