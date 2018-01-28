#ifndef LR_GL_TEXTURE_2D_VIEW
#define LR_GL_TEXTURE_2D_VIEW

#include "lrGLTextureView.h"

#include <lResourceManager/liResourceManager.h>

class lrGLTexture2DView : public lrGLTextureView<GL_TEXTURE_2D>
{
public:
	
	void Fill(const liBitmap2D &bitmap);
	
	lrGLTexture2DView()
	{}
	
	lrGLTexture2DView(lrGLTextureResource *texture_resource)
		:lrGLTextureView(texture_resource)
	{}
	
	~lrGLTexture2DView()
	{}
};


#endif // LR_GL_TEXTURE_2D_VIEW
