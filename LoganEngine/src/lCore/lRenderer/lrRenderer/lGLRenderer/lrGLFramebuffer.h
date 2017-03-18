#ifndef LR_GL_FRAMEBUFFER_H
#define LR_GL_FRAMEBUFFER_H

#include "lGLIncludes.h"
#include "lrGLViewport.h"

#include "../lrFramebuffer.h"

class lrGLFramebuffer : public lrFramebuffer
{
private:
	std::list<lrGLViewport *> Viewports;
	//
	virtual lrGLViewport *CreateGLViewport(int x,int y,int width,int height) = 0;
	//
public:
	//
	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = CreateGLViewport(x,y,width,height);
		Viewports.push_back(NewViewport);
		//
		return NewViewport;
	}
	//
	void Draw()
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			Viewport->Draw();
		}
	}
	//
	lrGLFramebuffer(unsigned int width,unsigned int height)
		:lrFramebuffer(width,height)
	{}
	//
	virtual ~lrGLFramebuffer() override
	{
		for(lrGLViewport *Viewport : Viewports)
		{
			delete Viewport;
		}
	}
	/*
	 * End of class
	 */
};

#endif // LR_GL_FRAMEBUFFER_H
