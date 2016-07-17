#ifndef LFFGL_RENDERER_H
#define LFFGL_RENDERER_H

#include <GL/gl.h>

class lffGL2DSceneDrawer : public li2DSceneDrawer, public li2DSceneVisitor
{
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
			glColor3f(element.GetColor().GetRed(),element.GetColor().GetGreen(),element.GetColor().GetBlue());

			element.Accept(*this);
		}
	}
	//
	lffGL2DSceneDrawer(){}
	virtual ~lffGL2DSceneDrawer() override {}
};

#include "../lRenderer/lrLayer.h"

class lffGL2D_Layer : public lrLayer, public li2DLayer
{
private:
	li2DScene *Scene = nullptr;
	li2DCamera *Camera = nullptr;

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
		//
		lffGL2DSceneDrawer SceneDrawer;
		Scene->Draw(SceneDrawer);
	}

public:

	virtual void Accept(liLayerVisitor &layer_visitor) override
	{
		layer_visitor.Visit2DLayer(*this);
	}

	virtual void SetScene(li2DScene *scene) override
	{
		Scene = scene;
	}

	virtual void SetCamera(li2DCamera *camera) override
	{
		Camera = camera;
	}

	lffGL2D_Layer(){}
	virtual ~lffGL2D_Layer() override {}
};

#include "../lRenderer/lrViewport.h"
#include <list>

class lrGLViewport : public lrViewport
{
private:
	std::list<lrLayer *> Layers;

public:

	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = new lffGL2D_Layer;
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	void Draw()
	{
		glViewport(X,Y,Width,Height);
		//
		for(lrLayer *Layer : Layers)
		{
			Layer->Draw();
		}
	}

	lrGLViewport(int x,int y,int width,int height)
		:lrViewport(x,y,width,height)
	{}

	virtual ~lrGLViewport() override
	{
		for(lrLayer *Layer : Layers)
		{
			delete Layer;
		}
	}
};

#include "../lRenderer/lrFramebuffer.h"

class lffGLFramebuffer : public lrFramebuffer
{
private:
	std::list<lrGLViewport *> Viewports;

public:

	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = new lrGLViewport(x,y,width,height);
		Viewports.push_back(NewViewport);
		//
		return NewViewport;
	}

	void Draw()
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			Viewport->Draw();
		}
	}

	lffGLFramebuffer(unsigned int width,unsigned int height)
		:lrFramebuffer(width,height)
	{}

	virtual ~lffGLFramebuffer() override
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			delete Viewport;
		}
	}
};

class lffGLRenderer : public li2DRenderer
{
private:
	//
	lffGLFramebuffer MainFramebuffer;
	//
public:
	//
	virtual liFramebuffer &GetMainFramebuffer() override
	{
		return MainFramebuffer;
	}
	//
	virtual void Render() override
	{
		MainFramebuffer.Draw();
	}

	lffGLRenderer()
		:MainFramebuffer(800,600)
	{
		glShadeModel(GL_SMOOTH);
		//
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual ~lffGLRenderer() override
	{
    	//
	}
};

#endif // LFFGL_RENDERER_H
