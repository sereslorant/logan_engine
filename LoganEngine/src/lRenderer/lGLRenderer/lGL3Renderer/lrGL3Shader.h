/*
 * lrGL3Shader.h
 *
 *  Created on: Jan 21, 2017
 *      Author: sereslorant
 */

#ifndef LR_GL3_SHADER_H_
#define LR_GL3_SHADER_H_

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

class lrGL3Shader
{
protected:
	bool Initialized = false;
	//
	unsigned int AttachedShaderCount = 0;
	GLuint *AttachedShaders = nullptr;
	//
	GLuint ProgramId = 0;   /**< A shader program azonosítója */
	//
public:
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
	void Initialize(GLuint attached_shaders[],unsigned int attached_shader_count)
	{
		ProgramId = glCreateProgram();
		//
		AttachedShaderCount = attached_shader_count;
		AttachedShaders = new GLuint[AttachedShaderCount];
		//
		for(unsigned int i=0;i < AttachedShaderCount;i++)
		{
			AttachedShaders[i] = attached_shaders[i];
			glAttachShader(ProgramId,AttachedShaders[i]);
		}
		//
		glLinkProgram(ProgramId);
		//
		Initialized = true;
	}
	//
	lrGL3Shader()
	{
		//
	}
	//
	virtual ~lrGL3Shader()
	{
		if(Initialized)
		{
			for(unsigned int i=0;i < AttachedShaderCount;i++)
			{
				glDetachShader(ProgramId,AttachedShaders[i]);
			}
			//
			delete AttachedShaders;
			//
			glDeleteProgram(ProgramId);
		}
	}
	//
	/*
	 * End of class
	 */
};


#endif /* LGLRENDERER_LGL3RENDERER_LRGL3SHADER_H_ */
