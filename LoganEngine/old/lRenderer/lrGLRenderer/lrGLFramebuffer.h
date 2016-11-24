#ifndef LR_GL_FRAMEBUFFER_H
#define LR_GL_FRAMEBUFFER_H

#include "../../lInterfaces/lRenderer/liFramebuffer.h"
#include "lrGLImg.h"

class lrGLFramebufferImg : public lrGLImg
{
protected:
	bool SizeModified;

	GLenum InternalFormat;
	GLenum Format;

public:

    void Resize(unsigned int width,unsigned int height)
    {
    	if(Width != width)
		{
			Width = width;
			SizeModified = true;
		}

    	if(Height != height)
		{
			Height = height;
			SizeModified = true;
		}

    	if(SizeModified == true)
		{
			glBindTexture(GL_TEXTURE_2D,Id);
			glTexImage2D(GL_TEXTURE_2D,0,InternalFormat,Width,Height,0,Format,GL_FLOAT,nullptr);

			SizeModified = false;
		}
    }

    lrGLFramebufferImg(unsigned int width,unsigned int height,GLenum internal_format,GLenum format,GLuint id)
		:lrGLImg(width,height,false,id),SizeModified(true),InternalFormat(internal_format),Format(format)
    {
    	Resize(width,height);
    }

    virtual ~lrGLFramebufferImg() override
    {

    }
};

class lrGLFramebuffer : public liFramebuffer
{
protected:
	GLuint FBO;

	GLuint ColorBuffer;
	GLuint DepthBuffer;

	lrGLFramebufferImg *Img;

public:

	virtual lrImg *GetColorBuffer() override;

	virtual void Enable() override;
	virtual void Disable() override;

	lrGLFramebuffer(unsigned int width,unsigned int height);
	virtual ~lrGLFramebuffer() override;
};

#endif // LR_GL_FRAMEBUFFER_H
