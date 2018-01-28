#ifndef LR_FRAMEBUFFER_H
#define LR_FRAMEBUFFER_H

#include <lRenderer/liRenderer.h>

class lrFramebuffer : public liFramebuffer2D
{
protected:
	unsigned int Width;
	unsigned int Height;
	
public:
	
	virtual unsigned int GetWidth() override
	{
		return Width;
	}
	
	virtual unsigned int GetHeight() override
	{
		return Height;
	}
	
	lrFramebuffer(unsigned int width,unsigned int height)
		:Width(width),Height(height)
	{}
	
	virtual ~lrFramebuffer() override
	{}
};

#endif
