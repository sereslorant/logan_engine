#ifndef LR_GL_PING_PONG_BUFFER_H
#define LR_GL_PING_PONG_BUFFER_H

#include "lGLFunctions.h"

class lrGLPingPongBuffer
{
private:
	unsigned int Width;
	unsigned int Height;

	GLuint	 RenderTexture[2];
	GLuint	 DepthTexture[2];
	lrGLFramebufferImg	*Image[2];

	unsigned int Index;

	GLuint FBO;

public:

	unsigned int GetWidth(){return Width;}
	unsigned int GetHeight(){return Height;}

	void Resize(unsigned int width,unsigned int height)
	{
		Width  = width;
		Height = height;

		//Image[0]->SetWidth(Width);Image[0]->SetHeight(Height);
		Image[0]->Resize(Width,Height);

		//Image[1]->SetWidth(Width);Image[1]->SetHeight(Height);
		Image[1]->Resize(Width,Height);

		glBindTexture(GL_TEXTURE_2D, DepthTexture[0]);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,Width,Height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);

		glBindTexture(GL_TEXTURE_2D, DepthTexture[1]);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,Width,Height,0,GL_DEPTH_COMPONENT,GL_FLOAT,nullptr);
	}

	lrGLImg *GetImg()
	{
		unsigned int ImgId;
		if(Index == 0)
			{ImgId = 1;}
		else
			{ImgId = 0;}

		return Image[ImgId];
	}

	void Enable()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);
	}

	void Disable()
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	void Flip()
	{
		if(Index == 0)
			{Index = 1;}
		else
			{Index = 0;}

		glBindFramebuffer(GL_FRAMEBUFFER,FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,RenderTexture[Index],0);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT ,GL_TEXTURE_2D,DepthTexture[Index],0);

		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	lrGLPingPongBuffer(unsigned int width,unsigned int height)
		:Width(width),Height(height)
	{
		RenderTexture[0] = lGL_CreateEmptyTexture(Width,Height);
		RenderTexture[1] = lGL_CreateEmptyTexture(Width,Height);

		DepthTexture[0] = lGL_CreateEmptyTexture(Width,Height,true);
		DepthTexture[1] = lGL_CreateEmptyTexture(Width,Height,true);

		Image[0] = new lrGLFramebufferImg(Width,Height,GL_RGBA8,GL_RGBA,RenderTexture[0]);
		Image[1] = new lrGLFramebufferImg(Width,Height,GL_RGBA8,GL_RGBA,RenderTexture[1]);

		Index = 0;

		glGenFramebuffers(1,&FBO);
		glBindFramebuffer(GL_FRAMEBUFFER,FBO);

		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,RenderTexture[Index],0);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT ,GL_TEXTURE_2D,DepthTexture[Index],0);

		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}

	virtual ~lrGLPingPongBuffer()
	{
		glDeleteFramebuffers(1,&FBO);

		delete Image[0];
		delete Image[1];

		glDeleteTextures(1,&DepthTexture[0]);
		glDeleteTextures(1,&DepthTexture[1]);
	}

	/*
	 * Class vége
	 */
};

#endif // LR_GL_PING_PONG_BUFFER_H
