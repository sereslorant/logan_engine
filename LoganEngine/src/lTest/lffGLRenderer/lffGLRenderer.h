#ifndef LFFGL_RENDERER_H
#define LFFGL_RENDERER_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"

#include "lffGL2DLayer.h"
/*
#include "lffGL3DDumbLayer.h"
*/
#include "lffGL3DLayer/lffGL3DCachedLayer.h"

#include "../../lRenderer/lrViewport.h"
#include <list>

class lrGLViewport : public lrViewport
{
private:
	std::list<lrLayer *> Layers;
	lffGLResourceLoader &ResourceLoader;

public:

	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = new lffGL2DLayer;
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	virtual liLayer *Create3DLayer() override
	{
		//lrLayer *NewLayer = new lffGL3DDumbLayer(ResourceLoader);
		lrLayer *NewLayer = new lffGL3DCachedLayer(ResourceLoader);
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}

	void Draw()
	{
		glViewport(X,Y,Width,Height);
		//Ez kell, hogy a glClear() csak a viewportot érintse.
		glScissor(X,Y,Width,Height);
		//
		for(lrLayer *Layer : Layers)
		{
			Layer->Draw();
		}
	}

	lrGLViewport(int x,int y,int width,int height,lffGLResourceLoader &resource_loader)
		:lrViewport(x,y,width,height),ResourceLoader(resource_loader)
	{}

	virtual ~lrGLViewport() override
	{
		for(lrLayer *Layer : Layers)
		{
			delete Layer;
		}
	}
};

#include "../../lRenderer/lrFramebuffer.h"

class lffGLFramebuffer : public lrFramebuffer
{
private:
	std::list<lrGLViewport *> Viewports;
	lffGLResourceLoader &ResourceLoader;

public:

	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = new lrGLViewport(x,y,width,height,ResourceLoader);
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

	lffGLFramebuffer(unsigned int width,unsigned int height,lffGLResourceLoader &resource_loader)
		:lrFramebuffer(width,height),ResourceLoader(resource_loader)
	{}

	virtual ~lffGLFramebuffer() override
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			delete Viewport;
		}
	}
};

class lffGLRenderer : public liRenderer
{
private:
	//
	lffGLResourceLoader ResourceLoader;
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

	lffGLRenderer(unsigned int width,unsigned int height,liResourceManager &resource_manager)
		:ResourceLoader(resource_manager),MainFramebuffer(width,height,ResourceLoader)
	{
		glShadeModel(GL_SMOOTH);
		//
		glEnable(GL_BLEND);
		glEnable(GL_SCISSOR_TEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	virtual ~lffGLRenderer() override
	{
    	//
	}
};

#endif // LFFGL_RENDERER_H
