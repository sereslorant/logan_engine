#ifndef LR_GL3_3_FWD_STM_PREPASS_SHADER_H
#define LR_GL3_3_FWD_STM_PREPASS_SHADER_H

#include "../lrGLShader/lrGLShader.h"

#include "../../../lApiAdapter/lDLLFunctions.h"

class lrGL3_3FwdStmPrePassShader : public lrGLShader
{
private:
	GLuint VsId;    /**< A vertex shader azonosítója */

	GLuint ProgramId;   /**< A shader program azonosítója */

	const int LR_V_POSITION_LOCATION = 0;

public:

	GLint GetModelViewProjectionMatrixLocation()
	{
		return glGetUniformLocation(ProgramId,"lr_ModelViewProjectionMatrix");
	}

	GLint GetPositionLocation()
	{
		return LR_V_POSITION_LOCATION;
	}

	virtual GLuint GetProgramId() override
	{
		return ProgramId;
	}

	virtual void UseProgram() override
	{
		glUseProgram(ProgramId);
	}

	virtual void DisableProgram() override
	{
		glUseProgram(0);
	}

	lrGL3_3FwdStmPrePassShader(const std::string &vs_name)
	{
		std::string Source;

		lLoadText(vs_name,Source);
		VsId = LoadShader(Source,GL_VERTEX_SHADER);

		ProgramId = glCreateProgram();

		glAttachShader(ProgramId,VsId);

		glLinkProgram(ProgramId);
	}

	virtual ~lrGL3_3FwdStmPrePassShader() override
	{
		glDetachShader(ProgramId,VsId);

		glDeleteShader(VsId);

		glDeleteProgram(ProgramId);
	}
};

#endif // LR_GL3_3_FWD_STM_PREPASS_SHADER_H
