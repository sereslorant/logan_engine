#ifndef LR_GL3_3_FWD_SKM_PREPASS_SHADER_H
#define LR_GL3_3_FWD_SKM_PREPASS_SHADER_H

#include "../lrGLShader/lrGLShader.h"

class lrGL3_3FwdSkmPrePassShader : public lrGLShader
{
private:
	GLuint VsId;    /**< A vertex shader azonosítója */

	GLuint ProgramId;   /**< A shader program azonosítója */

	enum lGL3_3LayoutLocation
    {
        LR_GL3_POSITION_LOCATION  = 0,
        LR_GL3_BONE_ID_LOCATION   = 5,
        LR_GL3_WEIGHT_LOCATION    = 6,
    };

public:

	GLint GetModelViewProjectionMatrixLocation()
	{
		return glGetUniformLocation(ProgramId,"lr_ModelViewProjectionMatrix");
	}

	GLint GetBoneTransformLocation()
	{
		return glGetUniformLocation(ProgramId,"lr_BoneTransform[0]");
	}

	GLint GetPositionLocation()
	{
		return LR_GL3_POSITION_LOCATION;
	}

	GLint GetBoneIdLocation()
	{
		return LR_GL3_BONE_ID_LOCATION;
	}

	GLint GetWeightLocation()
	{
		return LR_GL3_WEIGHT_LOCATION;
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

	lrGL3_3FwdSkmPrePassShader(const std::string &vs_name)
	{
		std::string Source;

		lLoadText(vs_name,Source);
		VsId = LoadShader(Source,GL_VERTEX_SHADER);

		ProgramId = glCreateProgram();

		glAttachShader(ProgramId,VsId);

		glLinkProgram(ProgramId);
	}

	virtual ~lrGL3_3FwdSkmPrePassShader() override
	{
		glDetachShader(ProgramId,VsId);

		glDeleteShader(VsId);

		glDeleteProgram(ProgramId);
	}
};

#endif // LR_GL3_3_FWD_SKM_PREPASS_SHADER_H
