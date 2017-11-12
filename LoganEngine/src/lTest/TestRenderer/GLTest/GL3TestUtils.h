#ifndef GL3_TEST_UTILS_H
#define GL3_TEST_UTILS_H

#include <lRenderer/lrRenderer/lGLRenderer/lGLIncludes.h>
#include <lRenderer/lrRenderer/lGLRenderer/lGLExt.h>

//TMP!!!!!!!!!!!!!!!!!
using namespace lGLExt;
//TMP!!!!!!!!!!!!!!!!!

class lrGLVertexArray
{
private:
	GLuint VertexArray = 0;
	
public:
	
	void BindVertexArray()
	{
		glBindVertexArray(VertexArray);
	}
	
	lrGLVertexArray()
	{
		glGenVertexArrays(1,&VertexArray);
	}
	
	~lrGLVertexArray()
	{
		glDeleteVertexArrays(1,&VertexArray);
	}
};

#endif // GL3_TEST_UTILS_H
