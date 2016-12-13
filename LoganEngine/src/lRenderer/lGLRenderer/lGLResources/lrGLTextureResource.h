#ifndef LR_GL_TEXTURE_RESOURCE
#define LR_GL_TEXTURE_RESOURCE

#include "../lGLIncludes.h"

class lrGLTextureResource
{
private:
	GLuint TextureId;
	//
public:
	//
	GLuint GetTextureId()
	{
		return TextureId;
	}
	//
	void Initialize()
	{
		glGenTextures(1,&TextureId);
	}
	//
	lrGLTextureResource()
		:TextureId(0)
	{
		//Empty
	}
	//
	~lrGLTextureResource()
	{
		glDeleteTextures(1,&TextureId);
	}
	/*
	 * End of class
	 */
};

class lrGLTexture2DView
{
private:
	lrGLTextureResource *TextureResource;
	//
public:
	//
	void Bind()
	{
		if(TextureResource != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D,TextureResource->GetTextureId());
		}
	}
	//
	void Unbind()
	{
		if(TextureResource != nullptr)
		{
			glBindTexture(GL_TEXTURE_2D,0);
		}
	}
	//
	void Fill(const liBitmap &bitmap)
	{
		if(TextureResource != nullptr)
		{
			Bind();
			//
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.GetWidth(), bitmap.GetHeight(), 0, GL_RGB, GL_FLOAT, bitmap.GetPixelArray());
			//
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//
			Unbind();
		}
	}
	//
	lrGLTexture2DView()
		:TextureResource(nullptr)
	{}
	//
	lrGLTexture2DView(lrGLTextureResource *texture_resource)
		:TextureResource(texture_resource)
	{}
	//
	~lrGLTexture2DView()
	{}
};

#endif // LR_GL_TEXTURE_RESOURCE
