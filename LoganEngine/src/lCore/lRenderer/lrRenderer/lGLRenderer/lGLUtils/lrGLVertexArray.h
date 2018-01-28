#ifndef LR_GL_VERTEX_ARRAY_H
#define LR_GL_VERTEX_ARRAY_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

class lrGLVertexArray
{
private:
	GLuint VertexArray = 0;
	
public:
	
	void Create()
	{
		glGenVertexArrays(1,&VertexArray);
	}
	
	void Bind()
	{
		glBindVertexArray(VertexArray);
	}
	
	~lrGLVertexArray()
	{
		glDeleteVertexArrays(1,&VertexArray);
	}
};

#endif // LR_GL_VERTEX_ARRAY_H
