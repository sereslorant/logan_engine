#ifndef LI_FRAMEBUFFER_H
#define LI_FRAMEBUFFER_H

#include "liImg.h"

class liFramebuffer
{
public:

	virtual liImg *GetColorBuffer() = 0;

	virtual void Enable() = 0;
	virtual void Disable() = 0;

	liFramebuffer()
	{

	}

	virtual ~liFramebuffer()
	{

	}
};

#endif // LI_FRAMEBUFFER_H
