
#include "lrGLTexture2DView.h"

void lrGLTexture2DView::Fill(const liBitmap2D &bitmap)
{
	if(TextureResource != nullptr)
	{
		Bind();
		
		GLenum InternalFormat = GL_RGB,Type = GL_FLOAT;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, bitmap.GetWidth(), bitmap.GetHeight(), 0, InternalFormat, Type, bitmap.GetPixelArray());
		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		
		Unbind();
	}
}
