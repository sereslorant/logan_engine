#ifndef LR_GL_TEXTURE_RESOURCE
#define LR_GL_TEXTURE_RESOURCE

#include "../../lGLIncludes.h"

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

#endif // LR_GL_TEXTURE_RESOURCE
