
#include "lrGLTextureCubemapView.h"

void lrGLTextureCubemapView::Fill(const int32_t cubemap_size,const void *cubemap_data[6])
{
	if(TextureResource != nullptr)
	{
		Bind();
		
		GLenum InternalFormat = GL_RGB,Type = GL_FLOAT;
		for(int i=0;i < 6;i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, cubemap_size, cubemap_size, 0, InternalFormat, Type, cubemap_data[i]);
		}
		
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		
		Unbind();
	}
}
