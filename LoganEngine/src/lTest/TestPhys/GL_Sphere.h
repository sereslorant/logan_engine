#ifndef GL_SPHERE_H
#define GL_SPHERE_H

#include <GL/gl.h>

#include <lCore/lMath/lMath.h>

struct GL_VertexData
{
	lmVector2D Position;
	lmVector2D TexCoord;
};

#include <cmath>

extern PFNGLGENBUFFERSPROC glGenBuffers;
extern PFNGLBINDBUFFERPROC glBindBuffer;
extern PFNGLBUFFERDATAPROC glBufferData;
extern PFNGLDELETEBUFFERSPROC glDeleteBuffers;

extern PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
extern PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
extern PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
extern PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;
extern PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;

#include "GL_Shader.h"

class GL_Square
{
private:
	GLuint VBO;
	GLuint VAO;
	//
public:
	//
	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN,0,4);
	}
	//
	GL_Square(const GL_Shader &shader)
	{
		GL_VertexData VertexArray[4];
		
		VertexArray[0].Position = {-1.0f/2.0f,-1.0f/2.0f};
		VertexArray[0].TexCoord = {0.0,0.0};
		VertexArray[1].Position = { 1.0f/2.0f,-1.0f/2.0f};
		VertexArray[1].TexCoord = {1.0,0.0};
		VertexArray[2].Position = { 1.0f/2.0f, 1.0f/2.0f};
		VertexArray[2].TexCoord = {1.0,1.0};
		VertexArray[3].Position = {-1.0f/2.0f, 1.0f/2.0f};
		VertexArray[3].TexCoord = {0.0,1.0};
		
		glGenBuffers(1,&VBO);
		
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,4 * sizeof(GL_VertexData),VertexArray,GL_STATIC_DRAW);
		
		glGenVertexArrays(1,&VAO);
		
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		
		glEnableVertexAttribArray(shader.GetPositionLocation());
		glVertexAttribPointer(shader.GetPositionLocation(),2,GL_FLOAT,GL_FALSE,sizeof(GL_VertexData),0);
		
		glEnableVertexAttribArray(shader.GetTexCoordLocation());
		glVertexAttribPointer(shader.GetTexCoordLocation(),2,GL_FLOAT,GL_FALSE,sizeof(GL_VertexData),(const void *) sizeof(lmVector2D));
	}
	//
	~GL_Square()
	{
		glDeleteVertexArrays(1,&VAO);
		glDeleteBuffers(1,&VBO);
	}
	/*
	 * End of class
	 */
};

class GL_Sphere
{
private:
	int Resolution;
	GLuint VBO;
	GLuint VAO;
	//
public:
	//
	void Draw()
	{
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLE_FAN,0,Resolution);
	}
	//
	GL_Sphere(int resolution,const GL_Shader &shader)
		:Resolution(resolution)
	{
		GL_VertexData *VertexArray = new GL_VertexData[Resolution];
		
		float dAngle = (2.0*PI/Resolution);
		for(int i=0;i < Resolution;i++)
		{
			VertexArray[i].Position = lmVector2D({std::cos(i*dAngle),std::sin(i*dAngle)});
			VertexArray[i].TexCoord = (VertexArray[i].Position + lmVector2D({1.0f,1.0f})) * (1.0f / 2.0f);
		}
		
		glGenBuffers(1,&VBO);
		
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		glBufferData(GL_ARRAY_BUFFER,Resolution * sizeof(GL_VertexData),VertexArray,GL_STATIC_DRAW);
		
		glGenVertexArrays(1,&VAO);
		
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER,VBO);
		
		glEnableVertexAttribArray(shader.GetPositionLocation());
		glVertexAttribPointer(shader.GetPositionLocation(),2,GL_FLOAT,GL_FALSE,sizeof(GL_VertexData),0);
		
		glEnableVertexAttribArray(shader.GetTexCoordLocation());
		glVertexAttribPointer(shader.GetTexCoordLocation(),2,GL_FLOAT,GL_FALSE,sizeof(GL_VertexData),(const void *) sizeof(lmVector2D));
		
		delete [] VertexArray;
	}
	//
	~GL_Sphere()
	{
		glDeleteVertexArrays(1,&VAO);
		glDeleteBuffers(1,&VBO);
	}
	/*
	 * End of class
	 */
};

#endif // GL_SPHERE_H
