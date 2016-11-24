#ifndef LR_GL_SKYBOX_H
#define LR_GL_SKYBOX_H

class lrGLSkybox
{
private:
	GLuint VertexBuffer;
	GLuint IndexBuffer;

	GLuint VAO;

public:

	void Draw()
	{
		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES,36,GL_UNSIGNED_INT,nullptr);

		glBindVertexArray(0);
	}

	lrGLSkybox(lrGLPostProcessShader *shader)
	{
		lmVector3D VertexArray[8]    = {{-1.0, 1.0, 1.0},
										{-1.0,-1.0, 1.0},
										{ 1.0,-1.0, 1.0},
										{ 1.0, 1.0, 1.0},
										{-1.0, 1.0,-1.0},
										{-1.0,-1.0,-1.0},
										{ 1.0,-1.0,-1.0},
										{ 1.0, 1.0,-1.0}};

		unsigned int IndexArray[36]  = {0,1,2,0,3,2,  //Front face
										2,3,6,3,7,6,  //Left face
										4,5,6,4,7,6,  //Back face
										0,1,5,0,5,4,  //Right face
										0,3,4,3,4,7,  //Top face
										1,2,6,1,5,6}; //Bottom face

		glGenBuffers(1,&VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER,8 * sizeof(lmVector3D),VertexArray,GL_STATIC_DRAW);

		glGenBuffers(1,&IndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,36 * sizeof(unsigned int),IndexArray,GL_STATIC_DRAW);

		GLint PositionLocation = shader->GetPositionLocation();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER,VertexBuffer);
		glEnableVertexAttribArray(PositionLocation);
		glVertexAttribPointer(PositionLocation,3,GL_FLOAT,GL_FALSE,0,0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBuffer);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}

	~lrGLSkybox()
	{
		glDeleteBuffers(1,&VertexBuffer);
		glDeleteBuffers(1,&IndexBuffer);

		glDeleteVertexArrays(1,&VAO);
	}

	/*
	 * Class vége
	 */
};

#endif // LR_GL_SKYBOX_H
