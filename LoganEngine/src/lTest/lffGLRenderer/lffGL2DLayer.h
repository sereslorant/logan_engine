#ifndef LFF_GL_2D_LAYER_H
#define LFF_GL_2D_LAYER_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"

class lffGL2DSceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
{
public:
	//
	virtual void VisitRectangle(const li2DRectangle &rectangle) override
	{
		glBegin(GL_QUADS);

		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1]);
		glVertex2f(rectangle.GetPosition()[0] + rectangle.GetWidth()	,rectangle.GetPosition()[1] + rectangle.GetHeight());
		glVertex2f(rectangle.GetPosition()[0]							,rectangle.GetPosition()[1] + rectangle.GetHeight());

		glEnd();
	}
	//
	virtual void Draw(const li2DElement &element) override
	{
		if(!element.IsHidden())
		{
			glColor3f(element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue());

			element.Accept(*this);
		}
	}
	//
	lffGL2DSceneDrawer()
	{}
	//
	virtual ~lffGL2DSceneDrawer() override
	{}
};

#include "../../lRenderer/lrLayer.h"

class lffGL2DLayer : public lr2DLayer
{
private:
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
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(Camera->GetPosition()[0],
				Camera->GetPosition()[0] + Camera->GetWidth(),
				Camera->GetPosition()[1] + Camera->GetHeight(),
				Camera->GetPosition()[1],
				-1,1);
		//
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//
		lffGL2DSceneDrawer SceneDrawer;
		Scene->Draw(SceneDrawer);
	}
	//
public:
	//
	lffGL2DLayer(){}
	virtual ~lffGL2DLayer() override {}
};

#endif // LFF_GL_2D_LAYER_H
