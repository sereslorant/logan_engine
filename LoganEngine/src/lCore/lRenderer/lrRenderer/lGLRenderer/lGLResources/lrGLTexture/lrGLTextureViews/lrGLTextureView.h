#ifndef LR_GL_TEXTURE_VIEW
#define LR_GL_TEXTURE_VIEW

#include "../lrGLTextureResource.h"

template<GLenum Target_T>
class lrGLTextureView
{
protected:
	lrGLTextureResource *TextureResource;
	
public:
	
	GLuint GetTextureId()
	{
		return TextureResource->GetTextureId();
	}
	
	void Bind()
	{
		if(TextureResource != nullptr)
		{
			glBindTexture(Target_T,TextureResource->GetTextureId());
		}
		else
		{
			glBindTexture(Target_T,0);
		}
	}
	
	void Unbind()
	{
		if(TextureResource != nullptr)
		{
			glBindTexture(Target_T,0);
		}
	}
	
	lrGLTextureView()
		:TextureResource(nullptr)
	{}
	
	lrGLTextureView(lrGLTextureResource *texture_resource)
		:TextureResource(texture_resource)
	{}
	//
	~lrGLTextureView()
	{}
};

#endif // LR_GL_TEXTURE_VIEW
