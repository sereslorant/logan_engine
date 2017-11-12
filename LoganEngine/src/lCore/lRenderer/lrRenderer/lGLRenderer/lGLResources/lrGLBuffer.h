#ifndef LR_GL_BUFFER_H
#define LR_GL_BUFFER_H

#include "../lGLIncludes.h"
#include "../lGLExt.h"

//TMP!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!

class lrGLBuffer
{
private:
	GLuint Buffer = 0;
	
public:
	
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
