#ifndef LR_GL_TEXTURE_RESOURCE
#define LR_GL_TEXTURE_RESOURCE

#include "../../lGLIncludes.h"

class lrGLTextureResource
{
private:
	GLuint TextureId = 0;
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
