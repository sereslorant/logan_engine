/*
 * lrGL2DLayer.h
 *
 *  Created on: Jan 21, 2017
 *      Author: sereslorant
 */

#ifndef LR_GL2D_LAYER_H_
#define LR_GL2D_LAYER_H_

#include "../lGL3Shaders/lrGL2DShader.h"

#include "../../../lrUtils.h"

class lrGL2DSceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
{
private:
	lrGL2DShader &Shader;
	//
	GLuint RectVBO;
	GLuint RectEBO;
	//
	GLuint VAO;
	//
public:
	//
	virtual void VisitRectangle(const li2DRectangle &rectangle) override
	{
		lmMatrix3x3 TransformMatrix(lmMatrix3x3::IDENTITY);
		lrUtils::GetTransformMatrix(rectangle,TransformMatrix);
		//
		GLint TransformLocation = Shader.GetTransformLocation();
		glUniformMatrix3fv(TransformLocation,1,GL_FALSE,TransformMatrix[0]);
		//
		glBindVertexArray(VAO);
		//
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,RectEBO);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,nullptr);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
		//
		glBindVertexArray(0);
	}
	//
	virtual void Draw(const li2DElement &element) override
	{
		if(!element.IsHidden())
		{
			GLfloat Color[4] = {element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue(),element.GetColor().GetAlpha()};
			//
			GLint ColorLocation = Shader.GetColorLocation();
			glUniform4fv(ColorLocation,1,Color);
			//
			element.Accept(*this);
		}
	}
	//
	lrGL2DSceneDrawer(lrGL2DShader &shader)
		:Shader(shader)
	{
		glGenBuffers(1,&RectVBO);
		glBindBuffer(GL_ARRAY_BUFFER,RectVBO);
		//
		lmVector2D Vertices[4] = {{0.0f,0.0f},{0.0f,1.0f},{1.0f,1.0f},{1.0f,0.0f}};
		glBufferData(GL_ARRAY_BUFFER,sizeof(Vertices),&Vertices[0][0],GL_STATIC_DRAW);
		//
		glGenBuffers(1,&RectEBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,RectEBO);
		//
		GLuint IndexBuffer[6] = {0,1,2,0,2,3};
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(IndexBuffer),IndexBuffer,GL_STATIC_DRAW);
		//
		glGenVertexArrays(1,&VAO);
		glBindVertexArray(VAO);
		//
		glEnableVertexAttribArray(Shader.GetVertexLocation());
		glBindBuffer(GL_ARRAY_BUFFER,RectVBO);
		glVertexAttribPointer(Shader.GetVertexLocation(),2,GL_FLOAT,GL_FALSE,0,nullptr);
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER,0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	}
	//
	virtual ~lrGL2DSceneDrawer() override
	{
		glDeleteVertexArrays(1,&VAO);
		//
		glDeleteBuffers(1,&RectVBO);
		glDeleteBuffers(1,&RectEBO);
	}
	/*
	 * End of class
	 */
};

#include "../../../lrLayer.h"

class lrGL2DLayer : public lr2DLayer
{
private:
	lrGL2DShader &Shader;
	//
	virtual void DrawScene() override
	{
		if((Scene == nullptr) || (Camera == nullptr))
		{return;}
		//
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);
		//
		if(Scene->HasBackgroundColor())
		{
			glClearColor(Scene->GetBackgroundColor().GetRed(),Scene->GetBackgroundColor().GetGreen(),Scene->GetBackgroundColor().GetBlue(),Scene->GetBackgroundColor().GetAlpha());
			glClear(GL_COLOR_BUFFER_BIT);
		}
		//
		Shader.UseProgram();
		//
		lmMatrix3x3 CameraMatrix(lmMatrix3x3::IDENTITY);
		lrUtils::GetCameraMatrix(*Camera,CameraMatrix);
		//
		GLint CameraLocation = Shader.GetCameraLocation();
		glUniformMatrix3fv(CameraLocation,1,GL_FALSE,CameraMatrix[0]);
		//
		lrGL2DSceneDrawer SceneDrawer(Shader);
		Scene->Draw(SceneDrawer);
		//
		Shader.DisableProgram();
	}
	//
public:
	//
	lrGL2DLayer(lrGL2DShader &shader)
		:Shader(shader)
	{}
	//
	virtual ~lrGL2DLayer() override
	{}
	/*
	 * End of class
	 */
};

#endif /* LR_GL2D_LAYER_H_ */
