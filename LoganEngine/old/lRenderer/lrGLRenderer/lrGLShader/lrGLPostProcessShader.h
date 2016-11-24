#ifndef LR_GL_POSTPROCESS_SHADER_H
#define LR_GL_POSTPROCESS_SHADER_H

#include "lrGLShader.h"

class lrGLPostProcessShader : public lrGLShader
{
private:
	GLuint VsId;    /**< A vertex shader azonosítója */
	GLuint FsId;    /**< A vertex shader azonosítója */

	GLuint ProgramId;   /**< A shader program azonosítója */

	enum lGL3_3LayoutLocation
    {
    	LR_V_POSITION_LOCATION = 0,
		LR_V_TEXCOORD_LOCATION = 1,
    };

public:

	GLint GetPositionLocation();
	GLint GetTexCoordLocation();

	virtual unsigned int GetProgramId() override
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

	lrGLPostProcessShader(const std::string &VsName,const std::string &FsName);
	virtual ~lrGLPostProcessShader() override;
};

#endif // LR_GL_POSTPROCESS_SHADER_H
