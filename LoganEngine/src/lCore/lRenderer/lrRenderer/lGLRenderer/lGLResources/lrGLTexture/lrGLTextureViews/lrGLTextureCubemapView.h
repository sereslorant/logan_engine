#ifndef LR_GL_TEXTURE_CUBEMAP_VIEW
#define LR_GL_TEXTURE_CUBEMAP_VIEW

#include "lrGLTextureView.h"

#include <lResourceManager/liResourceManager.h>

class lrGLTextureCubemapView: public lrGLTextureView<GL_TEXTURE_CUBE_MAP>
{
public:
	
	void Fill(const int32_t cubemap_size,const void *cubemap_data[6]);
	
	lrGLTextureCubemapView()
	{}
	
	lrGLTextureCubemapView(lrGLTextureResource *texture_resource)
		:lrGLTextureView(texture_resource)
	{}
	
	~lrGLTextureCubemapView()
	{}
};

#endif // LR_GL_TEXTURE_CUBEMAP_VIEW
