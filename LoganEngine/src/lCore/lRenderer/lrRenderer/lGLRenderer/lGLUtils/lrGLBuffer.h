#ifndef LR_GL_BUFFER_H
#define LR_GL_BUFFER_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

struct lrGLBuffer
{
	GLuint Buffer = 0;
	
	void BindBuffer(GLenum target)
	{
		glBindBuffer(target,Buffer);
	}
	
	void UploadData(GLenum target,unsigned int array_length,void *array)
	{
		BindBuffer(target);
		glBufferData(target,array_length,array,GL_STATIC_DRAW);
	}
	
	lrGLBuffer()
	{
		glGenBuffers(1,&Buffer);
	}
	
	~lrGLBuffer()
	{
		glDeleteBuffers(1,&Buffer);
	}
};

#endif // LR_GL_BUFFER_H
