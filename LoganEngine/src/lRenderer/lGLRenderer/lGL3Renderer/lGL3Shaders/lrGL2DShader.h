/*
 * lrGL2DShader.h
 *
 *  Created on: Jan 22, 2017
 *      Author: sereslorant
 */

#ifndef LR_GL_2D_SHADER_H_
#define LR_GL_2D_SHADER_H_

#include "../../liGLShaderInterfaces.h"

#include "lrGL3Shader.h"

class lrGL2DShader : public lrGL3Shader, public liGL2DShader, public liGL2DSolidColorShader
{
public:
	//
	virtual GLint GetCameraLocation() override
	{
		return glGetUniformLocation(ProgramId,"CameraMatrix");
	}
	//
	virtual GLint GetColorLocation() override
	{
		return glGetUniformLocation(ProgramId,"Color");
	}
	//
	virtual GLint GetTransformLocation() override
	{
		return glGetUniformLocation(ProgramId,"TransformMatrix");
	}
	//
	virtual GLint GetVertexLocation() override
	{
		return glGetAttribLocation(ProgramId,"Vertex");
	}
	//
	lrGL2DShader()
	{}
	//
	virtual ~lrGL2DShader() override
	{}
	/*
	 * End of class
	 */
};

#endif /* LR_GL_2D_SHADER_H_ */
