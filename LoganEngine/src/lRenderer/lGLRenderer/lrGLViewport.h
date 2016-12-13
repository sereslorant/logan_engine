#ifndef LR_GL_VIEWPORT_H
#define LR_GL_VIEWPORT_H

#include "lGLIncludes.h"

#include "../lrLayer.h"
#include "../lrViewport.h"
#include <list>

class lrGLViewport : public lrViewport
{
private:
	std::list<lrLayer *> Layers;
	//
	virtual lrLayer *CreateGL2DLayer() = 0;
	virtual lrLayer *CreateGL3DLayer() = 0;
	//
public:
	//
	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = CreateGL2DLayer();
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}
	//
	virtual liLayer *Create3DLayer() override
	{
		//lrLayer *NewLayer = new lffGL3DDumbLayer(ResourceLoader);
		lrLayer *NewLayer = CreateGL3DLayer();
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}
	//
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
	//
	lrGLViewport(int x,int y,int width,int height/*,lffGLResourceLoader &resource_loader*/)
		:lrViewport(x,y,width,height)//,ResourceLoader(resource_loader)
	{}
	//
	virtual ~lrGLViewport() override
	{
		for(lrLayer *Layer : Layers)
		{
			delete Layer;
		}
	}
	/*
	 * End of class
	 */
};


#endif // LR_GL_VIEWPORT_H
