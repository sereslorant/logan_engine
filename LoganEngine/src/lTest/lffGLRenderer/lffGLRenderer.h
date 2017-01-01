#ifndef LFFGL_RENDERER_H
#define LFFGL_RENDERER_H

#include "../../lRenderer/lGLRenderer/lGLIncludes.h"
#include "../../lRenderer/lGLRenderer/lrGLViewport.h"
#include "../../lRenderer/lGLRenderer/lrGLFramebuffer.h"

#include "lffGL2DLayer.h"
/*#include "lffGL3DDumbLayer.h"*/
#include "lffGL3DLayer/lffGL3DCachedLayer.h"

class lffGLViewport : public lrGLViewport
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
	virtual lrLayer *CreateGL2DLayer() override
	{
		return new lffGL2DLayer;
	}
	//
	virtual lrLayer *CreateGL3DLayer() override
	{
		return new lffGL3DCachedLayer(ResourceLoader);
	}
	//
public:
	//
	lffGLViewport(int x,int y,int width,int height,lffGLResourceLoader &resource_loader)
		:lrGLViewport(x,y,width,height),ResourceLoader(resource_loader)
	{

	}
	//
	virtual ~lffGLViewport() override
	{}
};

class lffGLFramebuffer : public lrGLFramebuffer
{
private:
	lffGLResourceLoader &ResourceLoader;
	//
	virtual lrGLViewport *CreateGLViewport(int x,int y,int width,int height) override
	{
		return new lffGLViewport(x,y,width,height,ResourceLoader);
	}
	//
public:
	//
	lffGLFramebuffer(unsigned int width,unsigned int height,lffGLResourceLoader &resource_loader)
		:lrGLFramebuffer(width,height),ResourceLoader(resource_loader)
	{}
	//
	virtual ~lffGLFramebuffer() override
	{
		//
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
	virtual liFramebuffer2D &GetMainFramebuffer() override
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
