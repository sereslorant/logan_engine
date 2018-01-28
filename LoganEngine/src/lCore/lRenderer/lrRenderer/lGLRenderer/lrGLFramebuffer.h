#ifndef LR_GL_FRAMEBUFFER_H
#define LR_GL_FRAMEBUFFER_H

#include "lGLIncludes.h"
#include "lrGLViewport.h"

#include <lRenderer/lrRenderer/lrFramebuffer.h>

#include <list>
#include <memory>

class lrGLFramebuffer : public lrFramebuffer
{
private:
	liLayerFactory &Layer2DFactory;
	liLayerFactory &Layer3DFactory;
	
	std::list<std::unique_ptr<lrGLViewport> > Viewports;
	
public:
	
	virtual liViewport *CreateViewport(int x,int y,int width,int height) override
	{
		lrGLViewport *NewViewport = new lrGLViewport(x,y,width,height,Layer2DFactory,Layer3DFactory);
		Viewports.emplace_back(NewViewport);
		
		return NewViewport;
	}
	
	void Draw()
	{
		for(auto &Viewport : Viewports)
		{
			Viewport->Draw();
		}
	}
	
	lrGLFramebuffer(unsigned int width,unsigned int height,liLayerFactory &layer2d_factory,liLayerFactory &layer3d_factory)
		:lrFramebuffer(width,height),Layer2DFactory(layer2d_factory),Layer3DFactory(layer3d_factory)
	{}
	
	virtual ~lrGLFramebuffer() override
	{}
	/*
	 * End of class
	 */
};

#endif // LR_GL_FRAMEBUFFER_H
