/*
 * lrGL2DLayer.h
 *
 *  Created on: Jan 21, 2017
 *      Author: sereslorant
 */

#ifndef LR_GL2D_LAYER_H_
#define LR_GL2D_LAYER_H_

#include "lrGL3Shader.h"

#include "../../liGLShaderInterfaces.h"

class liGL2DShader
{
public:
	//
	virtual GLint GetCameraLocation() = 0;
	//
	virtual GLint GetColorLocation() = 0;
	//
	liGL2DShader(){}
	virtual ~liGL2DShader(){}
	/*
	 * End of class
	 */
};

class lrGL2DShader : public lrGL3Shader, public liGL2DShader
{
public:
	//
	virtual GLint GetCameraLocation() override
	{
		return glGetUniformLocation(ProgramId,"CameraMatrix");
	}
	//
	/*
	virtual GLint GetTransformLocation() override
	{
		std::cout << "Transform location requested" << std::endl;
		return -1;
	}
	*/
	//
	virtual GLint GetColorLocation() override
	{
		return glGetUniformLocation(ProgramId,"Color");
	}
	//
	lrGL2DShader(){}
	virtual ~lrGL2DShader() override {}
};

class lrGL2DSceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
{
private:
	liGL2DShader &Shader;
	//
public:
	//
	virtual void VisitRectangle(li2DRectangle &rectangle) override
	{
		glBegin(GL_QUADS);

		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1] + rectangle.GetHeight());
		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1] + rectangle.GetHeight());

		glEnd();
	}
	//
	virtual void Draw(li2DElement &element) override
	{
		if(!element.IsHidden())
		{
			GLfloat Color[4] = {element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue(),element.GetColor().GetAlpha()}
			//glColor3f(element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue());
			GLint ColorLocation = Shader.GetColorLocation();
			glUniform4fv(ColorLocation,1,Color);
			//
			element.Accept(*this);
		}
	}
	//
	lrGL2DSceneDrawer(liGL2DShader &shader)
		:Shader(shader)
	{}
	//
	virtual ~lrGL2DSceneDrawer() override
	{}
	/*
	 * End of class
	 */
};

#include "../../../lrLayer.h"

#include "../../../lrUtils.h"

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
