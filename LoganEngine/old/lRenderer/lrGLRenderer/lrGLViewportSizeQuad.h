#ifndef LR_GL_VIEWPORT_SIZE_QUAD_H
#define LR_GL_VIEWPORT_SIZE_QUAD_H

#include "lrGLShader/lrGLPostProcessShader.h"

class lrGLViewportSizeQuad
{
private:
	GLuint VertexBuffer;
	GLuint TexCoordBuffer;

	GLuint IndexBuffer;

	GLuint VAO;

public:

	void Draw()
	{
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLE_FAN,4,GL_UNSIGNED_INT,nullptr);

		glBindVertexArray(0);
	}

	lrGLViewportSizeQuad(lrGLPostProcessShader *shader)
	{
		lmVector2D Vertices[4]  = {
			{-1.0,1.0},
			{-1.0,-1.0},
			{1.0,-1.0},
			{1.0,1.0}};
		lmVector2D TexCoords[4] = {{0.0,1.0} ,{0.0,0.0}  ,{1.0,0.0} ,{1.0,1.0}};
		unsigned int IndexBuffer[4] = {0,1,2,3};

		glGenBuffers(1,&this->VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,4 * sizeof(lmVector2D),Vertices,GL_STATIC_DRAW);

		glGenBuffers(1,&this->TexCoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);
		glBufferData(GL_ARRAY_BUFFER,4 * sizeof(lmVector2D),TexCoords,GL_STATIC_DRAW);

		glGenBuffers(1,&this->IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,4 * sizeof(unsigned int),IndexBuffer,GL_STATIC_DRAW);

		GLint PositionLocation = shader->GetPositionLocation();
		GLint TexCoordLocation = shader->GetTexCoordLocation();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glEnableVertexAttribArray(PositionLocation);
		glVertexAttribPointer(PositionLocation,2,GL_FLOAT,GL_FALSE,0,0);

		glBindBuffer(GL_ARRAY_BUFFER,TexCoordBuffer);
		glEnableVertexAttribArray(TexCoordLocation);
		glVertexAttribPointer(TexCoordLocation,2,GL_FLOAT,GL_FALSE,0,0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->IndexBuffer);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}

	~lrGLViewportSizeQuad()
	{
		glDeleteBuffers(1,&VertexBuffer);
		glDeleteBuffers(1,&TexCoordBuffer);

		glDeleteBuffers(1,&IndexBuffer);

		glDeleteVertexArrays(1,&VAO);
	}

	/*
	 * Class vége
	 */
};

#endif // LR_GL_VIEWPORT_SIZE_QUAD_H
