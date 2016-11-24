
#include <iostream>

#include "lrGLFramebuffer.h"

#include "lGLFunctions.h"

lrImg *lrGLFramebuffer::GetColorBuffer()
{
	return Img;
}

void lrGLFramebuffer::Enable()
{
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);
}

void lrGLFramebuffer::Disable()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

lrGLFramebuffer::lrGLFramebuffer(unsigned int width,unsigned int height)
{
	ColorBuffer = lGL_CreateEmptyTexture(width,height);
	DepthBuffer = lGL_CreateEmptyTexture(width,height,true);

	glGenFramebuffers(1,&FBO);
	glBindFramebuffer(GL_FRAMEBUFFER,FBO);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ColorBuffer,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,DepthBuffer,0);

	glBindFramebuffer(GL_FRAMEBUFFER,0);

	Img = new lrGLFramebufferImg(width,height,GL_RGBA8,GL_RGBA,ColorBuffer);
}

lrGLFramebuffer::~lrGLFramebuffer()
{
	glDeleteFramebuffers(1,&FBO);

	delete Img;
	glDeleteTextures(1,&DepthBuffer);
}
