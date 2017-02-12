#ifndef LR_VIEWPORT_H
#define LR_VIEWPORT_H

#include <lRenderer/liRenderer.h>

class lrViewport : public liViewport
{
protected:
	int X;
	int Y;
	int Width;
	int Height;
	//
	//std::list<lrLayer *> Layers;
	//
public:
	//
	/*
	virtual liLayer *Create2DLayer() override
	{
		lrLayer *NewLayer = CreatePlatformSpecific2DLayer();
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}
	*/
	//
	/*
	virtual liLayer *Create3DLayer() override
	{
		lrLayer *NewLayer = CreatePlatformSpecific3DLayer();
		Layers.push_back(NewLayer);
		//
		return NewLayer;
	}
	*/
	//
	virtual int GetX() override
	{
		return X;
	}
	//
	virtual int GetY() override
	{
		return Y;
	}
	//
	virtual void SetX(int x) override
	{
		X = x;
	}
	//
	virtual void SetY(int y) override
	{
		Y = y;
	}
	//
	virtual int GetWidth() override
	{
		return Width;
	}
	//
	virtual int GetHeight() override
	{
		return Height;
	}
	//
	virtual void SetWidth(int width) override
	{
		Width = width;
	}
	//
	virtual void SetHeight(int height) override
	{
		Height = height;
	}
	//
	lrViewport(int x,int y,int width,int height)
		:X(x),Y(y),Width(width),Height(height)
	{}
	//
	virtual ~lrViewport() override
	{}
	//
	/*
	 * End of class
	 */
};


#endif // LR_VIEWPORT_H
