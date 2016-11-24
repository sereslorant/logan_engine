
#include "lrGLPostProcessShader.h"

GLint lrGLPostProcessShader::GetPositionLocation()
{
	return LR_V_POSITION_LOCATION;
}

GLint lrGLPostProcessShader::GetTexCoordLocation()
{
	return LR_V_TEXCOORD_LOCATION;
}

lrGLPostProcessShader::lrGLPostProcessShader(const std::string &VsName,const std::string &FsName)
{
	std::string Source;

	lLoadText(VsName,Source);
	VsId = LoadShader(Source,GL_VERTEX_SHADER);

	Source.clear();

	lLoadText(FsName,Source);
	FsId = LoadShader(Source,GL_FRAGMENT_SHADER);

	ProgramId = glCreateProgram();

	glAttachShader(ProgramId,VsId);
	glAttachShader(ProgramId,FsId);

	glLinkProgram(ProgramId);
}

lrGLPostProcessShader::~lrGLPostProcessShader()
{
	glDetachShader(ProgramId,VsId);
	glDetachShader(ProgramId,FsId);

	glDeleteShader(VsId);
	glDeleteShader(FsId);

	glDeleteProgram(ProgramId);
}
