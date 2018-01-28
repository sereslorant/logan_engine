#ifndef LR_GL_VIEWPORT_H
#define LR_GL_VIEWPORT_H

#include "lGLIncludes.h"

#include <lRenderer/lrRenderer/lrLayer.h>
#include <lRenderer/lrRenderer/lrViewport.h>

#include <list>
#include <memory>

class liLayerFactory
{
public:
	virtual lrLayer *CreateLayer() = 0;
	
	liLayerFactory()
	{}
	virtual ~liLayerFactory()
	{}
};

class lrGLViewport : public lrViewport
{
private:
	liLayerFactory &Layer2DFactory;
	liLayerFactory &Layer3DFactory;
	
	std::list<std::unique_ptr<lrLayer> > Layers;
	
public:
	
	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = Layer2DFactory.CreateLayer();
		Layers.emplace_back(NewLayer);
		
		return NewLayer;
	}
	
	virtual liLayer *Create3DLayer() override
	{
		lrLayer *NewLayer = Layer3DFactory.CreateLayer();
		Layers.emplace_back(NewLayer);
		
		return NewLayer;
	}
	
	void Draw()
	{
		glViewport(X,Y,Width,Height);
		
		//Ez kell, hogy a glClear() csak a viewportot érintse.
		glScissor(X,Y,Width,Height);
		
		for(auto &Layer : Layers)
		{
			Layer->Draw();
		}
	}
	
	lrGLViewport(int x,int y,int width,int height,liLayerFactory &layer2d_factory,liLayerFactory &layer3d_factory)
		:lrViewport(x,y,width,height),Layer2DFactory(layer2d_factory),Layer3DFactory(layer3d_factory)
	{}
	
	virtual ~lrGLViewport() override
	{}
	/*
	 * End of class
	 */
};


#endif // LR_GL_VIEWPORT_H
